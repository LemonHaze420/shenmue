from pathlib import Path

from splat.segtypes.common.o import CommonSegO
from splat.segtypes.linker_entry import LinkerEntry


class ObjectEntry(LinkerEntry):
    def emit_entry(self, writer):
        path = self.object_path.as_posix()
        if path != "*":
            path = '"' + path.replace('"', '\\"') + '"'
        expression = f"{path}({self.section_link})"
        if self.segment.yaml.get("keep", False):
            expression = f"KEEP({expression})"
        writer._writeln(expression + ";")


class SH4SegObject(CommonSegO):
    def get_linker_entries(self):
        entry = ObjectEntry(self, [], Path(self.yaml["object"]), self.get_linker_section_order(),
                            self.get_linker_section_linksection(), False)
        entry.object_path = Path(self.yaml["object"])
        return [entry]
