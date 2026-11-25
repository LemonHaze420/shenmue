#!/usr/bin/env python3
"""Compare a complete Dreamcast executable and prepare its objdiff project."""

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess

from build_disc import find_boot_file, payload_sector, read_tracks


ELF_HEADER = struct.Struct("<16sHHIIIIIHHHHHH")
SECTION = struct.Struct("<10I")
SYMBOL = struct.Struct("<IIIBBH")


def write(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    if isinstance(data, str):
        data = data.encode()
    if not path.exists() or path.read_bytes() != data:
        path.write_bytes(data)


class Elf:
    def __init__(self, data):
        self.data = data
        self.header = ELF_HEADER.unpack_from(data)
        header = self.header
        if header[0][:7] != b"\x7fELF\x01\x01\x01" or header[2] != 42 or header[11] != SECTION.size:
            raise ValueError("Expected a little-endian SuperH ELF32 image")
        if not header[12] or header[13] >= header[12]:
            raise ValueError("Invalid ELF section table")
        self.sections = [SECTION.unpack_from(data, header[6] + index * SECTION.size) for index in range(header[12])]
        strings = self.contents(self.sections[header[13]])
        self.names = [self.string(strings, section[0]) for section in self.sections]

    @staticmethod
    def string(strings, offset):
        if offset >= len(strings):
            raise ValueError("Invalid ELF string offset")
        end = strings.find(b"\0", offset)
        if end == -1:
            raise ValueError("Unterminated ELF string")
        return strings[offset:end].decode()

    def contents(self, section):
        data = self.data[section[4]:section[4] + section[5]]
        if len(data) != section[5]:
            raise ValueError("Truncated ELF section")
        return data

    def symbols(self):
        for section in self.sections:
            if section[1] != 2:
                continue
            if section[9] != SYMBOL.size or section[5] % SYMBOL.size:
                raise ValueError("Invalid ELF symbol table")
            strings = self.contents(self.sections[section[6]])
            for offset in range(section[4], section[4] + section[5], SYMBOL.size):
                symbol = SYMBOL.unpack_from(self.data, offset)
                yield offset, self.string(strings, symbol[0]), symbol

    def function_view(self):
        sections = [("", 0, 0, 0, b"", 0, 0, 0, 0)]
        indices = {}
        strings, symbols = bytearray(b"\0"), bytearray(SYMBOL.size)
        for index, (name, section) in enumerate(zip(self.names, self.sections)):
            if not section[2] & 4:
                continue
            indices[index] = len(sections)
            sections.append((name, 1, 6, 0, self.contents(section), 0, 0, section[8], 0))
        for index in range(1, len(sections)):
            symbols.extend(SYMBOL.pack(0, 0, 0, 3, 0, index))
        local_count = len(sections)
        normalized = {}
        for offset, name, symbol in self.symbols():
            if symbol[5] not in indices or symbol[3] & 15 != 2:
                continue
            section = self.sections[symbol[5]]
            value = symbol[1] - section[3]
            if value < 0 or value > section[5] or symbol[2] > section[5] - value:
                raise ValueError(f"Symbol is outside its ELF section: {name}")
            key = function_key(name)
            if key in normalized and normalized[key] != name:
                raise ValueError(f"Ambiguous function name: {key}")
            normalized[key] = name
            name_offset = len(strings)
            strings.extend(key.encode() + b"\0")
            symbols.extend(SYMBOL.pack(name_offset, value, symbol[2], 0x12, 0, indices[symbol[5]]))
        string_index = len(sections)
        sections.append((".strtab", 3, 0, 0, bytes(strings), 0, 0, 1, 0))
        sections.append((".symtab", 2, 0, 0, bytes(symbols), string_index, local_count, 4, SYMBOL.size))
        return make_elf(sections, self.header[7])


def make_elf(sections, flags):
    section_strings, offsets = bytearray(b"\0"), []
    for section in sections:
        offsets.append(len(section_strings))
        section_strings.extend(section[0].encode() + b"\0")
    offsets.append(len(section_strings))
    section_strings.extend(b".shstrtab\0")
    sections.append((".shstrtab", 3, 0, 0, bytes(section_strings), 0, 0, 1, 0))
    image, headers = bytearray(ELF_HEADER.size), []
    for name_offset, section in zip(offsets, sections):
        _, kind, section_flags, address, contents, link, info, align, entry_size = section
        if kind == 0:
            headers.append((0,) * 10)
            continue
        image.extend(b"\0" * (-len(image) % align))
        offset = len(image)
        size = contents if kind == 8 else len(contents)
        if kind != 8:
            image.extend(contents)
        headers.append((name_offset, kind, section_flags, address, offset, size, link, info, align, entry_size))
    image.extend(b"\0" * (-len(image) % 4))
    section_offset = len(image)
    for header in headers:
        image.extend(SECTION.pack(*header))
    image[:ELF_HEADER.size] = ELF_HEADER.pack(b"\x7fELF\x01\x01\x01" + b"\0" * 9, 1, 42, 1, 0, 0,
                                            section_offset, flags, ELF_HEADER.size, 0, 0, SECTION.size, len(headers), len(headers) - 1)
    return image


def reference_bytes(path, config):
    if path.suffix.lower() == ".gdi":
        tracks = read_tracks(path)
        name, extent, size, _, _ = find_boot_file(tracks)
        if name.upper() != "1ST_READ.BIN":
            raise ValueError(f"Unexpected boot file: {name}")
        data = b"".join(payload_sector(tracks, extent + index) for index in range((size + 2047) // 2048))[:size]
    else:
        data = path.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if digest != config["sha256"]:
        raise ValueError(f"Reference SHA256 mismatch: expected {config['sha256']}, got {digest}")
    if len(data) != config["end"] - config["base"]:
        raise ValueError("Reference extent differs from its configuration")
    return data


def reference_object(data, config, symbol_path, flags):
    strings = bytearray(b"\0")

    def add_string(table, value):
        offset = len(table)
        table.extend(value.encode() + b"\0")
        return offset

    sections = [("", 0, 0, 0, b"", 0, 0, 0, 0)]
    for section in config["sections"]:
        start, end = section["start"], section["end"]
        if not config["base"] <= start < end <= config["end"]:
            raise ValueError(f"Reference section is outside the image: {section['name']}")
        size = end - start
        contents = size if section["kind"] == "bss" else data[start - config["base"]:end - config["base"]]
        sections.append((section["name"], 8 if section["kind"] == "bss" else 1,
                         6 if section["kind"] == "code" else 3, start, contents, 0, 0, 4, 0))
    symbols = bytearray(SYMBOL.size)
    for index, section in enumerate(sections[1:], 1):
        symbols.extend(SYMBOL.pack(0, section[3], 0, 3, 0, index))
    local_count = len(sections)
    names = set()
    previous_end = 0
    for line in symbol_path.read_text().splitlines():
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        match = re.fullmatch(r"(\S+) = (0x[0-9a-fA-F]+); // type:func size:(0x[0-9a-fA-F]+)", line)
        if not match:
            raise ValueError(f"Invalid function symbol: {line}")
        name, address, size = match[1], int(match[2], 16), int(match[3], 16)
        index = next((i for i, section in enumerate(config["sections"], 1)
                      if section["kind"] == "code" and section["start"] <= address < address + size <= section["end"]), None)
        if index is None or address < previous_end or name in names:
            raise ValueError(f"Overlapping, duplicate or out-of-range function: {name}")
        names.add(name)
        previous_end = address + size
        symbols.extend(SYMBOL.pack(add_string(strings, name), address, size, 0x12, 0, index))
    string_index = len(sections)
    sections.append((".strtab", 3, 0, 0, bytes(strings), 0, 0, 1, 0))
    sections.append((".symtab", 2, 0, 0, bytes(symbols), string_index, local_count, 4, SYMBOL.size))
    return Elf(make_elf(sections, flags)).function_view(), names


def literal_mask(blob, address):
    mask = set()
    for offset in range(0, len(blob) - 1, 2):
        op = struct.unpack_from("<H", blob, offset)[0]
        pc = address + offset
        if op >> 12 == 0xD:
            target = (pc & ~3) + 4 + (op & 0xff) * 4 - address
            mask.update(range(target, target + 4))
        elif op >> 12 == 0x9:
            target = pc + 4 + (op & 0xff) * 2 - address
            mask.update((target, target + 1))
    return mask


def function_key(name):
    replacements = {}
    pattern = r"_GLOBAL_\.N\.[A-Za-z0-9_.]*?(src_(?:s1|s2|common|platform)_[A-Za-z0-9_]+\.cpp)[A-Za-z0-9_]{6}"
    for qualifier in re.finditer(r"Q([1-9])", name):
        position, components = qualifier.end(), []
        for _ in range(int(qualifier[1])):
            length = re.match(r"[1-9][0-9]*", name[position:])
            if not length:
                break
            start = position + len(length[0])
            end = start + int(length[0])
            if end > len(name):
                break
            components.append((position, start, end))
            position = end
        else:
            for position, start, end in components:
                owner = re.fullmatch(pattern, name[start:end])
                if owner:
                    namespace = "_GLOBAL_.N." + owner[1] + "stable"
                    replacements[position] = (end, str(len(namespace)) + namespace)
    result, end = [], 0
    for position, (stop, replacement) in sorted(replacements.items()):
        result.extend((name[end:position], replacement))
        end = stop
    return "".join(result) + name[end:]


def compare_functions(reference, candidate, base, symbol_path, elf, lo=0, hi=1 << 32):
    starts, sized = {}, {}
    for _, name, symbol in elf.symbols():
        if symbol[3] & 15 == 2 and symbol[5] < len(elf.sections) and elf.sections[symbol[5]][2] & 4:
            starts.setdefault(symbol[5], set()).add(symbol[1])
            sized.setdefault(name, (symbol[1], symbol[2], symbol[5]))
    ordered = {index: sorted(values) for index, values in starts.items()}
    ours = {}
    for name, (address, size, index) in sized.items():
        if not size:
            section = elf.sections[index]
            later = [value for value in ordered[index] if value > address]
            size = (later[0] if later else section[3] + section[5]) - address
        ours[name] = (address, size)
    pad = (b"\x09\x00", b"\x00\x00")

    def trim(blob):
        while len(blob) >= 2 and blob[-2:] in pad:
            blob = blob[:-2]
        return blob

    aliases = {}
    for name in ours:
        aliases.setdefault(function_key(name), []).append(name)

    counts = {"exact": 0, "instruction_equivalent": 0, "different": 0, "unpaired": 0}
    for line in symbol_path.read_text().splitlines():
        match = re.fullmatch(r"(\S+) = (0x[0-9a-fA-F]+); // type:func size:(0x[0-9a-fA-F]+)", line.strip())
        if not match or not lo <= int(match[2], 16) < hi:
            continue
        name, address, size = match[1], int(match[2], 16), int(match[3], 16)
        candidate_name = name
        if candidate_name not in ours:
            matches = aliases.get(function_key(name), [])
            if len(matches) != 1:
                counts["unpaired"] += 1
                continue
            candidate_name = matches[0]
        at, our_size = ours[candidate_name]
        expected = trim(reference[address - base:address - base + size])
        actual = trim(candidate[at - base:at - base + our_size])
        if expected == actual:
            counts["exact"] += 1
            continue
        mask = literal_mask(expected, address) | literal_mask(actual, at)
        same = len(expected) == len(actual) and all(expected[k:k + 2] == actual[k:k + 2] for k in range(0, len(expected) - 1, 2) if k not in mask)
        counts["instruction_equivalent" if same else "different"] += 1
    total = sum(counts.values())
    counts.update(total=total, exact_percent=round(100 * counts["exact"] / total, 2) if total else 0.0,
                  instruction_equivalent_percent=round(100 * (counts["exact"] + counts["instruction_equivalent"]) / total, 2) if total else 0.0,
                  instruction_comparison="Excludes PC-relative literal data; referenced addresses and constant values are not verified")
    return counts


def check_source_image(elf, candidate, config):
    sections = sorted((section for section in elf.sections if section[2] & 2 and section[1] != 8 and section[5]), key=lambda s: s[3])
    if not sections or sections[0][3] != config["base"]:
        raise ValueError("Source executable starts outside its configured image")
    image = bytearray(max(config["end"], max(s[3] + s[5] for s in sections)) - config["base"])
    end = config["base"]
    for section in sections:
        if section[3] < end:
            raise ValueError("Source executable has overlapping loadable sections")
        offset = section[3] - config["base"]
        image[offset:offset + section[5]] = elf.contents(section)
        end = section[3] + section[5]
    if candidate != image:
        raise ValueError("Candidate is not the source executable with zero-filled image gaps")


def compare(reference, candidate, base):
    extent = max(len(reference), len(candidate))
    equal = sum(a == b for a, b in zip(reference, candidate))
    differences = []
    for offset in range(extent):
        expected = reference[offset] if offset < len(reference) else None
        actual = candidate[offset] if offset < len(candidate) else None
        if expected != actual:
            differences.append({"address": f"0x{base + offset:08x}", "expected": expected, "actual": actual})
            if len(differences) == 16:
                break
    return {"expected_size": len(reference), "actual_size": len(candidate), "equal_bytes": equal,
            "different_bytes": extent - equal, "masked_bytes": 0, "match": reference == candidate, "first_differences": differences}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--reference", type=Path, required=True)
    parser.add_argument("--candidate", type=Path)
    parser.add_argument("--elf", type=Path)
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--objdiff", type=Path)
    parser.add_argument("--summary-only", action="store_true", help="Compare bytes and functions without running objdiff")
    parser.add_argument("--prepare-reference", action="store_true", help="Extract build resources from a verified reference")
    args = parser.parse_args()
    try:
        import yaml
        config = yaml.safe_load(args.config.read_text())
        native = config["reference"]
        output = args.output_dir.resolve()
        if args.prepare_reference:
            reference = reference_bytes(args.reference, native)
            font = native["debug_font"]
            start = font["address"] - native["base"]
            end = start + font["size"]
            if start < 0 or end > len(reference):
                raise ValueError("Debug font is outside the reference image")
            contents = reference[start:end]
            if hashlib.sha256(contents).hexdigest() != font["sha256"]:
                raise ValueError("Debug font SHA256 mismatch")
            destination = output / "debug_screen_font.bin"
            if destination.resolve() in (args.reference.resolve(), args.config.resolve()):
                raise ValueError("Resource output must not replace its input files")
            write(destination, contents)
            return 0
        if not args.candidate or not args.elf or (not args.objdiff and not args.summary_only):
            raise ValueError("Comparison requires --candidate, --elf and either --objdiff or --summary-only")
        generated = [output / name for name in ("reference/1ST_READ.BIN", "expected.o", "current.o", "objdiff.json", "report.json", "image.json")]
        inputs = {p.resolve() for p in (args.config, args.reference, args.candidate, args.elf, args.config.parent / config["symbols"])}
        if inputs.intersection(p.resolve() for p in generated):
            raise ValueError("Matching output must not replace its input files")
        reference = reference_bytes(args.reference, native)
        candidate = args.candidate.read_bytes()
        elf = Elf(args.elf.read_bytes())
        if elf.header[1] != 2:
            raise ValueError("Expected a linked source executable")
        check_source_image(elf, candidate, native)
        expected, names = reference_object(reference, native, args.config.parent / config["symbols"], elf.header[7])
        write(output / "reference/1ST_READ.BIN", reference)
        write(output / "expected.o", expected)
        write(output / "current.o", elf.function_view())
        write(output / "objdiff.json", json.dumps({"build_base": False, "build_target": False, "units": [
            {"name": args.config.parent.name + "/1ST_READ", "target_path": "expected.o", "base_path": "current.o"}]}, indent=2) + "\n")
        result = compare(reference, candidate, native["base"])
        result.update(reference_sha256=hashlib.sha256(reference).hexdigest(), candidate_sha256=hashlib.sha256(candidate).hexdigest(),
                      native_functions=len(names),
                      paired_function_names=len({function_key(name) for name in names}.intersection(
                          function_key(name) for _, name, _ in elf.symbols())),
                      objdiff_scope="functions; all image bytes are checked in image.json")
        symbols = args.config.parent / config["symbols"]
        result["functions"] = compare_functions(reference, candidate, native["base"], symbols, elf)
        if "game" in native:
            result["game_functions"] = compare_functions(reference, candidate, native["base"], symbols, elf, native["game"]["start"], native["game"]["end"])
        result["sections"] = []
        for section in native["sections"]:
            start, end = section["start"] - native["base"], section["end"] - native["base"]
            result["sections"].append(dict(name=section["name"], **compare(reference[start:end], candidate[start:end], section["start"])))
        result["source_sections"] = [{"name": name, "start": f"0x{section[3]:08x}", "size": section[5], "bss": section[1] == 8}
                                    for name, section in zip(elf.names, elf.sections) if section[2] & 2]
        write(output / "image.json", json.dumps(result, indent=2) + "\n")
        state = "MATCH" if result["match"] else "MISMATCH"
        for label in ("functions", "game_functions"):
            if label in result:
                functions = result[label]
                print(f"[match] {label.replace('_', ' ')}: {functions['exact']}/{functions['total']} byte-exact "
                      f"({functions['exact_percent']}%); {functions['exact'] + functions['instruction_equivalent']}/{functions['total']} "
                      f"instruction-equivalent excluding literal data ({functions['instruction_equivalent_percent']}%); "
                      f"{functions['unpaired']} unpaired", flush=True)
        print(f"[match] 1ST_READ.BIN: {state}; {result['equal_bytes']}/{max(len(reference), len(candidate))} identical bytes; no masks", flush=True)
        print(f"[match] comparing {len(names)} native functions", flush=True)
        if not args.summary_only:
            subprocess.run([str(args.objdiff.resolve()), "report", "generate", "-p", str(output), "-o", str(output / "report.json"),
                            "-f", "json-pretty"], check=True)
        print(f"[match] {result['paired_function_names']}/{len(names)} native function names paired; objdiff project: {output}")
        print(f"[match] whole-image comparison: {output / 'image.json'}")
        if not args.summary_only:
            print(f"[match] function report: {output / 'report.json'}")
        return 0 if result["match"] else 1
    except (ImportError, OSError, ValueError, KeyError, struct.error, subprocess.CalledProcessError) as error:
        parser.exit(2, f"[match] {error}\n")


if __name__ == "__main__":
    raise SystemExit(main())
