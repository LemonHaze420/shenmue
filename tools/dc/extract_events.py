#!/usr/bin/env python3
"""Extract loose and packaged EV overlays into a local build cache."""

import argparse
import gzip
import hashlib
import io
import json
import os
from pathlib import Path
import shlex
import struct
import subprocess
import zipfile

from build_disc import read_tracks


CONTAINERS = (b'PAKF', b'PAKS', b'IPAC', b'AFS\0')
MAX_PACKAGE_BYTES = 256 * 1024 * 1024


def component(value):
    name = value.rstrip(b'\0 ').decode('ascii')
    if not name or name in ('.', '..') or any(c in name for c in '/\\:'):
        raise ValueError('Invalid archive filename')
    return name


def write(path, contents):
    if not path.is_file() or path.read_bytes() != contents:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(contents)


class Events:
    def __init__(self, destination):
        self.destination = destination
        self.entries = []
        self.paths = {}
        self.packages = 0

    def event(self, path, contents):
        sha = hashlib.sha256(contents).hexdigest()
        if path in self.paths and self.paths[path] != sha:
            raise ValueError(f'Different EV files share an extraction path: {path}')
        self.paths[path] = sha
        write(self.destination / path, contents)
        self.entries.append({'path': path.as_posix(), 'bytes': len(contents), 'sha256': sha})

    def package(self, read, size, path, depth=0):
        if depth > 12:
            raise ValueError('Archive nesting exceeds 12 levels')
        head = read(0, min(16, size))
        if head[:2] == b'\x1f\x8b':
            if size > MAX_PACKAGE_BYTES:
                raise ValueError('Compressed package exceeds 256 MiB')
            with gzip.GzipFile(fileobj=io.BytesIO(read(0, size))) as stream:
                data = stream.read(MAX_PACKAGE_BYTES + 1)
            if len(data) > MAX_PACKAGE_BYTES:
                raise ValueError('Decompressed package exceeds 256 MiB')
            self.package(lambda offset, count: data[offset:offset + count], len(data), path, depth + 1)
            return
        tag = head[:4]
        if tag not in CONTAINERS:
            return
        if len(head) != 16:
            raise ValueError('Truncated archive header')
        self.packages += 1
        if tag == b'AFS\0':
            count = struct.unpack_from('<I', head, 4)[0]
            if count > 100000 or 8 + count * 8 > size:
                raise ValueError('Invalid AFS directory')
            table = read(8, count * 8)
            if len(table) != count * 8:
                raise ValueError('Truncated AFS directory')
            for index in range(count):
                offset, length = struct.unpack_from('<II', table, index * 8)
                if offset + length > size:
                    raise ValueError('AFS member is outside its archive')
                if length:
                    self.package(lambda at, n, offset=offset: read(offset + at, n), length,
                                 path / f'{index:04d}', depth + 1)
            return
        base = struct.unpack_from('<I', head, 4)[0] if tag in (b'PAKF', b'PAKS') else 0
        if base + 16 > size:
            raise ValueError('IPAC header is outside its package')
        directory = read(base, 16)
        if len(directory) != 16:
            raise ValueError('Truncated IPAC header')
        tag, unknown, count, table_offset = struct.unpack('<4sIII', directory)
        if tag != b'IPAC' or count > 100000 or base + table_offset + count * 20 > size:
            raise ValueError('Invalid IPAC directory')
        table = read(base + table_offset, count * 20)
        if len(table) != count * 20:
            raise ValueError('Truncated IPAC directory')
        for index in range(count):
            name, kind, offset, length = struct.unpack_from('<8s4sII', table, index * 20)
            if base + offset + length > size:
                raise ValueError('IPAC member is outside its package')
            if not length:
                continue
            at = base + offset
            kind = kind.rstrip(b'\0 ')
            if kind in (b'EV1', b'EV2', b'EV3'):
                contents = read(at, length)
                if len(contents) != length:
                    raise ValueError('Truncated EV member')
                self.event(path / (component(name) + '.' + kind.decode('ascii')), contents)
            else:
                head = read(at, min(4, length))
                if head[:4] in CONTAINERS or head[:2] == b'\x1f\x8b':
                    self.package(lambda offset, n, at=at: read(at + offset, n), length,
                                 path / (component(name) + '.' + component(kind)), depth + 1)

    def file(self, read, size, path):
        if path.suffix.upper() in ('.EV1', '.EV2', '.EV3'):
            contents = read(0, size)
            if len(contents) != size:
                raise ValueError(f'Truncated EV: {path}')
            self.event(path, contents)
        else:
            self.package(read, size, path)


def iso_files(read, session=45000):
    volume = read(session + 16, 2048)
    if volume[:7] != b'\x01CD001\x01' or struct.unpack_from('<H', volume, 128)[0] != 2048:
        raise ValueError('Missing ISO9660 primary volume descriptor')
    pending = [(Path(), struct.unpack_from('<I', volume, 158)[0], struct.unpack_from('<I', volume, 166)[0])]
    visited, files = set(), []
    while pending:
        path, extent, size = pending.pop()
        if (extent, size) in visited:
            raise ValueError('Repeated ISO9660 directory extent')
        visited.add((extent, size))
        data = read(extent, size)
        if len(data) != size:
            raise ValueError('Truncated ISO9660 directory')
        offset = 0
        while offset < size:
            length = data[offset]
            if not length:
                offset = (offset // 2048 + 1) * 2048
                continue
            record = data[offset:offset + length]
            if length < 34 or offset + length > size or 33 + record[32] > length:
                raise ValueError('Invalid ISO9660 directory record')
            offset += length
            name = record[33:33 + record[32]]
            if name in (b'\0', b'\1'):
                continue
            child = path / component(name.split(b';')[0])
            position, length = struct.unpack_from('<I', record, 2)[0], struct.unpack_from('<I', record, 10)[0]
            if record[25] & 0x80:
                raise ValueError('Multi-extent ISO9660 files are unsupported')
            if record[25] & 2:
                pending.append((child, position, length))
            else:
                files.append((child, position, length))
    return sorted(files, key=lambda row: row[1])


def extract_gdi(source, events):
    tracks = read_tracks(source)
    handles = {track.number: track.path.open('rb') for track in tracks if track.kind == 4}

    def read(extent, size):
        track = next((t for t in tracks if t.kind == 4 and t.lba <= extent < t.lba + t.count), None)
        if track is None or extent + (size + 2047) // 2048 > track.lba + track.count:
            raise ValueError('ISO extent is outside its data track')
        stream = handles[track.number]
        stream.seek(track.offset + (extent - track.lba) * track.stride)
        raw = stream.read(((size + 2047) // 2048) * track.stride)
        return payload(raw, track.stride)[:size]

    try:
        session = next(track.lba for track in tracks if track.kind == 4 and track.lba >= 45000)
        for path, extent, size in iso_files(read, session):
            head = read(extent, min(16, size)) if size else b''
            if path.suffix.upper() in ('.EV1', '.EV2', '.EV3') or head[:4] in CONTAINERS or head[:2] == b'\x1f\x8b':
                data = read(extent, size)
                events.file(lambda offset, n: data[offset:offset + n], size, path)
    finally:
        for stream in handles.values():
            stream.close()


def payload(raw, stride):
    if stride == 2048:
        return raw
    if stride != 2352 or len(raw) % stride:
        raise ValueError('Invalid data-sector extent')
    parts = []
    for offset in range(0, len(raw), stride):
        if raw[offset:offset + 12] != b'\0' + b'\xff' * 10 + b'\0' or raw[offset + 15] != 1:
            raise ValueError('Expected Mode 1 data sectors')
        parts.append(raw[offset + 16:offset + 2064])
    return b''.join(parts)


def extract_directory(source, events):
    for directory, folders, files in os.walk(source):
        for name in sorted(files):
            path = Path(directory, name)
            with path.open('rb') as stream:
                size = os.fstat(stream.fileno()).st_size

                def read(offset, count):
                    stream.seek(offset)
                    return stream.read(count)

                events.file(read, size, path.relative_to(source))


def read_exact(stream, size):
    chunks = []
    while size:
        chunk = stream.read(min(size, 1024 * 1024))
        if not chunk:
            raise ValueError('Truncated archive track')
        chunks.append(chunk)
        size -= len(chunk)
    return b''.join(chunks)


def sector_lba(sector):
    payload(sector, 2352)

    def bcd(value):
        if value >> 4 > 9 or value & 15 > 9:
            raise ValueError('Invalid sector address')
        return (value >> 4) * 10 + (value & 15)

    minute, second, frame = (bcd(value) for value in sector[12:15])
    if second >= 60 or frame >= 75:
        raise ValueError('Invalid sector address')
    return (minute * 60 + second) * 75 + frame - 150


def extract_track(stream, track, events, catalogue):
    stride, size = track['stride'], track['size']
    prefix = read_exact(stream, min(size, 16 * 1024 * 1024))
    base = track.get('lba')
    if base is None:
        base = sector_lba(prefix[:stride])
    if base < 45000:
        raise ValueError('Expected a high-density data track')
    if catalogue is None:
        def read(extent, count):
            offset = track.get('offset', 0) + (extent - base) * stride
            end = offset + ((count + 2047) // 2048) * stride
            if offset < 0 or end > len(prefix):
                raise ValueError('ISO directory exceeds the archive track prefix')
            return payload(prefix[offset:end], stride)[:count]

        catalogue = iso_files(read, base)
    position = 0

    def raw(count):
        nonlocal position
        available = prefix[position:min(position + count, len(prefix))]
        remainder = count - len(available)
        result = available + (read_exact(stream, remainder) if remainder else b'')
        position += count
        return result

    def skip(count):
        while count:
            n = min(count, 1024 * 1024)
            raw(n)
            count -= n

    files = {}
    sectors = (size - track.get('offset', 0)) // stride
    for path, extent, length in catalogue:
        if base <= extent < base + sectors:
            files.setdefault((extent, length), []).append(path)
    for (extent, length), paths in sorted(files.items()):
        offset = track.get('offset', 0) + (extent - base) * stride
        if offset < position:
            raise ValueError('Overlapping ISO file extents')
        skip(offset - position)
        if not length:
            continue
        head = raw(stride)
        first = payload(head, stride)
        is_event = any(p.suffix.upper() in ('.EV1', '.EV2', '.EV3') for p in paths)
        if is_event or first[:4] in CONTAINERS or first[:2] == b'\x1f\x8b':
            rest = raw(((length + 2047) // 2048 - 1) * stride)
            data = payload(head + rest, stride)[:length]
            for path in paths:
                events.file(lambda at, n: data[at:at + n], length, path)
    skip(size - position)
    return catalogue


def extract_zip(source, events):
    with zipfile.ZipFile(source) as archive:
        gdis = [name for name in archive.namelist() if name.lower().endswith('.gdi')]
        if len(gdis) != 1:
            raise ValueError('Expected one GDI in the ZIP archive')
        lines = archive.read(gdis[0]).decode('utf-8-sig').splitlines()
        tracks = []
        count = 0
        for line in lines[1:]:
            fields = shlex.split(line)
            if not fields:
                continue
            if len(fields) != 6:
                raise ValueError('Invalid GDI track entry')
            count += 1
            if int(fields[2]) != 4 or int(fields[1]) < 45000:
                continue
            matches = [name for name in archive.namelist() if Path(name).name == fields[4]]
            if len(matches) != 1:
                raise ValueError('Missing or ambiguous archive data track')
            member = matches[0]
            tracks.append({'member': member, 'lba': int(fields[1]), 'stride': int(fields[3]),
                           'offset': int(fields[5]), 'size': archive.getinfo(member).file_size})
        if count != int(lines[0]) or not tracks:
            raise ValueError('Invalid GDI track count or no high-density data tracks')
        catalogue = None
        for track in sorted(tracks, key=lambda row: row['lba']):
            with archive.open(track['member']) as stream:
                catalogue = extract_track(stream, track, events, catalogue)


def extract_7z(source, events):
    listing = subprocess.run(['7z', 'l', '-slt', str(source)], check=True, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, text=True).stdout
    tracks = []
    for block in listing.split('\n\n'):
        fields = dict(line.split(' = ', 1) for line in block.splitlines() if ' = ' in line)
        name = fields.get('Path', '')
        if name.lower().endswith('.bin') and '_track' in name:
            number = int(name.rsplit('_track', 1)[1].split('.')[0])
            if number < 3:
                continue
            if fields.get('Encrypted') == '+':
                raise ValueError('Encrypted disc archives are unsupported')
            tracks.append({'member': name, 'stride': 2352, 'size': int(fields['Size'])})
    if not tracks:
        raise ValueError('No high-density BIN tracks in 7z archive')
    catalogue = None
    for track in sorted(tracks, key=lambda row: row['member']):
        process = subprocess.Popen(['7z', 'x', '-so', '-bsp0', str(source), track['member']],
                                   stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        try:
            catalogue = extract_track(process.stdout, track, events, catalogue)
            while process.stdout.read(1024 * 1024):
                pass
            if process.wait() != 0:
                raise ValueError('7z data-track extraction failed')
        finally:
            process.stdout.close()
            if process.poll() is None:
                process.terminate()
            process.wait()


def extract(source, destination):
    source, destination = source.resolve(), destination.resolve()
    if source == destination or source in destination.parents:
        raise ValueError('Extraction output must be outside its input')
    events = Events(destination)
    if source.is_dir():
        extract_directory(source, events)
    elif source.suffix.lower() == '.zip':
        extract_zip(source, events)
    elif source.suffix.lower() == '.7z':
        extract_7z(source, events)
    else:
        extract_gdi(source, events)
    if not events.entries:
        raise ValueError('No EV overlays found')
    manifest = {'source': str(source), 'packages': events.packages,
                'total': len(events.entries), 'distinct': len(set(events.paths.values())),
                'events': sorted(events.entries, key=lambda row: row['path'])}
    write(destination / 'event-manifest.json', (json.dumps(manifest, indent=2) + '\n').encode())
    print(f'[event] extracted {manifest["total"]} EV files ({manifest["distinct"]} distinct)', flush=True)
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('destination', type=Path)
    args = parser.parse_args()
    try:
        extract(args.source, args.destination)
    except (OSError, ValueError, StopIteration, struct.error, subprocess.CalledProcessError, zipfile.BadZipFile) as error:
        parser.exit(1, f'[event] extraction failed: {error}\n')
