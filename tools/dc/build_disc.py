#!/usr/bin/env python3
"""Build a GDI image with a replacement boot executable, preserving asset LBAs."""

import argparse
import base64
from dataclasses import dataclass
import hashlib
import json
import os
from pathlib import Path
import shlex
import struct
import subprocess


@dataclass
class Track:
    number: int
    lba: int
    kind: int
    stride: int
    path: Path
    offset: int

    @property
    def count(self):
        return (self.path.stat().st_size - self.offset) // self.stride


def read_tracks(gdi):
    lines = gdi.read_text().splitlines()
    tracks = []
    for line in lines[1:]:
        fields = shlex.split(line)
        if fields:
            number, lba, kind, stride = map(int, fields[:4])
            tracks.append(Track(number, lba, kind, stride, (gdi.parent / fields[4]).resolve(), int(fields[5])))
    if len(tracks) != int(lines[0]):
        raise ValueError("GDI track count does not match its entries")
    return tracks


def find_track(tracks, lba):
    for track in tracks:
        if track.kind == 4 and track.lba <= lba < track.lba + track.count:
            return track
    raise ValueError(f"No data track contains LBA {lba}")


def raw_sector(track, lba):
    with track.path.open("rb") as source:
        source.seek(track.offset + (lba - track.lba) * track.stride)
        sector = source.read(track.stride)
    if len(sector) != track.stride:
        raise ValueError("Truncated sector")
    return sector


def payload_sector(tracks, lba):
    track = find_track(tracks, lba)
    sector = raw_sector(track, lba)
    if track.stride == 2048:
        return sector
    if track.stride != 2352 or sector[:12] != b"\0" + b"\xff" * 10 + b"\0" or sector[15] != 1:
        raise ValueError("This packager requires Mode 1 data sectors")
    return sector[16:2064]


def find_boot_file(tracks):
    session = next(track for track in tracks if track.kind == 4 and track.lba >= 45000)
    volume = payload_sector(tracks, session.lba + 16)
    if volume[:7] != b"\x01CD001\x01" or struct.unpack_from("<H", volume, 128)[0] != 2048:
        raise ValueError("Missing ISO9660 primary volume descriptor")
    boot_name = payload_sector(tracks, session.lba)[0x60:0x70].decode("ascii").strip()
    directory_lba = struct.unpack_from("<I", volume, 158)[0]
    directory_size = struct.unpack_from("<I", volume, 166)[0]
    directory = b"".join(payload_sector(tracks, directory_lba + n) for n in range((directory_size + 2047) // 2048))
    position = 0
    while position < directory_size:
        length = directory[position]
        if not length:
            position = (position // 2048 + 1) * 2048
            continue
        record = directory[position:position + length]
        name = record[33:33 + record[32]].decode("ascii").split(";")[0]
        if name == boot_name:
            if record[25] & 0x82:
                raise ValueError("Boot file must be a regular, single-extent ISO file")
            extent = struct.unpack_from("<I", record, 2)[0]
            size = struct.unpack_from("<I", record, 10)[0]
            return boot_name, extent, size, session.lba + 16, directory_lba
        position += length
    raise ValueError(f"Boot file {boot_name!r} is absent from the root directory")


def checksum_tables():
    edc, forward, reverse = [], [], [0] * 256
    for value in range(256):
        doubled = ((value << 1) ^ (0x11d if value & 0x80 else 0)) & 255
        forward.append(doubled)
        reverse[value ^ doubled] = value
        crc = value
        for bit in range(8):
            crc = (crc >> 1) ^ (0xd8018001 if crc & 1 else 0)
        edc.append(crc)
    return edc, forward, reverse


EDC, GF_FORWARD, GF_REVERSE = checksum_tables()


def write_parity(sector, columns, rows, column_stride, row_stride, destination):
    length = columns * rows
    for column in range(columns):
        index = (column // 2) * column_stride + column % 2
        first, second = 0, 0
        for row in range(rows):
            value = sector[12 + index]
            index = (index + row_stride) % length
            first = GF_FORWARD[first ^ value]
            second ^= value
        first = GF_REVERSE[GF_FORWARD[first] ^ second]
        sector[destination + column] = first
        sector[destination + columns + column] = first ^ second


def finish_sector(sector):
    crc = 0
    for value in sector[:2064]:
        crc = (crc >> 8) ^ EDC[(crc ^ value) & 255]
    struct.pack_into("<I", sector, 2064, crc)
    sector[2068:2076] = bytes(8)
    write_parity(sector, 86, 24, 2, 86, 2076)
    write_parity(sector, 52, 43, 86, 88, 2248)


def confirm_sector_checksums(track, lba):
    sector = raw_sector(track, lba)
    rebuilt = bytearray(sector)
    finish_sector(rebuilt)
    if rebuilt != sector:
        raise ValueError(f"Source sector {lba} does not match the Mode 1 checksum format")


def reference_directory(destination, source):
    if destination.exists():
        if not destination.samefile(source):
            raise ValueError(f"Existing track reference points elsewhere: {destination}")
        return
    if os.name == "nt":
        subprocess.run(["cmd.exe", "/d", "/c", "mklink", "/J", str(destination), str(source)], check=True, capture_output=True)
    elif os.environ.get("WSL_DISTRO_NAME"):
        windows_destination = subprocess.check_output(["wslpath", "-w", str(destination)]).decode().strip()
        windows_source = subprocess.check_output(["wslpath", "-w", str(source)]).decode().strip()
        quote = lambda value: "'" + value.replace("'", "''") + "'"
        command = "$ErrorActionPreference='Stop'; New-Item -ItemType Junction -Path " + quote(windows_destination)
        command += " -Target " + quote(windows_source) + " | Out-Null"
        encoded = base64.b64encode(command.encode("utf-16le")).decode()
        subprocess.run(["powershell.exe", "-NoProfile", "-NonInteractive", "-EncodedCommand", encoded], check=True, capture_output=True)
    else:
        destination.symlink_to(source, target_is_directory=True)


def copy_bytes(source, destination, remaining):
    while remaining:
        block = source.read(min(8 * 1024 * 1024, remaining))
        if not block:
            raise ValueError("Truncated source track")
        destination.write(block)
        remaining -= len(block)


def build(gdi, binary, output):
    tracks = read_tracks(gdi)
    boot_name, extent, size, volume_lba, directory_lba = find_boot_file(tracks)
    count = (size + 2047) // 2048
    track = find_track(tracks, extent)
    if track.stride != 2352 or find_track(tracks, extent + count - 1) != track:
        raise ValueError("Boot extent must fit entirely in one raw Mode 1 track")
    executable = binary.read_bytes()
    if not executable or len(executable) > size or executable.startswith(b"\x7fELF"):
        raise ValueError(f"Supply a nonempty flat binary no larger than {size} bytes")
    if output == gdi.parent or gdi.parent in output.parents:
        raise ValueError("Output must be separate from the source image")
    for lba in (volume_lba, directory_lba):
        confirm_sector_checksums(find_track(tracks, lba), lba)
    output.mkdir(parents=True, exist_ok=True)
    reference_directory(output / "source_tracks", gdi.parent)
    destination = output / track.path.name
    temporary = destination.with_suffix(destination.suffix + ".tmp")
    start = track.offset + (extent - track.lba) * track.stride
    end = start + count * track.stride
    padded = executable.ljust(count * 2048, b"\0")
    with track.path.open("rb") as source, temporary.open("w+b") as image:
        copy_bytes(source, image, start)
        for index in range(count):
            source.seek(start + index * track.stride)
            header = source.read(16)
            if header[:12] != b"\0" + b"\xff" * 10 + b"\0" or header[15] != 1:
                raise ValueError("Boot extent contains a sector other than Mode 1")
            sector = bytearray(2352)
            sector[:16] = header
            sector[16:2064] = padded[index * 2048:(index + 1) * 2048]
            finish_sector(sector)
            image.write(sector)
        source.seek(end)
        copy_bytes(source, image, track.path.stat().st_size - end)
        image.flush()
        image.seek(start)
        for index in range(count):
            sector = image.read(2352)
            if sector[16:2064] != padded[index * 2048:(index + 1) * 2048]:
                raise ValueError("Written boot extent does not match the executable and zero padding")
            rebuilt = bytearray(sector)
            finish_sector(rebuilt)
            if rebuilt != sector:
                raise ValueError("Written boot extent has incorrect checksums")
    temporary.replace(destination)
    rows = [str(len(tracks))]
    for entry in tracks:
        filename = entry.path.name if entry.number == track.number else "source_tracks/" + entry.path.name
        rows.append(f"{entry.number} {entry.lba} {entry.kind} {entry.stride} {filename} {entry.offset}")
    image_gdi = output / "shenmue.gdi"
    image_gdi.write_text("\n".join(rows) + "\n")
    return {"image": str(image_gdi), "boot_file": boot_name, "extent_lba": extent, "iso_file_size": size,
            "binary_size": len(executable), "binary_sha256": hashlib.sha256(executable).hexdigest(),
            "replaced_sectors": count, "zero_padding_bytes": len(padded) - len(executable), "copied_track": track.number}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--gdi", required=True, type=Path)
    parser.add_argument("--binary", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    print(json.dumps(build(args.gdi.resolve(), args.binary.resolve(), args.output.resolve()), indent=2))


if __name__ == "__main__":
    main()
