#!/usr/bin/env python3
"""Extract EV overlays from a GDI into a local build cache."""

import argparse
from pathlib import Path
import struct

from build_disc import payload_sector, read_tracks


def extract(gdi, destination):
    tracks = read_tracks(gdi)
    session = next(track for track in tracks if track.kind == 4 and track.lba >= 45000)
    volume = payload_sector(tracks, session.lba + 16)
    if volume[:7] != b'\x01CD001\x01':
        raise ValueError('Missing ISO9660 primary volume descriptor')

    def read(extent, size):
        return b''.join(payload_sector(tracks, extent + n)
                        for n in range((size + 2047) // 2048))[:size]

    visited = set()
    count = 0

    def directory(extent, size, path):
        nonlocal count
        if (extent, size) in visited:
            raise ValueError('Repeated ISO9660 directory extent')
        visited.add((extent, size))
        data = read(extent, size)
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
            name = name.decode('ascii').split(';')[0]
            if name in ('', '.', '..') or '/' in name or '\\' in name:
                raise ValueError('Invalid ISO9660 filename')
            child = path / name
            position, length = struct.unpack_from('<I', record, 2)[0], struct.unpack_from('<I', record, 10)[0]
            if record[25] & 2:
                directory(position, length, child)
            elif child.suffix.upper() in ('.EV1', '.EV2', '.EV3'):
                if record[25] & 0x80:
                    raise ValueError('Multi-extent EV files are unsupported')
                output = destination / child
                contents = read(position, length)
                if not output.is_file() or output.read_bytes() != contents:
                    output.parent.mkdir(parents=True, exist_ok=True)
                    output.write_bytes(contents)
                count += 1

    directory(struct.unpack_from('<I', volume, 158)[0], struct.unpack_from('<I', volume, 166)[0], Path())
    if not count:
        raise ValueError('No EV overlays found')
    print(f'[event] extracted {count} EV files', flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('gdi', type=Path)
    parser.add_argument('destination', type=Path)
    args = parser.parse_args()
    extract(args.gdi.resolve(), args.destination.resolve())
