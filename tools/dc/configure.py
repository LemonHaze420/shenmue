#!/usr/bin/env python3
"""Generate the Dreamcast source-object link from its build configuration."""

import argparse
import json
import os
from fnmatch import fnmatchcase
from pathlib import Path
import shlex
import sys


def elf_section_names(path):
    """Section names of a 32-bit little-endian ELF object."""
    import struct
    data = Path(path).read_bytes()
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    headers = [struct.unpack_from("<IIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    strtab_off = headers[shstrndx][4]
    return [data[strtab_off + h[0]:data.index(b"\0", strtab_off + h[0])].decode() for h in headers]


def write(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    if isinstance(data, str):
        data = data.encode()
    if not path.exists() or path.read_bytes() != data:
        path.write_bytes(data)


def members(path):
    raw = path.read_bytes()
    if raw[:8] != b"!<arch>\n":
        raise ValueError(f"Expected a GNU archive: {path}")
    offset, names = 8, b""
    while offset < len(raw):
        header = raw[offset:offset + 60]
        if len(header) != 60 or header[58:] != b"`\n":
            raise ValueError(f"Invalid archive member in {path}")
        size = int(header[48:58])
        contents = raw[offset + 60:offset + 60 + size]
        if len(contents) != size:
            raise ValueError(f"Truncated archive member in {path}")
        offset += 60 + size + (size & 1)
        name = header[:16].decode().strip()
        if name == "//":
            names = contents
        elif name != "/":
            if name.startswith("/") and name[1:].isdigit():
                name = names[int(name[1:]):].split(b"/\n", 1)[0].decode()
            else:
                name = name.rstrip("/")
            yield name, contents


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--build-dir", type=Path, required=True)
    parser.add_argument("--sdk-dir", type=Path, required=True)
    parser.add_argument("--syscalls", type=Path, required=True)
    parser.add_argument("--library", type=Path, action="append", default=[])
    parser.add_argument("--splat-root", type=Path)
    args = parser.parse_args()
    if args.splat_root:
        sys.path.insert(0, str(args.splat_root.resolve() / "src"))
    try:
        from splat import __version__
        from splat.scripts.split import initialize_segments, write_linker_script
        from splat.util import conf
        import yaml
    except ImportError as error:
        parser.exit(2, f"Install tools/dc/requirements.txt: {error}\n")
    try:
        if tuple(int(part) for part in __version__.split(".")) < (0, 50, 1):
            raise ValueError("Install splat64 0.50.1 from tools/dc/requirements.txt")
        config = yaml.safe_load(args.config.read_text())
        build = args.build_dir.resolve()
        config_dir = args.config.resolve().parent
        source_objects = {}
        for entry in json.loads((build / "compile_commands.json").read_text()):
            command = entry.get("arguments") or shlex.split(entry["command"])
            obj = Path(entry["directory"]) / command[command.index("-o") + 1]
            if any(target in obj.as_posix() for target in ("CMakeFiles/shenmue2_dc.dir/", "CMakeFiles/shenmue2_dc_core.dir/")):
                source = "src/" + entry["file"].split("/src/", 1)[1]
                if source in source_objects:
                    raise ValueError(f"Ambiguous source object: {source}")
                source_objects[source] = obj
        archives = {path.name: path for path in args.sdk_dir.glob("*.a")}
        archives.update((path.name, path) for path in args.library)
        archive_contents, objects = {}, []
        order = [line for line in (config_dir / config["objects"]).read_text().splitlines() if line and not line.startswith("#")]
        if len(order) != len(set(order)):
            raise ValueError("Duplicate source object in input order")
        missing = set(source_objects) - set(order)
        if missing:
            raise ValueError("Source objects missing from configuration: " + ", ".join(sorted(missing)))
        for name in order:
            keep = False
            if name.startswith("src/"):
                path, keep = source_objects[name], True
            elif name.startswith("lib/"):
                _, library, member = name.split("/", 2)
                if Path(member).name != member or Path(library).name != library:
                    raise ValueError(f"Invalid archive object: {name}")
                if library not in archive_contents:
                    archive_contents[library] = dict(members(archives[library]))
                path = build / "matching" / "objects" / library / member
                write(path, archive_contents[library][member])
            elif name == "data/debug_screen_font":
                path, keep = build / "resources/debug_screen_font.obj", True
            elif name == "external/syscalls":
                path = args.syscalls.resolve()
            else:
                raise ValueError(f"Unknown source object: {name}")
            filename = str(path) if name == "external/syscalls" else os.path.relpath(path, build)
            objects.append((name, filename, keep))
        function_before, function_sections = None, []
        if config.get("function_order"):
            lines = [line for line in (config_dir / config["function_order"]).read_text().splitlines() if line]
            if lines and lines[0].startswith("before "):
                function_before, function_sections = lines[0][len("before "):], lines[1:]
        data_sections = {}
        object_paths = {name: path for name, path, _ in objects}
        if config.get("data_order"):
            for line in (config_dir / config["data_order"]).read_text().splitlines():
                if "\t" in line:
                    columns = line.split("\t")
                    if len(columns) not in (2, 3):
                        raise ValueError(f"Invalid data order: {line}")
                    group_name, section = columns[:2]
                    owner = columns[2] if len(columns) == 3 else "*"
                    if owner != "*" and owner not in object_paths:
                        raise ValueError(f"Unknown data owner: {owner}")
                    data_sections.setdefault(group_name, []).append((section, owner))
        constructors = (config_dir / config["constructors"]).read_text().splitlines()
        ranks = {name: index for index, name in enumerate(constructors)}
        if set(constructors) - set(order):
            raise ValueError("Constructor order names an unknown source object")
        patterns = [
            ("text", ".text .text.* .gnu.linkonce.t.* PSG PSG32"),
            ("rodata", ".rodata .rodata.* .gnu.linkonce.r.* CSG CSG32 C32"),
            ("data", ".data .data.* .gnu.linkonce.d.* DSG DSG32"),
            ("preinit_array", ".preinit_array .preinit_array.*"),
            ("init_array", ".init_array .init_array.*"),
            ("fini_array", ".fini_array .fini_array.*"),
            ("ctors", ".ctors .ctors.*"),
            ("bss", ".bss .bss.* .gnu.linkonce.b.* BSG BSG32 COMMON"),
        ]
        options = dict(config["options"])
        options.update(base_path=str(build), build_path=".", target_path=str(build / "1ST_READ.BIN"),
                       extensions_path=str(Path(__file__).resolve().parent / "splat_ext"),
                       ld_script_path=str(build / "s2-sections.ld"), section_order=["." + name for name, _ in patterns])
        segments = []
        for index, (name, pattern) in enumerate(patterns):
            group = {"name": name, "type": "code", "start": options["ld_rom_start"] - config["reference"]["base"] if not index else "auto", "align": 32,
                     "section_order": ["." + name], "subsegments": []}
            if not index:
                group["vram"] = options["ld_rom_start"]
            else:
                group["follows_vram"] = patterns[index - 1][0]
            ordered = sorted(objects, key=lambda obj: ranks.get(obj[0], len(ranks))) if name == "ctors" else objects
            # globals placed in .bss by data_order must not be claimed by an object's .data wildcard first
            bss_patterns = [section for section, _ in data_sections.get("bss", []) if section.startswith(".data.")]
            for number, (object_name, path, keep) in enumerate(ordered):
                if name == "data" and bss_patterns and object_name.startswith("src/"):
                    names = elf_section_names(build / path)
                    kept = [n for n in names if n.startswith(".data.") and not any(fnmatchcase(n, p) for p in bss_patterns)]
                    group["subsegments"].append({"start": "auto", "type": "object", "name": f"{name}_{number}", "object": path,
                                                 "section": " ".join([".data"] + kept + [".gnu.linkonce.d.*", "DSG", "DSG32"]),
                                                 "linker_section_order": "." + name, "keep": keep})
                    continue
                # GameJam function order: each mapped function's section, from whichever object holds it
                if name == "text" and object_name == function_before:
                    for fn_index, section in enumerate(function_sections):
                        group["subsegments"].append({"start": "auto", "type": "object", "name": f"text_fn_{fn_index}", "object": "*",
                                                     "section": section, "linker_section_order": ".text", "keep": True})
                # GameJam data order: each paired global's section, grouped by our output section
                if name in data_sections and object_name == function_before:
                    for data_index, (section, owner) in enumerate(data_sections[name]):
                        group["subsegments"].append({"start": "auto", "type": "object", "name": f"{name}_data_{data_index}",
                                                     "object": "*" if owner == "*" else object_paths[owner],
                                                     "section": section, "linker_section_order": "." + name, "keep": True})
                group["subsegments"].append({"start": "auto", "type": "object", "name": f"{name}_{number}",
                                             "object": path, "section": pattern, "linker_section_order": "." + name, "keep": keep})
            # every object is listed above; with .bss placement active the catch-all must leave .data.<name> sections alone
            unlisted = ".data .gnu.linkonce.d.* DSG DSG32" if name == "data" and bss_patterns else pattern
            group["subsegments"].append({"start": "auto", "type": "object", "name": name + "_unlisted", "object": "*",
                                         "section": unlisted, "linker_section_order": "." + name})
            segments.append(group)
        segments.append([config["reference"]["end"] - config["reference"]["base"]])
        resolved = build / "matching" / "splat.yml"
        write(resolved, yaml.safe_dump({"options": options, "segments": segments}, sort_keys=False))
        os.chdir(build)
        loaded = conf.load([resolved], modes=["ld"])
        write_linker_script(initialize_segments(loaded["segments"]))
        # .bss now also receives all-zero .data.<name> sections; it still occupies no file space
        script = build / "s2-sections.ld"
        text = script.read_text()
        import re as _re
        script.write_text(_re.sub(r"(?m)^(\s*\.bss\s+\S+)\s*:\s*AT\(", r"\1 (NOLOAD) : AT(", text, count=1))
        (build / "s2-sections.ld").touch()
        print(f"[link] {len(objects)} input objects; {build / 's2-sections.ld'}")
    except (OSError, ValueError, KeyError) as error:
        parser.exit(2, f"[link] {error}\n")


if __name__ == "__main__":
    main()
