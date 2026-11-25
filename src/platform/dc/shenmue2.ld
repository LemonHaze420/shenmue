OUTPUT_FORMAT("elf32-shl")
OUTPUT_ARCH(sh)
ENTRY(_dc_disc_start)
PROVIDE(__dc_image_limit = 0x0c328000);
__dc_stack_top = 0x0c039000;

SECTIONS
{
    . = 0x0c010000;
    .disc_boot : { KEEP(*(.text.dc_disc_start)) }
    . = 0x0c020000;
    .boot :
    {
        KEEP(*(.text.dc_start))
        . = 0x100;
        KEEP(*(.rodata.dc_take))
        . = 0x114;
        __dc_loader_word_begin = .;
        KEEP(*(.data.dc_loader_flag))
        __dc_loader_word_end = .;
        . = 0x118;
        __dc_loaded_act_begin = .;
        KEEP(*(.data.dc_loaded_act))
        __dc_loaded_act_end = .;
        . = 0x120;
        __dc_file_directory_begin = .;
        KEEP(*(.data.dc_file_directory))
        . = 0x140;
        KEEP(*(.data.dc_file_name))
        . = 0x150;
        KEEP(*(.data.dc_default_vblank))
        KEEP(*(.data.dc_frame_vblank))
        KEEP(*(.data.dc_frame_counter))
        KEEP(*(.data.dc_vblank_counter))
        KEEP(*(.data.dc_loading_flags))
        KEEP(*(.data.dc_scene_act))
        KEEP(*(.data.dc_scene_area))
        KEEP(*(.data.dc_scene_entry))
        . = 0x170;
        KEEP(*(.data.dc_frame_unknown170))
        KEEP(*(.data.dc_frame_microseconds))
        . = 0x178;
        __dc_story_step_begin = .;
        KEEP(*(.data.dc_story_step))
        __dc_story_step_end = .;
        . = 0x180;
        KEEP(*(.data.dc_event_task_flags1))
        . = 0x190;
        KEEP(*(.data.dc_event_task_flags2))
        . = 0x1a0;
        KEEP(*(.data.dc_event_task_flags3))
    }
    . = 0x0c020200;
    .event_functions : { KEEP(*(.data.dc_event_functions)) }
    __dc_stack_bottom = .;
}
INCLUDE s2-sections.ld
SECTIONS
{
    __dc_text_begin = ABSOLUTE(ADDR(.text));
    __dc_text_contents_end = ABSOLUTE(ADDR(.text) + SIZEOF(.text));
    __dc_initialized_begin = ABSOLUTE(ADDR(.rodata));
    __dc_initialized_contents_end = ABSOLUTE(ADDR(.ctors) + SIZEOF(.ctors));
    __preinit_array_start = ABSOLUTE(ADDR(.preinit_array));
    __preinit_array_end = ABSOLUTE(ADDR(.preinit_array) + SIZEOF(.preinit_array));
    __init_array_start = ABSOLUTE(ADDR(.init_array));
    __init_array_end = ABSOLUTE(ADDR(.init_array) + SIZEOF(.init_array));
    __fini_array_start = ABSOLUTE(ADDR(.fini_array));
    __fini_array_end = ABSOLUTE(ADDR(.fini_array) + SIZEOF(.fini_array));
    __dc_ctors_begin = ABSOLUTE(ADDR(.ctors));
    __dc_ctors_end = ABSOLUTE(ADDR(.ctors) + SIZEOF(.ctors));
    __dc_bss_begin = ABSOLUTE(ADDR(.bss));
    __dc_bss_contents_end = ABSOLUTE(ADDR(.bss) + SIZEOF(.bss));
    __dc_bss_end = __dc_bss_contents_end;
    __dc_bss_size = __dc_bss_end - __dc_bss_begin;
    __dc_crt_heap_begin = ALIGN(__dc_bss_end, 32);
    __dc_crt_heap_end = __dc_image_limit;
    ASSERT(__dc_loader_word_begin == 0x0c020114 && __dc_loader_word_end == 0x0c020118, "S2 loader sentinel changed layout")
    ASSERT(__dc_loaded_act_begin == 0x0c020118 && __dc_loaded_act_end == 0x0c02011c, "S2 loaded act changed layout")
    ASSERT(__dc_loader_word_end <= __dc_loaded_act_begin && __dc_loaded_act_end <= __dc_file_directory_begin, "S2 header words overlap")
    ASSERT(__dc_story_step_begin == 0x0c020178 && __dc_story_step_end == 0x0c02017c, "S2 story step changed layout")
    ASSERT(SIZEOF(.event_functions) == 850 * 4, "S2 event function table changed layout")
    ASSERT(__dc_stack_bottom < __dc_stack_top, "S2 boot header overlaps its native stack")
    ASSERT(__dc_bss_end <= __dc_image_limit, "S2 image overlaps the native heap or validation output")
    ASSERT(__dc_crt_heap_begin < __dc_crt_heap_end, "S2 resident storage overlaps the runtime heap")
    /DISCARD/ : { *(.comment) *(.note*) }
}
