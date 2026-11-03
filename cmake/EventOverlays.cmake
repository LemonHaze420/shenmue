option(SHENMUE_DC_EVENT_OVERLAYS "Build Shenmue II Dreamcast EV1/EV2/EV3 overlays with the pinned GCC 2.95.2" OFF)
set(SHENMUE_DC_EVENT_REFERENCE_DIR "" CACHE STRING "Disc data directories (a list) whose EV files the built overlays must match byte-for-byte")
if(NOT SHENMUE_DC_EVENT_OVERLAYS)
  return()
endif()
if(SHENMUE_DC_EVENT_REFERENCE_DIR)
  enable_testing()
endif()

include(ExternalProject)
find_program(SHENMUE_EVENT_MAKE NAMES make gmake REQUIRED)
find_program(SHENMUE_EVENT_PATCH NAMES patch REQUIRED)
find_program(SHENMUE_EVENT_ICONV NAMES iconv REQUIRED)

set(SHENMUE_EVENT_HOST_CC "gcc -m32" CACHE STRING "32-bit host C compiler command used to build the pinned GCC 2.95.2")
set(_event_host_cc "${SHENMUE_EVENT_HOST_CC} -D_FILE_OFFSET_BITS=64 -std=gnu89 -fcommon -fgnu89-inline -w -O1")
file(WRITE "${CMAKE_BINARY_DIR}/event-gcc/host-check.c" "typedef char host_long_is_32_bits[sizeof(long) == 4 ? 1 : -1];\n")
separate_arguments(_event_host_check UNIX_COMMAND "${_event_host_cc}")
execute_process(COMMAND ${_event_host_check} -c host-check.c -o host-check.o WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/event-gcc"
  RESULT_VARIABLE _event_host_result ERROR_VARIABLE _event_host_error)
if(NOT _event_host_result EQUAL 0)
  message(FATAL_ERROR "SHENMUE_EVENT_HOST_CC must compile 32-bit host code (install gcc-multilib): ${_event_host_error}")
endif()
set(_event_host --host=i686-pc-linux-gnu --build=i686-pc-linux-gnu --target=sh-elf)
ExternalProject_Add(shenmue_event_gxx_source
  URL https://ftp.gnu.org/gnu/gcc/gcc-2.95.2/gcc-g++-2.95.2.tar.gz
  URL_HASH SHA256=fa3372cf8a5aca73d4be651eda01cb87811e2f0e76e469b1b8fa124615fa671b
  PREFIX "${CMAKE_BINARY_DIR}/event-gcc"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "")
ExternalProject_Get_Property(shenmue_event_gxx_source SOURCE_DIR)
set(_event_gxx_source "${SOURCE_DIR}")
ExternalProject_Add(shenmue_event_gcc
  URL https://ftp.gnu.org/gnu/gcc/gcc-2.95.2/gcc-core-2.95.2.tar.gz
  URL_HASH SHA256=65e68d39ba3f2a6b23431ee361b62fe0103b35c3ccaa818d174ace80dfc587b1
  PREFIX "${CMAKE_BINARY_DIR}/event-gcc"
  DEPENDS shenmue_event_gxx_source
  PATCH_COMMAND "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-host.patch"
    COMMAND "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-sh-backend.patch"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${_event_gxx_source}/gcc/cp" <SOURCE_DIR>/gcc/cp
    COMMAND "${CMAKE_COMMAND}" -E touch <SOURCE_DIR>/gcc/cp/parse.c <SOURCE_DIR>/gcc/cp/parse.h <SOURCE_DIR>/gcc/cp/cfns.h
  CONFIGURE_COMMAND "${CMAKE_COMMAND}" -E make_directory <BINARY_DIR>/libiberty <BINARY_DIR>/gcc/include
    COMMAND "${CMAKE_COMMAND}" -E chdir <BINARY_DIR>/libiberty "${CMAKE_COMMAND}" -E env "CC=${_event_host_cc}" sh <SOURCE_DIR>/libiberty/configure ${_event_host}
    COMMAND "${CMAKE_COMMAND}" -E chdir <BINARY_DIR>/gcc "${CMAKE_COMMAND}" -E env "CC=${_event_host_cc}" sh <SOURCE_DIR>/gcc/configure ${_event_host} --disable-nls
  BUILD_COMMAND "${SHENMUE_EVENT_MAKE}" -C <BINARY_DIR>/libiberty
    COMMAND "${SHENMUE_EVENT_MAKE}" -C <BINARY_DIR>/gcc cc1 cc1plus cccp "LANGUAGES=c c++"
    COMMAND "${CMAKE_COMMAND}" -E copy <SOURCE_DIR>/gcc/glimits.h <BINARY_DIR>/gcc/include/limits.h
    COMMAND "${CMAKE_COMMAND}" -E copy <SOURCE_DIR>/gcc/ginclude/stddef.h <SOURCE_DIR>/gcc/ginclude/stdarg.h <SOURCE_DIR>/gcc/ginclude/va-sh.h
      <BINARY_DIR>/gcc/include
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS <BINARY_DIR>/gcc/cc1 <BINARY_DIR>/gcc/cc1plus <BINARY_DIR>/gcc/cccp)
ExternalProject_Add_Step(shenmue_event_gcc math_options
  COMMAND "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-math-errno.patch"
  WORKING_DIRECTORY <SOURCE_DIR>
  DEPENDEES patch
  DEPENDERS configure
  DEPENDS "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-math-errno.patch")
ExternalProject_Get_Property(shenmue_event_gcc SOURCE_DIR BINARY_DIR)
set(_event_gcc_source "${SOURCE_DIR}")
set(_event_gcc_build "${BINARY_DIR}/gcc")

ExternalProject_Add(shenmue_event_newlib
  URL https://sourceware.org/pub/newlib/newlib-1.8.1.tar.gz
  URL_HASH SHA256=bdf73c1b691d952798af33dfa3c7ec90854d89bc6afa100e6c547ee26d7a3c42
  PREFIX "${CMAKE_BINARY_DIR}/event-newlib"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "")
ExternalProject_Get_Property(shenmue_event_newlib SOURCE_DIR)
set(_event_newlib_source "${SOURCE_DIR}/newlib/libc")

set(_event_cccp "${_event_gcc_build}/cccp")
set(_event_cc1 "${_event_gcc_build}/cc1")
set(_event_cc1plus "${_event_gcc_build}/cc1plus")
set(_event_as "${SHENMUE_DC_TOOLCHAIN_ROOT}/bin/sh4-elf-as")
set(_event_ld "${SHENMUE_DC_TOOLCHAIN_ROOT}/bin/sh4-elf-ld")
set(_event_defines -D__sh__ -D__ELF__ -D__LITTLE_ENDIAN__ -D__SH4_SINGLE_ONLY__)
set(_event_root "${PROJECT_SOURCE_DIR}/src/s2/events")
file(GLOB_RECURSE _event_headers CONFIGURE_DEPENDS "${_event_root}/*.h" "${_event_root}/*.hpp" "${_event_root}/*.inc")
list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/areas/area_loop_dc.c" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_loop_dc.h")
list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/event_queued_dc.h" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_exqt.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/areas/area_dgct.hpp" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_arbt.cpp" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_arbt.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/areas/et_tejo_dc.h" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_knmn.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/training_seqp.hpp")
file(GLOB _event_common_headers CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/s2/areas/common/*.h")
list(APPEND _event_headers ${_event_common_headers})
foreach(_event_c_header event_queued_dc event_utility_dc event_debug_dc event_library_tail_dc event_sleep_dc sk_event_dc areas/numeric_event_dc)
  list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/${_event_c_header}.h")
endforeach()

function(_shenmue_event_object out kind source)
  set(object "${CMAKE_CURRENT_BINARY_DIR}/events/${out}")
  get_filename_component(object_dir "${object}" DIRECTORY)
  set(includes -I${PROJECT_SOURCE_DIR}/src -I${_event_gcc_build}/include)
  set(language)
  if(kind STREQUAL "GAME")
    set(language -lang-c++ -D__cplusplus -DSHENMUE_EVENT_OVERLAY=1)
    set(compile "${_event_cc1plus}" -quiet -ml -m4-single-only -O1 -fno-exceptions -fno-rtti ${_event_game_options} "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "GAME_C")
    set(language -DSHENMUE_EVENT_OVERLAY=1)
    set(compile "${_event_cc1}" -quiet -ml -m4-single-only -O1 ${_event_game_options} "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "LIBC")
    list(PREPEND includes -I${_event_newlib_source}/include)
    set(compile "${_event_cc1}" -quiet -ml -m4-single-only -O2 "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "LIBGCC")
    list(PREPEND includes -I${_event_gcc_build} -I${_event_gcc_source}/gcc -I${_event_gcc_source}/gcc/config -I${_event_gcc_source}/include)
    set(compile "${_event_cc1}" -quiet -ml -m4-single-only -O2 "${object}.i" -o "${object}.s")
  else()
    set(compile "${CMAKE_COMMAND}" -E copy "${object}.i" "${object}.s")
  endif()
  set(preprocess "${_event_cccp}" ${language} ${_event_defines} ${ARGN} ${includes})
  if(kind STREQUAL "ASM")
    list(APPEND preprocess -lang-asm)
  endif()
  set(preprocess_source "${source}")
  set(preprocess_directory "${PROJECT_SOURCE_DIR}")
  if(source MATCHES "^(.*/s2/areas/([^/]+))/src/([^/]+[.]c)$")
    set(preprocess_directory "${CMAKE_MATCH_1}")
    if(CMAKE_MATCH_2 STREQUAL "common")
      set(preprocess_source "../common/src/${CMAKE_MATCH_3}")
    else()
      set(preprocess_source "./src/${CMAKE_MATCH_3}")
    endif()
  endif()
  add_custom_command(OUTPUT "${object}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${object_dir}"
    COMMAND "${CMAKE_COMMAND}" -E chdir "${preprocess_directory}" ${preprocess} "${preprocess_source}" "${object}.i.utf8"
    COMMAND "${SHENMUE_EVENT_ICONV}" -f UTF-8 -t EUC-JP -o "${object}.i" "${object}.i.utf8"
    COMMAND ${compile}
    COMMAND "${_event_as}" -little --no-pad-sections "${object}.s" -o "${object}"
    DEPENDS "${source}" ${_event_headers} shenmue_event_gcc shenmue_event_newlib
    VERBATIM)
  set(${out}_object "${object}" PARENT_SCOPE)
endfunction()

function(_shenmue_event_runtime_object var name)
  if(TARGET shenmue_event_runtime_${name})
    get_target_property(object shenmue_event_runtime_${name} EVENT_OBJECT)
    set(${var} "${object}" PARENT_SCOPE)
    return()
  endif()
  if(name STREQUAL "memset")
    _shenmue_event_object("runtime/${name}.o" ASM "${_event_newlib_source}/machine/sh/memset.S" -I${_event_newlib_source}/machine/sh
      -I${_event_newlib_source}/include)
  elseif(name MATCHES "^(memcpy|memchr|strcat|strcpy|strlen|strcmp|strncmp)$")
    _shenmue_event_object("runtime/${name}.o" LIBC "${_event_newlib_source}/string/${name}.c")
  elseif(name STREQUAL "atof")
    _shenmue_event_object("runtime/${name}.o" LIBC "${_event_newlib_source}/stdlib/atof.c")
  elseif(name STREQUAL "ctype_")
    _shenmue_event_object("runtime/${name}.o" LIBC "${_event_newlib_source}/ctype/ctype_.c")
  elseif(name MATCHES "^(sprintf|vsprintf|strtod|ctype|infinity)$")
    _shenmue_event_object("runtime/${name}.o" LIBC "${_event_root}/runtime/${name}.c")
  elseif(name STREQUAL "fixunssfsi")
    _shenmue_event_object("runtime/${name}.o" LIBGCC "${_event_gcc_source}/gcc/libgcc2.c" -DIN_GCC -DIN_LIBGCC2 -D__GCC_FLOAT_NOT_NEEDED -Dinhibit_libc
      -DL_${name})
  elseif(name MATCHES "^(movstr|movstr_i4)$")
    _shenmue_event_object("runtime/${name}.o" ASM "${_event_gcc_source}/gcc/config/sh/lib1funcs.asm" -DL_${name})
  elseif(name MATCHES "^(sdivsi3_i4|udivsi3_i4)$")
    _shenmue_event_object("runtime/${name}.o" ASM "${_event_root}/runtime/division_fpu.S" -DL_${name})
  else()
    message(FATAL_ERROR "Unknown event runtime member: ${name}")
  endif()
  set(object "${runtime/${name}.o_object}")
  add_custom_target(shenmue_event_runtime_${name} DEPENDS "${object}")
  set_target_properties(shenmue_event_runtime_${name} PROPERTIES EVENT_OBJECT "${object}")
  set(${var} "${object}" PARENT_SCOPE)
endfunction()

set(SHENMUE_DC_EVENT_FUNCTION_TABLE 0x0c020200 CACHE STRING "Event function table (FTBL) address in the selected main executable")

function(shenmue_add_event_overlay name)
  cmake_parse_arguments(EVENT "C" "TYPE;ENTRY;REFERENCE" "SOURCES;RUNTIME;DEFINES;OPTIONS" ${ARGN})
  set(_event_game_options ${EVENT_OPTIONS})
  set(kind GAME)
  if(EVENT_C)
    set(kind GAME_C)
  endif()
  if(EVENT_TYPE STREQUAL "EV1")
    set(address 0x0c328000)
  elseif(EVENT_TYPE STREQUAL "EV2")
    set(address 0x0c338000)
  elseif(EVENT_TYPE STREQUAL "EV3")
    set(address 0x0c348000)
  else()
    message(FATAL_ERROR "Unknown event overlay type: ${EVENT_TYPE}")
  endif()
  set(objects)
  list(TRANSFORM EVENT_DEFINES PREPEND -D OUTPUT_VARIABLE defines)
  if(EVENT_ENTRY)
    get_filename_component(stem "${EVENT_ENTRY}" NAME_WE)
    _shenmue_event_object("${name}/${stem}_entry.o" ${kind} "${PROJECT_SOURCE_DIR}/src/${EVENT_ENTRY}" -DSHENMUE_EVENT_ENTRY=1 ${defines})
    list(APPEND objects "${${name}/${stem}_entry.o_object}")
  endif()
  foreach(entry IN LISTS EVENT_SOURCES)
    set(source "${entry}")
    set(source_defines)
    if(entry MATCHES "^([^@]+)@(.+)$")
      set(source "${CMAKE_MATCH_1}")
      set(source_defines "-D${CMAKE_MATCH_2}")
    endif()
    get_filename_component(stem "${source}" NAME_WE)
    if(source_defines)
      string(MAKE_C_IDENTIFIER "${CMAKE_MATCH_2}" suffix)
      string(TOLOWER "${suffix}" suffix)
      set(stem "${stem}_${suffix}")
    endif()
    _shenmue_event_object("${name}/${stem}.o" ${kind} "${PROJECT_SOURCE_DIR}/src/${source}" ${defines} ${source_defines})
    list(APPEND objects "${${name}/${stem}.o_object}")
  endforeach()
  foreach(member IN LISTS EVENT_RUNTIME)
    _shenmue_event_runtime_object(object "${member}")
    list(APPEND objects "${object}")
  endforeach()
  set(output_dir "${CMAKE_CURRENT_BINARY_DIR}/events")
  set(elf "${output_dir}/${name}.elf")
  set(output "${output_dir}/${name}.${EVENT_TYPE}")
  set(script "${_event_root}/event.ld")
  set(compare)
  set(reference)
  foreach(root IN LISTS SHENMUE_DC_EVENT_REFERENCE_DIR)
    if(NOT compare AND EXISTS "${root}/${EVENT_REFERENCE}")
      set(reference "${root}/${EVENT_REFERENCE}")
      set(compare COMMAND "${CMAKE_COMMAND}" -E compare_files "${output}" "${root}/${EVENT_REFERENCE}")
    endif()
  endforeach()
  if(SHENMUE_DC_EVENT_REFERENCE_DIR AND NOT compare)
    message(WARNING "No reference ${EVENT_REFERENCE} for event overlay ${name}")
    list(GET SHENMUE_DC_EVENT_REFERENCE_DIR 0 root)
    set(reference "${root}/${EVENT_REFERENCE}")
  endif()
  add_custom_command(OUTPUT "${output}"
    COMMAND "${_event_ld}" -EL -T "${script}" --defsym=__event_base=${address} --defsym=_FTBL=${SHENMUE_DC_EVENT_FUNCTION_TABLE}
      -Map "${output_dir}/${name}.map" ${objects} "${_event_root}/event_resident.ld" -o "${elf}"
    COMMAND "${CMAKE_OBJCOPY}" -O binary "${elf}" "${output}"
    ${compare}
    DEPENDS ${objects} "${script}" "${_event_root}/event_resident.ld"
    VERBATIM)
  add_custom_target(shenmue2_dc_event_${name} ALL DEPENDS "${output}")
  foreach(member IN LISTS EVENT_RUNTIME)
    add_dependencies(shenmue2_dc_event_${name} shenmue_event_runtime_${member})
  endforeach()
  if(SHENMUE_DC_EVENT_REFERENCE_DIR)
    set_target_properties(shenmue2_dc_event_${name} PROPERTIES EVENT_OUTPUT "${output}" EVENT_REFERENCE "${reference}")
    set_property(DIRECTORY APPEND PROPERTY SHENMUE_EVENT_TARGETS shenmue2_dc_event_${name})
  endif()
endfunction()

set(_event_c_sources ON)
if(_event_c_sources)
  set(_event_c C)
  set(_event_ext c)
else()
  set(_event_c)
  set(_event_ext cpp)
endif()
set(_event_library s2/event_queued_dc.${_event_ext})
set(_event_library_runtime memcpy strcat strcpy)
set(_event_utility s2/event_utility_dc.${_event_ext})
set(_event_utility_v1 s2/event_utility_v1_dc.${_event_ext})
set(_event_area_base s2/areas/common/src/et_areabase.c)
set(_event_debug s2/event_debug_dc.${_event_ext})
set(_event_library_tail s2/event_library_tail_dc.${_event_ext})
set(_event_sleep s2/event_sleep_dc.${_event_ext})

shenmue_add_event_overlay(DGCT ${_event_c} TYPE EV3 REFERENCE EVENT/DGCT.EV3 ENTRY s2/areas/area_dgct.cpp
  SOURCES s2/areas/area_dgct.cpp s2/areas/exqt_input.cpp ${_event_library} RUNTIME ${_event_library_runtime} fixunssfsi movstr_i4 sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(WBK1_TES2 ${_event_c} TYPE EV1 REFERENCE SCENE/01/WBK1/TES2.EV1 ENTRY s2/areas/area_tes2.cpp SOURCES s2/areas/area_tes2.cpp)
shenmue_add_event_overlay(AKT0_WNPC ${_event_c} TYPE EV3 REFERENCE SCENE/01/AKT0/WNPC.EV3 SOURCES s2/wnpc_event.cpp)
shenmue_add_event_overlay(WSG1_GETO ${_event_c} TYPE EV2 REFERENCE SCENE/01/WSG1/GETO.EV2 ENTRY s2/geto_event.cpp SOURCES s2/geto_event.cpp)
shenmue_add_event_overlay(QF40_0273 ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF40/0273.EV3 ENTRY s2/areas/area_0273.cpp SOURCES s2/areas/area_0273.cpp)
shenmue_add_event_overlay(S03_0362 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0362/0362.EV3 ENTRY s2/areas/area_0362.cpp
  SOURCES s2/areas/area_0362.cpp ${_event_utility})
shenmue_add_event_overlay(S03_0252 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0252/0252.EV3 ENTRY s2/areas/area_0252.cpp
  SOURCES s2/areas/area_0252.cpp ${_event_utility})
shenmue_add_event_overlay(S03_0221 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0221/0221.EV3 ENTRY s2/areas/area_0221.cpp
  SOURCES s2/areas/area_0221.cpp ${_event_utility})
shenmue_add_event_overlay(S01_0001 ${_event_c} TYPE EV3 REFERENCE SCENE/01/0001/0001.EV3 ENTRY s2/areas/area_0001.cpp SOURCES s2/areas/area_0001.cpp)
shenmue_add_event_overlay(S01_0017 ${_event_c} TYPE EV3 REFERENCE SCENE/01/0017/0017.EV3 ENTRY s2/areas/area_0017.cpp
  SOURCES s2/areas/area_0017.cpp ${_event_utility})
shenmue_add_event_overlay(OP02 ${_event_c} TYPE EV3 REFERENCE SCENE/01/OP02/OP02.EV3 ENTRY s2/areas/area_op02.cpp
  SOURCES s2/areas/area_op02.cpp ${_event_utility})
shenmue_add_event_overlay(WT00_192E ${_event_c} TYPE EV3 REFERENCE SCENE/01/WT00/192E.EV3 ENTRY s2/areas/area_192e.cpp SOURCES s2/areas/area_192e.cpp)
shenmue_add_event_overlay(WS00_ANBD ${_event_c} TYPE EV3 REFERENCE SCENE/01/WS00/ANBD.EV3 SOURCES s2/anbd_event.cpp)
shenmue_add_event_overlay(KWWQ ${_event_c} TYPE EV1 REFERENCE SCENE/04/KWQA/KWWQ.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_kwwq.cpp ${_event_area_base})
shenmue_add_event_overlay(KRK4 ${_event_c} TYPE EV1 REFERENCE SCENE/04/KRK4/KRK4.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_krk4.cpp ${_event_area_base})
shenmue_add_event_overlay(QESC ${_event_c} TYPE EV3 REFERENCE EVENT/QESC.EV3 ENTRY s2/areas/area_qesc.cpp
  SOURCES s2/areas/area_qesc.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S02_0039 ${_event_c} TYPE EV3 REFERENCE SCENE/02/0039/0039.EV3 ENTRY s2/areas/area_0039.cpp
  SOURCES s2/areas/area_0039.cpp ${_event_utility})
shenmue_add_event_overlay(S02_0048 ${_event_c} TYPE EV3 REFERENCE SCENE/02/0048/0048.EV3 ENTRY s2/areas/area_0048.cpp
  SOURCES s2/areas/area_0048.cpp ${_event_sleep} ${_event_utility})
shenmue_add_event_overlay(S02_0055 ${_event_c} TYPE EV3 REFERENCE SCENE/02/0055/0055.EV3 ENTRY s2/areas/area_0055.cpp
  SOURCES s2/areas/area_0055.cpp ${_event_sleep} ${_event_utility})
shenmue_add_event_overlay(S02_0059 ${_event_c} TYPE EV3 REFERENCE SCENE/02/0059/0059.EV3 ENTRY s2/areas/area_0059.cpp
  SOURCES s2/areas/area_0059.cpp ${_event_sleep} ${_event_utility})
shenmue_add_event_overlay(S02_0142 ${_event_c} TYPE EV3 REFERENCE SCENE/02/0142/0142.EV3 ENTRY s2/areas/area_0142.cpp
  SOURCES s2/areas/area_0142.cpp ${_event_sleep} ${_event_utility} DEFINES SHENMUE_EVENT_SLEEP_REVISION=1)
shenmue_add_event_overlay(S03_0240 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0240/0240.EV3 ENTRY s2/areas/area_0240.cpp
  SOURCES s2/areas/area_0240.cpp ${_event_utility})
shenmue_add_event_overlay(S03_0250 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0250/0250.EV3 ENTRY s2/areas/area_0250.cpp
  SOURCES s2/areas/area_0250.cpp ${_event_utility})
shenmue_add_event_overlay(S04_0505 ${_event_c} TYPE EV3 REFERENCE SCENE/04/0505/0505.EV3 ENTRY s2/areas/area_0505.cpp
  SOURCES s2/areas/area_0505.cpp ${_event_utility_v1})
shenmue_add_event_overlay(QF00_QRDK ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/QRDK.EV3 SOURCES s2/areas/area_qrdk.cpp ${_event_debug} RUNTIME movstr_i4)
shenmue_add_event_overlay(S03_0200 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0200/0200.EV3 SOURCES s2/areas/area_0200.cpp ${_event_library_tail} ${_event_utility}
  RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)
set(_event_sk s2/sk_event_dc.c)
shenmue_add_event_overlay(KRK2 ${_event_c} TYPE EV1 REFERENCE SCENE/04/KRK2/KRK2.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_krk2.cpp ${_event_area_base} ${_event_library} RUNTIME ${_event_library_runtime})
shenmue_add_event_overlay(ISTB ${_event_c} TYPE EV3 REFERENCE SCENE/04/KRK2/ISTB.EV3 SOURCES s2/areas/ISTB/src/et_ISTB.c ${_event_library} ${_event_sk}
  RUNTIME ${_event_library_runtime})
shenmue_add_event_overlay(QR00_GAIS ${_event_c} TYPE EV3 REFERENCE SCENE/03/QR00/GAIS.EV3 SOURCES s2/areas/area_gais.cpp ${_event_library}
  RUNTIME ${_event_library_runtime})
shenmue_add_event_overlay(KEBI ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR02/KEBI.EV3 ENTRY s2/areas/area_kebi.cpp
  SOURCES s2/areas/area_kebi.cpp ${_event_debug} ${_event_library} ${_event_library_tail} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QAXX ${_event_c} TYPE EV1 REFERENCE SCENE/03/QAXX/QAXX.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_qaxx.cpp ${_event_area_base} ${_event_library} ${_event_library_tail} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(TOFB ${_event_c} TYPE EV3 REFERENCE SCENE/02/TOFB/TOFB.EV3 ENTRY s2/areas/area_tofb.cpp
  SOURCES s2/areas/area_tofb.cpp ${_event_debug} ${_event_library} ${_event_library_tail} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(BTEL ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/BTEL.EV3 ENTRY s2/areas/area_btel.cpp
  SOURCES s2/areas/area_btel.cpp s2/areas/area_btel.cpp@SHENMUE_EVENT_BTEL_BATTLE=1 ${_event_debug} ${_event_library} ${_event_library_tail}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(VSYB ${_event_c} TYPE EV3 REFERENCE SCENE/02/AKY0/VSYB.EV3 ENTRY s2/areas/area_vsyb.cpp
  SOURCES s2/areas/area_vsyb.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S03_0308 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0308/0308.EV3 ENTRY s2/areas/area_0308.cpp
  SOURCES s2/areas/area_0308.cpp ${_event_sleep} ${_event_utility})
shenmue_add_event_overlay(S03_0278 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0278/0278.EV3 ENTRY s2/areas/area_0278.cpp
  SOURCES s2/areas/area_0278.cpp ${_event_sleep} ${_event_utility})
shenmue_add_event_overlay(YMQB ${_event_c} TYPE EV3 REFERENCE SCENE/02/YMQB/YMQB.EV3 ENTRY s2/areas/area_ymqb.cpp
  SOURCES s2/areas/area_ymqb.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sleep}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(TBAK ${_event_c} TYPE EV1 REFERENCE SCENE/03/QTB1/TBAK.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_tbak.cpp ${_event_area_base} s2/areas/common/et_setgamble.c RUNTIME movstr_i4 sdivsi3_i4 DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT)
shenmue_add_event_overlay(AR02 ${_event_c} TYPE EV1 REFERENCE SCENE/01/AR02/AR02.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_ar02.cpp ${_event_area_base} s2/areas/area_pigeons.cpp s2/areas/area_entities.cpp)
shenmue_add_event_overlay(S01_0004 ${_event_c} TYPE EV3 REFERENCE SCENE/01/0004/0004.EV3 ENTRY s2/areas/area_0004.cpp
  SOURCES s2/areas/area_0004.cpp ${_event_library} ${_event_utility} RUNTIME ${_event_library_runtime})
shenmue_add_event_overlay(S01_0549 ${_event_c} TYPE EV3 REFERENCE SCENE/01/0549/0549.EV3 ENTRY s2/areas/area_0549.cpp
  SOURCES s2/areas/area_0549.cpp ${_event_library} ${_event_utility} RUNTIME ${_event_library_runtime} DEFINES SHENMUE_EVENT_LIBRARY_REVISION=30)
shenmue_add_event_overlay(AK00_0175 ${_event_c} TYPE EV3 REFERENCE SCENE/02/AK00/0175.EV3
  SOURCES s2/areas/area_0175.cpp ${_event_library} ${_event_utility} RUNTIME ${_event_library_runtime} DEFINES SHENMUE_EVENT_LIBRARY_REVISION=30)
shenmue_add_event_overlay(AR02_0009 ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR02/0009.EV3
  SOURCES s2/areas/area_0009.cpp ${_event_library} ${_event_utility_v1} RUNTIME strcat strcpy DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10)
shenmue_add_event_overlay(AKA3_AKTM ${_event_c} TYPE EV3 REFERENCE SCENE/01/AKA3/AKTM.EV3 ENTRY s2/areas/area_aktm.cpp
  SOURCES s2/areas/area_aktm.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 DEFINES SHENMUE_EVENT_LIBRARY_REVISION=20)
shenmue_add_event_overlay(QYSS ${_event_c} TYPE EV3 REFERENCE SCENE/03/QA11/QYSS.EV3 ENTRY s2/areas/area_qyss.cpp
  SOURCES s2/areas/area_qyss.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(TGBT ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/TGBT.EV3 ENTRY s2/areas/area_tgbt.cpp
  SOURCES s2/areas/area_tgbt.cpp@SHENMUE_EVENT_TGBT_AUTHOR=1 s2/areas/area_tgbt.cpp ${_event_debug} ${_event_library} s2/areas/common/src/et_coin_toss.c
  ${_event_library_tail} ${_event_sk} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(Q200 ${_event_c} TYPE EV1 REFERENCE SCENE/03/Q200/Q200.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_q200.cpp ${_event_debug} ${_event_area_base} ${_event_library} ${_event_library_tail}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4
  DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=20 SHENMUE_EVENT_LIBRARY_REVISION=25)
shenmue_add_event_overlay(S03_0337 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0337/0337.EV3 ENTRY s2/areas/area_0337.cpp
  SOURCES s2/areas/area_0337.cpp ${_event_library} ${_event_utility_v1} RUNTIME strcat strcpy DEFINES SHENMUE_EVENT_LIBRARY_REVISION=3)
shenmue_add_event_overlay(S03_0276 ${_event_c} TYPE EV3 REFERENCE SCENE/03/0276/0276.EV3 SOURCES s2/areas/area_0276.cpp ${_event_sleep} ${_event_utility}
  DEFINES SHENMUE_EVENT_SLEEP_REVISION=3)
shenmue_add_event_overlay(ABLB ${_event_c} TYPE EV3 REFERENCE SCENE/02/ABLB/ABLB.EV3 ENTRY s2/areas/area_ablb.cpp
  SOURCES s2/areas/area_ablb.cpp@SHENMUE_EVENT_ABLB_AUTHOR=1 s2/areas/area_ablb.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_utility}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
set(_event_area_loop_poses s2/areas/area_loop_dc.c@SHENMUE_AREA_LOOP_HAND_POSE_TABLE=1)
shenmue_add_event_overlay(PAWN ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR03/PAWN.EV3 SOURCES s2/areas/PAWN/src/et_main.c RUNTIME fixunssfsi)
shenmue_add_event_overlay(BRIB ${_event_c} TYPE EV3 REFERENCE SCENE/03/QEHI/BRIB.EV3 SOURCES s2/areas/area_brib.cpp RUNTIME fixunssfsi)
shenmue_add_event_overlay(WT00_WTKQ ${_event_c} TYPE EV3 REFERENCE SCENE/02/WT00/WTKQ.EV3 ENTRY s2/areas/area_wtkq.cpp
  SOURCES s2/areas/area_wtkq.cpp ${_event_debug} ${_event_library} ${_event_area_loop_poses} ${_event_sk} ${_event_utility}
  RUNTIME ${_event_library_runtime})
shenmue_add_event_overlay(S02_AR02_4BTL ${_event_c} TYPE EV3 REFERENCE SCENE/02/AR02/4BTL.EV3 ENTRY s2/areas/area_4btl.cpp
  SOURCES s2/areas/area_4btl.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(BJMP ${_event_c} TYPE EV3 REFERENCE SCENE/03/QAXX/BJMP.EV3 ENTRY s2/areas/BJMP/src/et_BJMP.c
  SOURCES s2/areas/BJMP/src/et_BJMP.c ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QF00_QTCV ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/QTCV.EV3 ENTRY s2/areas/area_qtcv.cpp
  SOURCES s2/areas/area_qtcv.cpp s2/areas/area_qtcv.cpp@SHENMUE_EVENT_QTCV_CARRY=1 ${_event_debug} ${_event_library} ${_event_library_tail}
  ${_event_area_loop_poses} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QF02_QKQB ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF02/QKQB.EV3 ENTRY s2/qkqb_event.cpp
  SOURCES s2/qkqb_event.cpp s2/qkqb_event.cpp@SHENMUE_EVENT_QKQB_CUTS=1 ${_event_debug} ${_event_library} ${_event_library_tail}
  ${_event_area_loop_poses} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QRC0_KORO ${_event_c} TYPE EV2 REFERENCE SCENE/03/QRC0/KORO.EV2 ENTRY s2/areas/area_koro.cpp
  SOURCES s2/areas/area_koro.cpp ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S01_AR02_4BTL ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR02/4BTL.EV3 ENTRY s2/areas/area_4btl.cpp
  SOURCES s2/areas/area_4btl.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 DEFINES SHENMUE_EVENT_4BTL_SCENE01 SHENMUE_EVENT_LIBRARY_REVISION=28)
shenmue_add_event_overlay(QF00_Q7QB ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/Q7QB.EV3 ENTRY s2/areas/area_q7qb.cpp
  SOURCES s2/areas/area_q7qb.cpp s2/areas/area_q7qb_choreography.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(WRSQ ${_event_c} TYPE EV2 REFERENCE SCENE/02/WR00/WRSQ.EV2 ENTRY s2/areas/area_wrsq.cpp
  SOURCES s2/areas/area_wrsq.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(BELL ${_event_c} TYPE EV2 REFERENCE SCENE/03/QBAA/BELL.EV2 ENTRY s2/areas/area_bell.cpp
  SOURCES s2/areas/area_bell.cpp ${_event_library} ${_event_library_tail} ${_event_area_loop_poses} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(ARKQ ${_event_c} TYPE EV3 REFERENCE SCENE/02/ARKQ/ARKQ.EV3 ENTRY s2/areas/area_arkq.cpp
  SOURCES s2/areas/area_arkq.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(WKKT ${_event_c} TYPE EV2 REFERENCE SCENE/01/WK00/WKKT.EV2 ENTRY s2/areas/WKKT/src/et_striketree.c
  SOURCES s2/areas/WKKT/src/et_striketree.c ${_event_library} ${_event_library_tail} ${_event_area_loop_poses} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen movstr_i4 sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(MQTE ${_event_c} TYPE EV3 REFERENCE SCENE/04/KWQA/MQTE.EV3 ENTRY s2/areas/MQTE/src/et_forestqte.c
  SOURCES s2/areas/mqte_effects.cpp@SHENMUE_EVENT_MQTE_AUTHOR=1 s2/areas/mqte_effects.cpp s2/areas/MQTE/src/et_forestqte.c
  s2/areas/mqte_effects.cpp@SHENMUE_EVENT_MQTE_CAMERA=1 ${_event_debug} ${_event_library} ${_event_area_loop_poses} ${_event_sk}
  RUNTIME memcpy memset strcat strcpy)
shenmue_add_event_overlay(ARZ0_TOMA ${_event_c} TYPE EV2 REFERENCE SCENE/01/ARZ0/TOMA.EV2 ENTRY s2/areas/area_toma.cpp
  SOURCES s2/areas/area_toma.cpp RUNTIME fixunssfsi)
shenmue_add_event_overlay(QDTG ${_event_c} TYPE EV3 REFERENCE SCENE/03/QDTG/QDTG.EV3 ENTRY s2/areas/area_qdtg.cpp
  SOURCES s2/areas/area_qdtg.cpp s2/areas/area_qdtg.cpp@SHENMUE_EVENT_QDTG_MOTIONS=1 s2/areas/area_qdtg.cpp@SHENMUE_EVENT_QDTG_OBJECTS=1
  ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses} s2/event_smoke.cpp
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S04_0504 ${_event_c} TYPE EV3 REFERENCE SCENE/04/0504/0504.EV3 ENTRY s2/areas/area_0504.cpp SOURCES s2/areas/area_0504.cpp ${_event_utility} OPTIONS -fno-math-errno)
shenmue_add_event_overlay(S01_VIEW ${_event_c} TYPE EV3 REFERENCE SCENE/01/VIEW/VIEW.EV3 SOURCES s2/areas/area_view.cpp)
shenmue_add_event_overlay(S01_1000 ${_event_c} TYPE EV1 REFERENCE SCENE/01/1000/1000.EV1 SOURCES s2/areas/area_1000.cpp s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1
  RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QYUA ${_event_c} TYPE EV3 REFERENCE SCENE/03/QAXX/QYUA.EV3 ENTRY s2/areas/area_qyua.cpp
  SOURCES s2/areas/area_qyua.cpp s2/areas/area_qyua.cpp@SHENMUE_EVENT_QYUA_SEGMENTS=1 ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S01_AR02_FRBT ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR02/FRBT.EV3
  SOURCES s2/areas/area_frbt.cpp s2/areas/common/et_abort.c ${_event_library} ${_event_library_tail} ${_event_sk} ${_event_utility_v1}
  RUNTIME strcat strcpy vsprintf memchr memcpy strlen sdivsi3_i4 udivsi3_i4
  DEFINES SHENMUE_EVENT_LIBRARY_REVISION=2 SHENMUE_EVENT_SK_REVISION=1)
shenmue_add_event_overlay(S01_WK00_TKYB ${_event_c} TYPE EV3 REFERENCE SCENE/01/WK00/TKYB.EV3 ENTRY s2/areas/area_tkyb.cpp
  SOURCES s2/areas/area_tkyb.cpp s2/areas/common/et_abort.c ${_event_library} s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1 ${_event_sk}
  RUNTIME strcat strcpy vsprintf memchr memcpy strlen sdivsi3_i4 udivsi3_i4
  DEFINES SHENMUE_EVENT_LIBRARY_REVISION=1 SHENMUE_EVENT_SK_REVISION=1)
shenmue_add_event_overlay(QF00_QTNJ ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/QTNJ.EV3 ENTRY s2/qt_nj_event.cpp
  SOURCES s2/qt_nj_event.cpp s2/qt_nj_obstacles.cpp@SHENMUE_EVENT_QTNJ_PLAY=1 s2/qt_nj_obstacles.cpp ${_event_debug} ${_event_library}
  s2/event_movement.cpp ${_event_library_tail} ${_event_area_loop_poses} s2/areas/common/src/et_movesp.c s2/areas/common/et_readnpc.c s2/event_smoke.cpp
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen movstr_i4 sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QJBT_SFJF ${_event_c} TYPE EV3 REFERENCE SCENE/03/QJBT/SFJF.EV3 ENTRY s2/areas/area_sfjf.cpp
  SOURCES s2/areas/area_sfjf.cpp@SHENMUE_EVENT_SFJF_AUTHOR=1 s2/areas/area_sfjf.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QJBT s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QJBT
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(WRS2 ${_event_c} TYPE EV1 REFERENCE SCENE/01/WRS2/WRS2.EV1 ENTRY ${_event_area_base}
  SOURCES s2/areas/area_wrs2.cpp ${_event_area_base} ${_event_library} s2/areas/common/et_darts.c ${_event_library_tail} s2/areas/common/src/et_npc_load.c
  RUNTIME memcpy memset strcat strcpy vsprintf memchr strlen sdivsi3_i4 udivsi3_i4
  DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10 SHENMUE_EVENT_LIBRARY_REVISION=5 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QDMR ${_event_c} TYPE EV2 REFERENCE SCENE/03/QF00/QDMR.EV2 ENTRY s2/areas/area_qdmr.cpp SOURCES s2/areas/area_qdmr.cpp RUNTIME fixunssfsi sdivsi3_i4)
shenmue_add_event_overlay(OMAK_OMAK ${_event_c} TYPE EV1 REFERENCE SCENE/04/OMAK/OMAK.EV1 ENTRY s2/areas/OMAK/src/et_omak.c SOURCES s2/areas/OMAK/src/et_omak.c ${_event_library_tail} RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QF00_BTTM ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF00/BTTM.EV3 ENTRY s2/areas/BTTM/src/et_tamuro.c
  SOURCES s2/areas/BTTM/src/et_tamuro.c s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1 s2/areas/common/src/et_thr.c
  RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(Q100_SFKK ${_event_c} TYPE EV3 REFERENCE SCENE/03/Q100/SFKK.EV3
  SOURCES s2/areas/area_sfkk.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFF s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFF
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(AB00_SFBR ${_event_c} TYPE EV3 REFERENCE SCENE/02/AB00/SFBR.EV3 ENTRY s2/areas/area_sfbr.cpp
  SOURCES s2/areas/area_sfbr.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=ABSF s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=ABSF
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QSFC_SFKM ${_event_c} TYPE EV3 REFERENCE SCENE/03/QSFC/SFKM.EV3 ENTRY s2/areas/area_sfkm.cpp
  SOURCES s2/areas/area_sfkm.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFC s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFC
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(S01_AR01_AOZR ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR01/AOZR.EV3 ENTRY s2/areas/area_aozr.cpp
  SOURCES s2/areas/area_aozr.cpp@SHENMUE_EVENT_AOZR_SCRIPT=1 s2/areas/area_aozr.cpp ${_event_debug} RUNTIME sdivsi3_i4)
shenmue_add_event_overlay(QABT ${_event_c} TYPE EV3 REFERENCE SCENE/03/QABT/QABT.EV3 ENTRY s2/areas/area_qabt.cpp
  SOURCES s2/areas/area_qabt.cpp s2/areas/qabt_authored.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S01_ARBT ${_event_c} TYPE EV3 REFERENCE SCENE/01/ARBT/ARBT.EV3 ENTRY s2/areas/area_arbt.cpp
  SOURCES s2/areas/area_arbt.cpp s2/areas/arbt_authored.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(JUMP ${_event_c} TYPE EV3 REFERENCE EVENT/JUMP.EV3
  SOURCES s2/areas/area_anatobi.cpp ${_event_library} ${_event_library_tail} s2/areas/common/src/et_movesp.c ${_event_utility}
  RUNTIME memcpy sprintf strcat strcpy vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QSFD_SFTC ${_event_c} TYPE EV3 REFERENCE SCENE/03/QSFD/SFTC.EV3 ENTRY s2/areas/area_sftc.cpp
  SOURCES s2/areas/area_sftc.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFD s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFD
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QSFB_SFS2 ${_event_c} TYPE EV3 REFERENCE SCENE/03/QSFB/SFS2.EV3 ENTRY s2/areas/area_sfs2.cpp
  SOURCES s2/areas/area_sfs2.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFB s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFB
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QF00_QFEB ${_event_c} TYPE EV2 REFERENCE SCENE/03/QF00/QFEB.EV2 ENTRY s2/areas/area_qfeb.cpp
  SOURCES s2/areas/area_qfeb.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QA00_TEJO ${_event_c} TYPE EV2 REFERENCE SCENE/03/QA00/TEJO.EV2 ENTRY s2/areas/area_tejo.cpp
  SOURCES s2/areas/area_tejo.cpp s2/areas/TEJO/src/et_tejo.c ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_utility}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QDTB ${_event_c} TYPE EV3 REFERENCE SCENE/03/QDTB/QDTB.EV3 ENTRY s2/areas/QDTB/src/et_tgy1st.c
  SOURCES s2/areas/QDTB/src/et_tgy1st.c@SHENMUE_EVENT_QDTB_AUTHOR=1 s2/areas/QDTB/src/et_tgy1st.c ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/src/et_thr.c@SHENMUE_ET_THR_REVISION=2 s2/areas/QDTB/src/et_tgy1st.c@SHENMUE_EVENT_QDTB_SKY=1
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QTB1_SFTS ${_event_c} TYPE EV3 REFERENCE SCENE/03/QTB1/SFTS.EV3 ENTRY s2/areas/area_sfts.cpp
  SOURCES s2/areas/area_sfts.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFE s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFE
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QSFA_SFM2 ${_event_c} TYPE EV3 REFERENCE SCENE/03/QSFA/SFM2.EV3 ENTRY s2/areas/area_sfm2.cpp
  SOURCES s2/areas/area_sfm2.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFA s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFA
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QF06_QLGT ${_event_c} TYPE EV3 REFERENCE SCENE/03/QF06/QLGT.EV3 ENTRY s2/areas/area_qlgt.cpp
  SOURCES s2/areas/area_qlgt.cpp ${_event_debug} ${_event_library} s2/event_movement.cpp ${_event_library_tail} ${_event_area_loop_poses}
  s2/areas/common/src/et_movesp.c RUNTIME ${_event_library_runtime} vsprintf memchr strlen movstr_i4 sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(S02_AKA3_AKTM ${_event_c} TYPE EV3 REFERENCE SCENE/02/AKA3/AKTM.EV3 ENTRY s2/areas/area_aktm.cpp
  SOURCES s2/areas/area_aktm.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 DEFINES SHENMUE_EVENT_AKTM_RETRIES=1)
shenmue_add_event_overlay(S01_AR02_ARMW ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR02/ARMW.EV3 ENTRY s2/areas/area_armw.cpp
  SOURCES s2/areas/area_armw.cpp@SHENMUE_EVENT_ARMW_CROWD=1 s2/areas/armw_authored.cpp@SHENMUE_EVENT_ARMW_SCENES=1
  s2/areas/armw_authored.cpp s2/areas/area_armw.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  ${_event_area_loop_poses} ${_event_sk} ${_event_utility}
  RUNTIME memcpy memset strcat strcpy vsprintf memchr strlen fixunssfsi movstr_i4 sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S02_AB00_FRBT ${_event_c} TYPE EV3 REFERENCE SCENE/02/AB00/FRBT.EV3 ENTRY s2/areas/area_frbt.cpp
  SOURCES s2/areas/area_frbt.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses} ${_event_sk} ${_event_utility}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4
  DEFINES SHENMUE_EVENT_FRBT_AB00=1)
shenmue_add_event_overlay(QGBT_SFWF ${_event_c} TYPE EV3 REFERENCE SCENE/03/QGBT/SFWF.EV3 ENTRY s2/areas/area_sfwf.cpp
  SOURCES s2/areas/area_sfwf.cpp@SHENMUE_EVENT_SFWF_AUTHOR=1 s2/areas/area_sfwf.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/et_plural_control.c ${_event_area_loop_poses}
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QGBT s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QGBT
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(QLBT_SFSR ${_event_c} TYPE EV3 REFERENCE SCENE/03/QLBT/SFSR.EV3
  SOURCES s2/areas/area_sfsr.cpp s2/areas/area_sfsr.cpp@SHENMUE_EVENT_SFSR_SPOTLIGHT=1 s2/areas/area_sfsr.cpp@SHENMUE_EVENT_SFSR_NORMAL=1
  ${_event_debug} ${_event_library} ${_event_library_tail} s2/areas/common/et_plural_control.c
  s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QLBT s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QLBT
  s2/areas/area_sfsr.cpp@SHENMUE_EVENT_SFSR_SPOTLIGHT_DATA=1
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(S01_AR01_URNI ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR01/URNI.EV3
  SOURCES s2/areas/area_urni.cpp s2/areas/urni_seh.cpp s2/areas/urni_rcd.cpp s2/areas/urni_kln.cpp s2/areas/urni_syk.cpp
  s2/areas/urni_sri.cpp s2/areas/urni_mm5.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_area_loop_poses}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)

shenmue_add_event_overlay(AKS0_DART ${_event_c} TYPE EV3 REFERENCE SCENE/01/AKS0/DART.EV3 ENTRY s2/areas/DART/src/et_darts.c
  SOURCES s2/areas/DART/src/et_darts.c ${_event_debug} ${_event_library} s2/areas/common/et_darts.c@SHENMUE_ET_DARTS_REVISION=2 ${_event_library_tail}
  RUNTIME memcpy memset strcat strcpy vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)

shenmue_add_event_overlay(WT00_DUCK ${_event_c} TYPE EV3 REFERENCE SCENE/02/WT00/DUCK.EV3 ENTRY s2/areas/area_duck.cpp
  SOURCES s2/areas/DUCK/src/et_duck_ctrl.c s2/areas/area_duck_debug.cpp s2/areas/area_duck_effect.cpp s2/areas/area_duck_obstacle.cpp s2/areas/area_duck.cpp
  s2/areas/area_duck_math.cpp s2/areas/area_duck_voice.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk} ${_event_utility}
  RUNTIME memcpy memset strcat strcpy vsprintf memchr strlen movstr_i4 sdivsi3_i4 udivsi3_i4)

shenmue_add_event_overlay(QKBT_SFLU ${_event_c} TYPE EV3 REFERENCE SCENE/03/QKBT/SFLU.EV3 ENTRY s2/areas/area_sflu.cpp
  SOURCES s2/areas/area_sflu.cpp@SHENMUE_EVENT_SFLU_AUTHOR=1 s2/areas/area_sflu.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
  s2/areas/common/et_plural_control.c ${_event_area_loop_poses} s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QKBT
  s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QKBT s2/areas/area_sflu.cpp@SHENMUE_EVENT_SFLU_TRAINING=1
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
shenmue_add_event_overlay(WB00_WBSR ${_event_c} TYPE EV2 REFERENCE SCENE/02/WB00/WBSR.EV2 ENTRY s2/areas/area_wbsr.cpp
  SOURCES s2/areas/area_wbsr.cpp ${_event_library} s2/areas/common/src/et_bunbu.c ${_event_library_tail}
  RUNTIME memcpy strcat strcpy vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(AK00_PVSJ ${_event_c} TYPE EV2 REFERENCE SCENE/01/AK00/PVSJ.EV2 ENTRY s2/areas/PVSJ/src/et_PVSJ.c
  SOURCES s2/areas/PVSJ/src/et_PVSJ.c ${_event_debug} ${_event_library} ${_event_library_tail} s2/areas/common/src/et_pati.c
  RUNTIME atof memcpy sprintf strcat strcmp strcpy strlen strtod vsprintf infinity ctype ctype_ memchr sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(QFRR_STEP ${_event_c} TYPE EV3 REFERENCE SCENE/03/QFRR/STEP.EV3 ENTRY s2/step_event.cpp
  SOURCES s2/step_event.cpp ${_event_debug} ${_event_library} ${_event_library_tail} ${_event_sk} ${_event_utility}
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S02_YTQB ${_event_c} TYPE EV3 REFERENCE SCENE/02/YTQB/YTQB.EV3 ENTRY s2/areas/area_ytqb.cpp
  SOURCES s2/areas/area_ytqb.cpp s2/areas/ytqb_attacks.cpp s2/areas/ytqb_effects.cpp s2/areas/ytqb_choreography.cpp ${_event_debug} ${_event_library}
  ${_event_library_tail} ${_event_area_loop_poses} s2/areas/common/et_readnpc.c s2/event_smoke.cpp
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(WK00_KNMN ${_event_c} TYPE EV3 REFERENCE SCENE/01/WK00/KNMN.EV3 ENTRY s2/areas/area_knmn.cpp
  SOURCES s2/areas/area_knmn.cpp s2/areas/KNMN/src/et_gamekenmin_wd.c s2/areas/knmn_feedback.cpp ${_event_debug} ${_event_library}
  ${_event_library_tail} ${_event_sk} s2/areas/common/src/et_thr.c@SHENMUE_ET_THR_REVISION=2
  s2/areas/common/src/et_wdsub.c@SHENMUE_TRAINING_FIGHT=1 s2/areas/common/src/et_wdsub.c
  RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)

shenmue_add_event_overlay(WS00_ANAN ${_event_c} TYPE EV2 REFERENCE SCENE/01/WS00/ANAN.EV2
  SOURCES s2/areas/area_anan.cpp s2/areas/anan_game.cpp s2/areas/anan_lifecycle.cpp ${_event_debug})

shenmue_add_event_overlay(AK00_PATI ${_event_c} TYPE EV3 REFERENCE SCENE/01/AK00/PATI.EV3 ENTRY s2/areas/PATI/src/et_patinko.c
  SOURCES s2/event_queued_dc.c@SHENMUE_EVENT_LIBRARY_REVISION=25 s2/areas/PATI/src/et_patinko.c
  s2/areas/PATI/src/et_patinko_data.c s2/areas/PATI/src/et_patinko_main.c ${_event_debug} ${_event_library_tail} s2/areas/common/src/et_pati.c
  RUNTIME atof memcpy sprintf strcat strcmp strcpy strlen strncmp strtod vsprintf infinity ctype_ ctype memchr sdivsi3_i4 udivsi3_i4
  DEFINES SHENMUE_EVENT_LIBRARY_PATI=1 OPTIONS -fno-math-errno)

shenmue_add_event_overlay(QRR0_MDTP ${_event_c} TYPE EV3 REFERENCE SCENE/03/QRR0/MDTP.EV3
  SOURCES s2/areas/MDTP/src/et_tapemod.c ${_event_debug} RUNTIME fixunssfsi sdivsi3_i4)

shenmue_add_event_overlay(S01_AR01_0005 ${_event_c} TYPE EV3 REFERENCE SCENE/01/AR01/0005.EV3 ENTRY s2/areas/area_0005.cpp
  SOURCES s2/areas/area_0005.cpp ${_event_library} ${_event_library_tail} RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4)
shenmue_add_event_overlay(S04_0523 ${_event_c} TYPE EV3 REFERENCE SCENE/04/0523/0523.EV3 ENTRY s2/areas/area_0523.cpp
  SOURCES s2/areas/area_0523.cpp RUNTIME sdivsi3_i4)
shenmue_add_event_overlay(WT00_B102 ${_event_c} TYPE EV2 REFERENCE SCENE/01/WT00/B102.EV2 ENTRY s2/areas/area_b102.cpp
  SOURCES s2/areas/area_b102.cpp s2/areas/area_b102.cpp@SHENMUE_EVENT_B102_SHOPS=1 RUNTIME fixunssfsi)
shenmue_add_event_overlay(AK00_RO10 ${_event_c} TYPE EV3 REFERENCE SCENE/01/AK00/RO10.EV3
  SOURCES s2/areas/area_ro10.cpp ${_event_debug} RUNTIME fixunssfsi sdivsi3_i4)

get_property(event_targets DIRECTORY PROPERTY SHENMUE_EVENT_TARGETS)
foreach(target IN LISTS event_targets)
  get_target_property(excluded "${target}" EXCLUDE_FROM_ALL)
  if(excluded)
    message(STATUS "Event reference check excluded: ${target}")
    continue()
  endif()
  get_target_property(output "${target}" EVENT_OUTPUT)
  get_target_property(reference "${target}" EVENT_REFERENCE)
  string(REPLACE "shenmue2_dc_event_" "EV_" test "${target}")
  add_test(NAME "${test}" COMMAND "${CMAKE_COMMAND}" -E compare_files "${output}" "${reference}")
  set_tests_properties("${test}" PROPERTIES LABELS s2-events)
endforeach()
