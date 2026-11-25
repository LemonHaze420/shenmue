option(SHENMUE_DC_EVENT_OVERLAYS "Build Shenmue II Dreamcast EV1/EV2/EV3 overlays with the pinned GCC 2.95.2" OFF)
set(SHENMUE_DC_EVENT_REFERENCE_DIR "" CACHE STRING "Disc data directories (a list) whose EV files the built overlays must match byte-for-byte")
set(SHENMUE_DC_EVENT_TARGET "S2_JP_1003" CACHE STRING "Dreamcast event build")
set_property(CACHE SHENMUE_DC_EVENT_TARGET PROPERTY STRINGS S2_JP_1003 S2_JP_GAMEJAM S2_US S2_EU)
if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_1001")
  set(SHENMUE_DC_EVENT_TARGET "S2_JP_1003" CACHE STRING "Dreamcast event build" FORCE)
endif()
if(NOT SHENMUE_DC_EVENT_TARGET MATCHES "^(S2_JP_1003|S2_JP_GAMEJAM|S2_US|S2_EU)$")
  message(FATAL_ERROR "Unknown Dreamcast event build: ${SHENMUE_DC_EVENT_TARGET}")
endif()
if(NOT SHENMUE_DC_EVENT_OVERLAYS AND NOT SHENMUE_BUILD_S2_DC_COMPILER)
  return()
endif()
if(SHENMUE_DC_EVENT_REFERENCE_DIR)
  enable_testing()
endif()

include(ExternalProject)
find_program(SHENMUE_EVENT_MAKE NAMES make gmake REQUIRED)
find_program(SHENMUE_EVENT_PATCH NAMES patch REQUIRED)
find_program(SHENMUE_EVENT_ICONV NAMES iconv REQUIRED)

function(_shenmue_event_gcc_project name)
  set(version "${ARGV1}")
  if(NOT version)
    set(version 2.95.2)
  endif()
  set(prefix "${CMAKE_BINARY_DIR}/event-gcc")
  set(patch_commands "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-host.patch")
  set(dependencies)
  set(programs cc1)
  set(languages c)
  set(byproducts <BINARY_DIR>/gcc/cc1)
  if(version STREQUAL "2.95.2")
    set(hash 65e68d39ba3f2a6b23431ee361b62fe0103b35c3ccaa818d174ace80dfc587b1)
    list(APPEND dependencies shenmue_event_gxx_source)
    list(APPEND patch_commands
      COMMAND "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-sh-backend.patch"
      COMMAND "${CMAKE_COMMAND}" -E copy_directory "${_event_gxx_source}/gcc/cp" <SOURCE_DIR>/gcc/cp
      COMMAND "${CMAKE_COMMAND}" -E touch <SOURCE_DIR>/gcc/cp/parse.c <SOURCE_DIR>/gcc/cp/parse.h <SOURCE_DIR>/gcc/cp/cfns.h)
    set(programs xgcc g++ cpp cc1 cc1plus cccp)
    set(languages "c c++")
    list(APPEND byproducts <BINARY_DIR>/gcc/xgcc <BINARY_DIR>/gcc/g++ <BINARY_DIR>/gcc/cpp
      <BINARY_DIR>/gcc/cc1plus <BINARY_DIR>/gcc/cccp)
  elseif(version STREQUAL "2.95.3")
    set(hash 56811ee60990b4660d54c2a66f5e0253a487e330ee53b499d8a565882ff16ef7)
    set(prefix "${CMAKE_BINARY_DIR}/event-gcc-${version}")
  else()
    message(FATAL_ERROR "Unknown event compiler version: ${version}")
  endif()
  set(SHENMUE_EVENT_HOST_CC "gcc -m32" CACHE STRING "32-bit host C compiler command used to build the pinned event compilers")
  set(host_cc "${SHENMUE_EVENT_HOST_CC} -D_FILE_OFFSET_BITS=64 -std=gnu89 -fcommon -fgnu89-inline -w -O1")
  file(MAKE_DIRECTORY "${prefix}")
  file(WRITE "${prefix}/host-check.c" "typedef char host_long_is_32_bits[sizeof(long) == 4 ? 1 : -1];\n")
  separate_arguments(host_check UNIX_COMMAND "${host_cc}")
  execute_process(COMMAND ${host_check} -c host-check.c -o host-check.o WORKING_DIRECTORY "${prefix}"
    RESULT_VARIABLE result ERROR_VARIABLE error)
  if(NOT result EQUAL 0)
    message(FATAL_ERROR "SHENMUE_EVENT_HOST_CC must compile 32-bit host code (install gcc-multilib): ${error}")
  endif()
  set(host --host=i686-pc-linux-gnu --build=i686-pc-linux-gnu --target=sh-elf)
  ExternalProject_Add(${name}
    URL https://ftp.gnu.org/gnu/gcc/gcc-${version}/gcc-core-${version}.tar.gz
    URL_HASH SHA256=${hash}
    PREFIX "${prefix}"
    DEPENDS ${dependencies}
    PATCH_COMMAND ${patch_commands}
    CONFIGURE_COMMAND "${CMAKE_COMMAND}" -E make_directory <BINARY_DIR>/libiberty <BINARY_DIR>/gcc/include
      COMMAND "${CMAKE_COMMAND}" -E chdir <BINARY_DIR>/libiberty "${CMAKE_COMMAND}" -E env "CC=${host_cc}" sh <SOURCE_DIR>/libiberty/configure ${host}
      COMMAND "${CMAKE_COMMAND}" -E chdir <BINARY_DIR>/gcc "${CMAKE_COMMAND}" -E env "CC=${host_cc}" sh <SOURCE_DIR>/gcc/configure ${host} --disable-nls
    BUILD_COMMAND "${SHENMUE_EVENT_MAKE}" -C <BINARY_DIR>/libiberty
      COMMAND "${SHENMUE_EVENT_MAKE}" -C <BINARY_DIR>/gcc ${programs} "LANGUAGES=${languages}"
      COMMAND "${CMAKE_COMMAND}" -E copy <SOURCE_DIR>/gcc/glimits.h <BINARY_DIR>/gcc/include/limits.h
      COMMAND "${CMAKE_COMMAND}" -E copy <SOURCE_DIR>/gcc/ginclude/stddef.h <SOURCE_DIR>/gcc/ginclude/stdarg.h <SOURCE_DIR>/gcc/ginclude/va-sh.h
        <BINARY_DIR>/gcc/include
      COMMAND "${CMAKE_COMMAND}" -E copy <SOURCE_DIR>/gcc/config/float-sh.h <BINARY_DIR>/gcc/include/float.h
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${byproducts})
  if(version STREQUAL "2.95.2")
    ExternalProject_Add_Step(${name} math_options
      COMMAND "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-math-errno.patch"
      WORKING_DIRECTORY <SOURCE_DIR>
      DEPENDEES patch
      DEPENDERS configure
      DEPENDS "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-math-errno.patch")
    if(SHENMUE_BUILD_S2_DC_COMPILER)
      ExternalProject_Add_Step(${name} insn_order
        COMMAND "${SHENMUE_EVENT_PATCH}" -p1 -i "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-final-shuid.patch"
        WORKING_DIRECTORY <SOURCE_DIR>
        DEPENDEES math_options
        DEPENDERS configure
        DEPENDS "${PROJECT_SOURCE_DIR}/cmake/patches/gcc-2.95.2-final-shuid.patch")
    endif()
  endif()
endfunction()

function(_shenmue_event_newlib_project name)
  set(version "${ARGV1}")
  if(NOT version)
    set(version 1.8.1)
  endif()
  if(version STREQUAL "1.8.1")
    set(hash bdf73c1b691d952798af33dfa3c7ec90854d89bc6afa100e6c547ee26d7a3c42)
    set(prefix "${CMAKE_BINARY_DIR}/event-newlib")
  elseif(version STREQUAL "1.9.0")
    set(hash 8f7b9407b13b424876b3ecdc31acf593068b48332297a40ae9665d8cfc4ac4e7)
    set(prefix "${CMAKE_BINARY_DIR}/event-newlib-${version}")
  else()
    message(FATAL_ERROR "Unknown event newlib version: ${version}")
  endif()
  ExternalProject_Add(${name}
    URL https://sourceware.org/pub/newlib/newlib-${version}.tar.gz
    URL_HASH SHA256=${hash}
    PREFIX "${prefix}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")
endfunction()

set(SHENMUE_EVENT_TOOLCHAIN_CACHE "" CACHE PATH "Existing event compiler and newlib build cache")
if(SHENMUE_EVENT_TOOLCHAIN_CACHE)
  set(_event_gcc_source "${SHENMUE_EVENT_TOOLCHAIN_CACHE}/event-gcc/src/shenmue_event_gcc")
  set(_event_gcc_build "${SHENMUE_EVENT_TOOLCHAIN_CACHE}/event-gcc/src/shenmue_event_gcc-build/gcc")
  set(_event_gxx_source "${SHENMUE_EVENT_TOOLCHAIN_CACHE}/event-gcc/src/shenmue_event_gxx_source")
  set(_event_newlib_source "${SHENMUE_EVENT_TOOLCHAIN_CACHE}/event-newlib/src/shenmue_event_newlib/newlib/libc")
  foreach(_event_program cc1 cc1plus cccp)
    if(NOT EXISTS "${_event_gcc_build}/${_event_program}")
      message(FATAL_ERROR "Missing cached event compiler: ${_event_gcc_build}/${_event_program}")
    endif()
  endforeach()
  if(NOT EXISTS "${_event_gcc_source}/gcc/libgcc2.c" OR NOT EXISTS "${_event_newlib_source}/include/stdio.h")
    message(FATAL_ERROR "The event toolchain cache is missing GCC or newlib sources")
  endif()
  add_custom_target(shenmue_event_gcc)
  add_custom_target(shenmue_event_newlib)
else()
ExternalProject_Add(shenmue_event_gxx_source
  URL https://ftp.gnu.org/gnu/gcc/gcc-2.95.2/gcc-g++-2.95.2.tar.gz
  URL_HASH SHA256=fa3372cf8a5aca73d4be651eda01cb87811e2f0e76e469b1b8fa124615fa671b
  PREFIX "${CMAKE_BINARY_DIR}/event-gcc"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "")
ExternalProject_Get_Property(shenmue_event_gxx_source SOURCE_DIR)
set(_event_gxx_source "${SOURCE_DIR}")
_shenmue_event_gcc_project(shenmue_event_gcc)
ExternalProject_Get_Property(shenmue_event_gcc SOURCE_DIR BINARY_DIR)
set(_event_gcc_source "${SOURCE_DIR}")
set(_event_gcc_build "${BINARY_DIR}/gcc")

_shenmue_event_newlib_project(shenmue_event_newlib)
ExternalProject_Get_Property(shenmue_event_newlib SOURCE_DIR)
set(_event_newlib_source "${SOURCE_DIR}/newlib/libc")
endif()

if(SHENMUE_BUILD_S2_DC_COMPILER)
  return()
endif()

if(SHENMUE_DC_EVENT_TARGET MATCHES "^S2_(US|EU)$")
  set(SHENMUE_EVENT_RUNTIME_TOOLCHAIN_CACHE "${SHENMUE_EVENT_TOOLCHAIN_CACHE}" CACHE PATH "Existing GCC 2.95.3 and newlib 1.9.0 build cache")
  set(_event_runtime_gcc_build "${SHENMUE_EVENT_RUNTIME_TOOLCHAIN_CACHE}/event-gcc-2.95.3/src/shenmue_event_runtime_gcc-build/gcc")
  if(SHENMUE_EVENT_RUNTIME_TOOLCHAIN_CACHE AND EXISTS "${_event_runtime_gcc_build}/cc1")
    add_custom_target(shenmue_event_runtime_gcc)
  else()
    _shenmue_event_gcc_project(shenmue_event_runtime_gcc 2.95.3)
    ExternalProject_Get_Property(shenmue_event_runtime_gcc BINARY_DIR)
    set(_event_runtime_gcc_build "${BINARY_DIR}/gcc")
  endif()
  set(_event_newlib_190_source "${SHENMUE_EVENT_RUNTIME_TOOLCHAIN_CACHE}/event-newlib-1.9.0/src/shenmue_event_newlib_190/newlib/libc")
  if(SHENMUE_EVENT_RUNTIME_TOOLCHAIN_CACHE AND EXISTS "${_event_newlib_190_source}/string/memchr.c"
      AND EXISTS "${_event_newlib_190_source}/machine/sh/strcpy.S")
    add_custom_target(shenmue_event_newlib_190)
  else()
    _shenmue_event_newlib_project(shenmue_event_newlib_190 1.9.0)
    ExternalProject_Get_Property(shenmue_event_newlib_190 SOURCE_DIR)
    set(_event_newlib_190_source "${SOURCE_DIR}/newlib/libc")
  endif()
endif()

set(_event_cccp "${_event_gcc_build}/cccp")
set(_event_cc1 "${_event_gcc_build}/cc1")
set(_event_cc1plus "${_event_gcc_build}/cc1plus")
set(_event_as "${SHENMUE_DC_TOOLCHAIN_ROOT}/bin/sh4-elf-as")
set(_event_ld "${SHENMUE_DC_TOOLCHAIN_ROOT}/bin/sh4-elf-ld")
set(_event_defines -D__sh__ -D__ELF__ -D__LITTLE_ENDIAN__ -D__SH4_SINGLE_ONLY__)
if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_GAMEJAM")
  list(APPEND _event_defines -DSHENMUE_TARGET_S2_DC_JP_GAMEJAM=1)
elseif(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_US")
  list(APPEND _event_defines -DSHENMUE_TARGET_S2_DC_US=1)
elseif(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_EU")
  list(APPEND _event_defines -DSHENMUE_TARGET_S2_DC_EU=1)
endif()
set(_event_root "${PROJECT_SOURCE_DIR}/src/s2/events")
file(GLOB_RECURSE _event_headers CONFIGURE_DEPENDS "${_event_root}/*.h" "${_event_root}/*.hpp" "${_event_root}/*.inc")
list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/areas/area_loop_dc.c" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_loop_dc.h")
list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/event_queued_dc.h" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_exqt.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/areas/area_dgct.hpp" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_arbt.cpp" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_arbt.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/areas/et_tejo_dc.h" "${PROJECT_SOURCE_DIR}/src/s2/areas/area_knmn.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/training_seqp.hpp")
list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/scripts/cwon/area_cwon.hpp")
file(GLOB_RECURSE _event_common_headers CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/s2/areas/*.h" "${PROJECT_SOURCE_DIR}/src/s2/areas/*.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/areas/*.inl" "${PROJECT_SOURCE_DIR}/src/s2/areas/*.inc" "${PROJECT_SOURCE_DIR}/src/s2/areas/*.def")
list(APPEND _event_headers ${_event_common_headers})
foreach(header anbd_event dai2_event event_movement event_smoke geto_event hide_event krht_event qkqb_event qt_nj_event qt_nj_obstacles step_event wnpc_event)
  list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/${header}.hpp")
endforeach()
list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/qte_game_entry.hpp"
  "${PROJECT_SOURCE_DIR}/src/s2/qte_title_data.inl" "${PROJECT_SOURCE_DIR}/src/s2/qte_machine_data.inl"
  "${PROJECT_SOURCE_DIR}/src/s2/butterfly.hpp")
foreach(_event_c_header event_queued_dc event_utility_dc event_debug_dc event_library_tail_dc event_sleep_dc sk_event_dc areas/numeric_event_dc)
  list(APPEND _event_headers "${PROJECT_SOURCE_DIR}/src/s2/${_event_c_header}.h")
endforeach()

function(_shenmue_event_object out kind source)
  set(object "${CMAKE_CURRENT_BINARY_DIR}/events/${out}")
  get_filename_component(object_dir "${object}" DIRECTORY)
  set(includes -I${PROJECT_SOURCE_DIR}/src -I${_event_gcc_build}/include)
  set(language)
  set(dependencies shenmue_event_gcc shenmue_event_newlib)
  if(kind STREQUAL "GAME")
    set(language -lang-c++ -D__cplusplus -DSHENMUE_EVENT_OVERLAY=1)
    set(compile "${_event_cc1plus}" -quiet -ml -m4-single-only -O1 -fno-exceptions -fno-rtti ${_event_game_options} "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "GAME_C")
    set(language -DSHENMUE_EVENT_OVERLAY=1)
    set(compile "${_event_cc1}" -quiet -ml -m4-single-only -O1 ${_event_game_options} "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "LIBC")
    list(PREPEND includes -I${_event_newlib_source}/include)
    set(compile "${_event_cc1}" -quiet -ml -m4-single-only -O2 "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "LIBC_190")
    list(PREPEND includes -I${_event_newlib_190_source}/include)
    list(APPEND dependencies shenmue_event_runtime_gcc shenmue_event_newlib_190)
    set(compile "${_event_runtime_gcc_build}/cc1" -quiet -ml -m4-single-only -O1 -fno-delayed-branch "${object}.i" -o "${object}.s")
  elseif(kind STREQUAL "LIBGCC")
    list(PREPEND includes -I${_event_gcc_build} -I${_event_gcc_source}/gcc -I${_event_gcc_source}/gcc/config -I${_event_gcc_source}/include)
    set(compile "${_event_cc1}" -quiet -ml -m4-single-only -O2 "${object}.i" -o "${object}.s")
  else()
    set(compile "${CMAKE_COMMAND}" -E copy "${object}.i" "${object}.s")
  endif()
  if(kind MATCHES "^GAME" AND _event_compile_environment)
    list(PREPEND compile "${CMAKE_COMMAND}" -E env ${_event_compile_environment})
  endif()
  set(preprocess "${_event_cccp}" ${language} ${_event_defines} ${ARGN} ${includes})
  if(kind STREQUAL "ASM" OR kind STREQUAL "ASM_190")
    list(APPEND preprocess -lang-asm)
  endif()
  if(kind STREQUAL "ASM_190")
    list(APPEND dependencies shenmue_event_newlib_190)
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
  set(convert "${SHENMUE_EVENT_ICONV}" -f UTF-8 -t EUC-JP -o "${object}.i" "${object}.i.utf8")
  set(assemble "${_event_as}" -little --no-pad-sections "${object}.s" -o "${object}")
  file(CONFIGURE OUTPUT "${object}.args"
    CONTENT "${preprocess_directory}\n${preprocess};${preprocess_source}\n${convert}\n${compile}\n${assemble}\n" @ONLY)
  add_custom_command(OUTPUT "${object}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${object_dir}"
    COMMAND "${CMAKE_COMMAND}" -E chdir "${preprocess_directory}" ${preprocess} "${preprocess_source}" "${object}.i.utf8"
    COMMAND ${convert}
    COMMAND ${compile}
    COMMAND ${assemble}
    DEPENDS "${source}" "${object}.args" ${_event_headers} ${dependencies}
    VERBATIM)
  set(${out}_object "${object}" PARENT_SCOPE)
endfunction()

function(_shenmue_event_runtime_object var name)
  if(TARGET shenmue_event_runtime_${name})
    get_target_property(object shenmue_event_runtime_${name} EVENT_OBJECT)
    set(${var} "${object}" PARENT_SCOPE)
    return()
  endif()
  if(name MATCHES "^(.+)_190$")
    set(member "${CMAKE_MATCH_1}")
    if(member MATCHES "^(memcpy|memset|strcpy)$")
      _shenmue_event_object("runtime190/${member}.o" ASM_190 "${_event_newlib_190_source}/machine/sh/${member}.S"
        -I${_event_newlib_190_source}/machine/sh)
    elseif(member MATCHES "^(memchr|strcat|strlen)$")
      _shenmue_event_object("runtime190/${member}.o" LIBC_190 "${_event_newlib_190_source}/string/${member}.c")
    elseif(member STREQUAL "vsprintf")
      _shenmue_event_object("runtime190/${member}.o" LIBC_190 "${_event_root}/runtime/vsprintf_190.c")
    elseif(member STREQUAL "vsnprintf")
      _shenmue_event_object("runtime190/${member}.o" LIBC_190 "${_event_root}/runtime/vsprintf.c" -DSHENMUE_EVENT_BOUNDED_PRINTF=1)
    else()
      message(FATAL_ERROR "Unknown event newlib 1.9.0 member: ${member}")
    endif()
    set(object "${runtime190/${member}.o_object}")
  elseif(name STREQUAL "memset")
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
  if(NOT name MATCHES "_190$")
    set(object "${runtime/${name}.o_object}")
  endif()
  add_custom_target(shenmue_event_runtime_${name} DEPENDS "${object}")
  set_target_properties(shenmue_event_runtime_${name} PROPERTIES EVENT_OBJECT "${object}")
  set(${var} "${object}" PARENT_SCOPE)
endfunction()

set(SHENMUE_DC_EVENT_FUNCTION_TABLE 0x0c020200 CACHE STRING "Event function table (FTBL) address in the selected main executable")

function(shenmue_add_event_overlay name)
  cmake_parse_arguments(EVENT "C;OPTIONAL" "TYPE;ENTRY;REFERENCE" "SOURCES;TAIL_SOURCES;RUNTIME;DEFINES;OPTIONS;COMPILER_ENVIRONMENT;COMMON_ORDER" ${ARGN})
  if(EVENT_OPTIONAL AND SHENMUE_DC_EVENT_REFERENCE_DIR)
    set(found FALSE)
    foreach(root IN LISTS SHENMUE_DC_EVENT_REFERENCE_DIR)
      if(EXISTS "${root}/${EVENT_REFERENCE}")
        set(found TRUE)
        break()
      endif()
    endforeach()
    if(NOT found)
      return()
    endif()
  endif()
  if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_GAMEJAM")
    if(name STREQUAL "WB00_WBSR")
      set(EVENT_REFERENCE SCENE/01/WB00/WBSR.EV2)
    elseif(name STREQUAL "AK00_RO10")
      set(EVENT_REFERENCE SCENE/01/AR01/RO10.EV3)
    endif()
    set(_gamejam_reference_found FALSE)
    foreach(root IN LISTS SHENMUE_DC_EVENT_REFERENCE_DIR)
      if(EXISTS "${root}/${EVENT_REFERENCE}")
        set(_gamejam_reference_found TRUE)
      endif()
    endforeach()
    if(NOT _gamejam_reference_found)
      return()
    endif()
    if(name STREQUAL "JUMP")
      set(EVENT_SOURCES s2/areas/area_anatobi.cpp s2/event_queued_dc.c s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1
        s2/areas/common/src/et_movesp.c s2/event_utility_v1_dc.c)
      set(EVENT_RUNTIME sprintf strcpy vsprintf memchr memcpy strlen sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "AR02")
      list(APPEND EVENT_DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=15)
    elseif(name STREQUAL "QESC")
      set(EVENT_SOURCES s2/areas/area_qesc.cpp s2/event_debug_dc.c s2/event_queued_dc.c
        s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1)
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_DEBUG_GAMEJAM=1)
      set(EVENT_RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "S01_AR01_AOZR")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_DEBUG_GAMEJAM=1)
    elseif(name STREQUAL "AKA3_AKTM")
      set(EVENT_RUNTIME strcat strcpy vsprintf memchr memcpy strlen sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "KEBI")
      list(FILTER EVENT_DEFINES EXCLUDE REGEX "^SHENMUE_EVENT_LIBRARY_REVISION=")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_LIBRARY_REVISION=2 SHENMUE_EVENT_LIBRARY_GAMEJAM_KEBI=1)
      set(EVENT_RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "PAWN")
      set(EVENT_ENTRY s2/areas/PAWN/src/et_main.c)
      list(APPEND EVENT_SOURCES s2/event_debug_dc.c)
    elseif(name STREQUAL "DGCT")
      set(EVENT_RUNTIME memcpy fixunssfsi movstr_i4 sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "S01_0549")
      set(EVENT_RUNTIME strcat strcpy)
    elseif(name STREQUAL "AK00_RO10")
      set(EVENT_ENTRY s2/areas/area_ro10.cpp)
    elseif(name STREQUAL "AK00_PATI")
      set(EVENT_RUNTIME atof sprintf strcat strcmp strcpy strlen strncmp strtod vsprintf infinity ctype ctype_ memchr memcpy sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "S01_0004")
      set(EVENT_ENTRY)
      set(EVENT_RUNTIME strcat strcpy)
    elseif(name STREQUAL "S01_AR02_4BTL")
      set(EVENT_RUNTIME strcat strcpy vsprintf memchr memcpy strlen sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "S01_ARBT" OR name STREQUAL "S01_AR01_URNI" OR name STREQUAL "S01_AR01_0005" OR name STREQUAL "WB00_WBSR")
      set(EVENT_RUNTIME strcat strcpy vsprintf memchr memcpy strlen sdivsi3_i4 udivsi3_i4)
    elseif(name STREQUAL "AK00_PVSJ")
      set(EVENT_RUNTIME atof strcat strcmp strcpy strlen strtod vsprintf infinity ctype ctype_ memchr memcpy sdivsi3_i4 udivsi3_i4)
    endif()
    if(name MATCHES "^(KEBI|AKA3_AKTM|PAWN|S01_AR02_4BTL|WKKT|S01_ARBT|S01_AR02_ARMW|S01_AR01_URNI|AKS0_DART|AK00_PVSJ|WK00_KNMN|WS00_ANAN|AK00_RO10|AK00_PATI)$")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_DEBUG_GAMEJAM=1)
    endif()
    if(name MATCHES "^(S01_0017|OP02|S01_0004|S01_0549|S01_AR02_ARMW)$")
      list(TRANSFORM EVENT_SOURCES REPLACE "event_utility_dc[.]c$" "event_utility_v1_dc.c")
    endif()
    if(name MATCHES "^(DGCT|QESC|JUMP)$")
      list(FILTER EVENT_DEFINES EXCLUDE REGEX "^SHENMUE_EVENT_LIBRARY_REVISION=")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_LIBRARY_REVISION=1 SHENMUE_EVENT_LIBRARY_GAMEJAM_JUMP=1)
    elseif(name MATCHES "^(S01_0004|AKA3_AKTM|S01_AR02_4BTL|WKKT|S01_ARBT|S01_AR02_ARMW|S01_AR01_URNI|AK00_PVSJ|S01_AR01_0005|WB00_WBSR|WK00_KNMN)$")
      list(FILTER EVENT_DEFINES EXCLUDE REGEX "^SHENMUE_EVENT_LIBRARY_REVISION=")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10)
    elseif(name STREQUAL "S01_0549")
      list(FILTER EVENT_DEFINES EXCLUDE REGEX "^SHENMUE_EVENT_LIBRARY_REVISION=")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_LIBRARY_REVISION=3)
    elseif(name STREQUAL "AKS0_DART")
      list(FILTER EVENT_DEFINES EXCLUDE REGEX "^SHENMUE_EVENT_LIBRARY_REVISION=")
      list(APPEND EVENT_DEFINES SHENMUE_EVENT_LIBRARY_REVISION=20)
    endif()
  endif()
  if((SHENMUE_DC_EVENT_TARGET STREQUAL "S2_US" AND name MATCHES "^(QJBT_SFJF|Q100_SFKK|AB00_SFBR|QSFC_SFKM|QSFD_SFTC|QSFB_SFS2|QTB1_SFTS|QSFA_SFM2|QGBT_SFWF|QLBT_SFSR|QKBT_SFLU)$")
      OR (SHENMUE_DC_EVENT_TARGET STREQUAL "S2_EU" AND name MATCHES "^(QJBT_SFJF|Q100_SFKK|Q109_SFKK|AB00_SFBR|QSFC_SFKM|QSFD_SFTC|QSFB_SFS2|QTB1_SFTS|QSFA_SFM2|QGBT_SFWF|QLBT_SFSR|QKBT_SFLU|WKKT|WT00_DUCK|OMAK_OMAK|AKS0_DART|S01_AR01_URNI|S01_AR02_ARMW)$"))
    set(runtime)
    foreach(member IN LISTS EVENT_RUNTIME)
      if(member STREQUAL "vsprintf")
        list(APPEND runtime vsprintf_190 vsnprintf_190)
      elseif(member MATCHES "^(memcpy|memset|strcat|strcpy|memchr|strlen)$")
        list(APPEND runtime "${member}_190")
      else()
        list(APPEND runtime "${member}")
      endif()
    endforeach()
    set(EVENT_RUNTIME ${runtime})
  endif()
  if(name STREQUAL "QRR0_MDTP" AND SHENMUE_DC_EVENT_TARGET MATCHES "^S2_(US|EU)$")
    list(INSERT EVENT_SOURCES 1 s2/areas/mdtp_authored.cpp)
    set(EVENT_RUNTIME fixunssfsi movstr_i4 sdivsi3_i4)
  endif()
  set(_event_game_options ${EVENT_OPTIONS})
  set(_event_compile_environment ${EVENT_COMPILER_ENVIRONMENT})
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
  foreach(group SOURCES RUNTIME TAIL_SOURCES)
    if(group STREQUAL "RUNTIME")
      foreach(member IN LISTS EVENT_RUNTIME)
        _shenmue_event_runtime_object(object "${member}")
        list(APPEND objects "${object}")
      endforeach()
      continue()
    endif()
    foreach(entry IN LISTS EVENT_${group})
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
  endforeach()
  set(output_dir "${CMAKE_CURRENT_BINARY_DIR}/events")
  set(elf "${output_dir}/${name}.elf")
  set(output "${output_dir}/${name}.${EVENT_TYPE}")
  set(script "${_event_root}/event.ld")
  if(EVENT_COMMON_ORDER)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${script}")
    set(common_order)
    foreach(entry IN LISTS EVENT_COMMON_ORDER)
      if(NOT entry MATCHES "^([A-Za-z_][A-Za-z_0-9]*):([0-9]+)$")
        message(FATAL_ERROR "Invalid COMMON symbol and size: ${entry}")
      endif()
      string(APPEND common_order "_${CMAKE_MATCH_1} = .; . += ${CMAKE_MATCH_2};\n")
    endforeach()
    file(READ "${script}" layout)
    string(REPLACE "*(COMMON)" "${common_order} *(COMMON)" layout "${layout}")
    set(script "${output_dir}/${name}.ld")
    file(CONFIGURE OUTPUT "${script}" CONTENT "${layout}" @ONLY)
  endif()
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
  set(link "${_event_ld}" -EL -T "${script}" --defsym=__event_base=${address} --defsym=_FTBL=${SHENMUE_DC_EVENT_FUNCTION_TABLE}
    -Map "${output_dir}/${name}.map" ${objects} "${_event_root}/event_resident.ld" -o "${elf}")
  file(CONFIGURE OUTPUT "${output}.args"
    CONTENT "${link}\n${CMAKE_OBJCOPY};-O;binary;${elf};${output}\n${reference}\n" @ONLY)
  add_custom_command(OUTPUT "${output}"
    COMMAND ${link}
    COMMAND "${CMAKE_OBJCOPY}" -O binary "${elf}" "${output}"
    ${compare}
    DEPENDS ${objects} "${script}" "${_event_root}/event_resident.ld" "${output}.args" ${reference}
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
if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_EU")
  shenmue_add_event_overlay(Q109_SFKK ${_event_c} TYPE EV3 REFERENCE SCENE/03/Q109/SFKK.EV3
    SOURCES s2/areas/area_sfkk.cpp ${_event_debug} ${_event_library} ${_event_library_tail}
    s2/areas/common/et_street_debug.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFF s2/areas/common/src/et_street.c@SHENMUE_STREET_FIGHT_PACKAGE=QSFF
    RUNTIME ${_event_library_runtime} vsprintf memchr strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno DEFINES SHENMUE_EVENT_SFKK_Q109=1)
endif()
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

if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_GAMEJAM")
  shenmue_add_event_overlay(EVENT_VEND C TYPE EV3 REFERENCE EVENT/VEND.EV3 ENTRY s2/areas/area_vend.cpp
    SOURCES s2/areas/area_vend.cpp s2/areas/area_vend.cpp@SHENMUE_EVENT_VEND_ACTIONS=1
    ${_event_debug} ${_event_library} ${_event_library_tail}
    TAIL_SOURCES s2/areas/area_vend.cpp@SHENMUE_EVENT_CONSTANTS=1
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=2 SHENMUE_EVENT_LIBRARY_VEND=1 SHENMUE_EVENT_DEBUG_GAMEJAM=1 SHENMUE_EVENT_VEND_SCENE=0
    RUNTIME vsprintf memchr memcpy strcpy strlen movstr_i4 sdivsi3_i4 udivsi3_i4)
  shenmue_add_event_overlay(WE00_VEND C TYPE EV3 REFERENCE SCENE/01/WE00/VEND.EV3 ENTRY s2/areas/area_vend.cpp
    SOURCES s2/areas/area_vend.cpp s2/areas/area_vend.cpp@SHENMUE_EVENT_VEND_ACTIONS=1
    ${_event_debug} ${_event_library} ${_event_library_tail}
    TAIL_SOURCES s2/areas/area_vend.cpp@SHENMUE_EVENT_CONSTANTS=1
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10 SHENMUE_EVENT_DEBUG_GAMEJAM=1 SHENMUE_EVENT_VEND_SCENE=1
    RUNTIME strcat strcpy vsprintf memchr memcpy strlen movstr_i4 sdivsi3_i4 udivsi3_i4)
  shenmue_add_event_overlay(CWON C TYPE EV3 REFERENCE SCENE/01/CWON/CWON.EV3 ENTRY s2/scripts/cwon/area_cwon.cpp
    SOURCES s2/scripts/cwon/area_cwon.cpp s2/scripts/cwon/cwon_authored.cpp s2/scripts/cwon/cwon_battle.cpp
    s2/scripts/cwon/cwon_tables.cpp s2/scripts/cwon/cwon_props.cpp s2/scripts/cwon/cwon_escape.cpp s2/scripts/cwon/cwon_helpers.cpp
    ${_event_library} ${_event_utility_v1} s2/areas/area_loop_dc.c
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10 SHENMUE_AREA_LOOP_HAND_POSE_TABLE=1
    RUNTIME memset strcat strcpy movstr_i4 sdivsi3_i4 OPTIONS -fno-math-errno
    # GCC 2.95.2 delay-sequence ordering requires the native branch choice.
    COMPILER_ENVIRONMENT MALLOC_PERTURB_=128
    COMMON_ORDER cwonPackIndex:4 cwonWaveTask:4 cwonSpringTask:4 cwonPack:4 cwonDynamixTask:4 cwonPackState:4)
  shenmue_add_event_overlay(EVENT_SMBL C TYPE EV3 REFERENCE EVENT/SMBL.EV3 ENTRY s2/areas/area_smbl.cpp
    SOURCES s2/areas/area_smbl_ball.cpp s2/areas/area_smbl.cpp ${_event_library}
    s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=1 SHENMUE_EVENT_LIBRARY_GAMEJAM_JUMP=1
    RUNTIME memcpy vsprintf memchr strcpy strlen sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
  shenmue_add_event_overlay(EVENT_DRBL C TYPE EV3 REFERENCE EVENT/DRBL.EV3 ENTRY s2/areas/area_drbl.cpp
    SOURCES s2/areas/area_drbl.cpp s2/event_library_tail_dc.c@SHENMUE_EVENT_LIBRARY_TAIL_LOG=1
    RUNTIME memcpy vsprintf memchr strcpy strlen movstr_i4 sdivsi3_i4 udivsi3_i4 OPTIONS -fno-math-errno)
  shenmue_add_event_overlay(WESM_DAI2 C TYPE EV3 REFERENCE SCENE/01/WESM/DAI2.EV3 SOURCES s2/dai2_event.cpp)
  shenmue_add_event_overlay(WSG1_TEST C TYPE EV1 REFERENCE SCENE/01/WSG1/TEST.EV1 SOURCES s2/areas/area_test.cpp)
  shenmue_add_event_overlay(EVENT_0000 C TYPE EV3 REFERENCE EVENT/0000.EV3 SOURCES s2/areas/area_0000.cpp)
  shenmue_add_event_overlay(S01_AR02_0008 C TYPE EV3 REFERENCE SCENE/01/AR02/0008.EV3 ENTRY s2/areas/area_0008.cpp
    SOURCES s2/areas/area_0008.cpp s2/event_utility_v1_dc.c)
  shenmue_add_event_overlay(WTA0_0034 C TYPE EV3 REFERENCE SCENE/01/WTA0/0034.EV3 ENTRY s2/areas/area_0034.cpp
    SOURCES s2/areas/area_0034.cpp s2/event_utility_v1_dc.c)
  shenmue_add_event_overlay(S01_AR02_0007 C TYPE EV3 REFERENCE SCENE/01/AR02/0007.EV3 ENTRY s2/areas/area_0007.cpp
    SOURCES s2/areas/area_0007.cpp ${_event_library} ${_event_utility_v1} RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=4)
  shenmue_add_event_overlay(WKA0_0023 C TYPE EV3 REFERENCE SCENE/01/WKA0/0023.EV3 ENTRY s2/areas/area_0023.cpp
    SOURCES s2/areas/area_0023.cpp ${_event_library} ${_event_utility_v1} RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=3)
  shenmue_add_event_overlay(WB01_0025 C TYPE EV3 REFERENCE SCENE/01/WB01/0025.EV3 ENTRY s2/areas/area_0025.cpp
    SOURCES s2/areas/area_0025.cpp ${_event_library} ${_event_utility_v1} RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=3)
  shenmue_add_event_overlay(WESM C TYPE EV1 REFERENCE SCENE/01/WESM/WESM.EV1 ENTRY ${_event_area_base}
    SOURCES s2/areas/area_wesm.cpp s2/areas/area_wesm.cpp@SHENMUE_EVENT_WESM_NPCS=1 ${_event_area_base}
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10)
endif()

# EVs stored inside scene packages.
if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_EU" OR SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_1003" OR SHENMUE_DC_EVENT_TARGET STREQUAL "S2_US")
  shenmue_add_event_overlay(PACK_0007 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0007A.PKF/0007.EV3
    ENTRY s2/areas/area_0007.cpp
    SOURCES s2/areas/area_0007.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0009 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0009A.PKF/0009.EV3
    SOURCES s2/areas/area_0009.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_0009_PACKAGED=1 SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0013 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0013A.PKF/0013.EV3
    ENTRY s2/areas/area_0013.cpp
    SOURCES s2/areas/area_0013.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0014 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0014A.PKF/0014.EV3
    ENTRY s2/areas/area_0014.cpp
    SOURCES s2/areas/area_0014.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0023 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WKA0/0023A.PKF/0023.EV3
    ENTRY s2/areas/area_0023.cpp
    SOURCES s2/areas/area_0023.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=28)

  shenmue_add_event_overlay(PACK_0025 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WB01/0025A.PKF/0025.EV3
    ENTRY s2/areas/area_0025.cpp
    SOURCES s2/areas/area_0025.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0034 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WTA0/0034A.PKF/0034.EV3
    ENTRY s2/areas/area_0034.cpp
    SOURCES s2/areas/area_0034.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_0049 C OPTIONAL TYPE EV3
    REFERENCE SCENE/02/WB00/0049A.PKF/0049.EV3
    ENTRY s2/areas/area_0049.cpp
    SOURCES s2/areas/area_0049.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_0066 C OPTIONAL TYPE EV3
    REFERENCE SCENE/02/WTA0/0066A.PKF/0066.EV3
    ENTRY s2/areas/area_0066.cpp
    SOURCES s2/areas/area_0066.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=30)

  shenmue_add_event_overlay(PACK_0077 C OPTIONAL TYPE EV3
    REFERENCE SCENE/02/WN00/0077A.PKF/0077.EV3
    ENTRY s2/areas/area_0077.cpp
    SOURCES s2/areas/area_0077.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0080 C OPTIONAL TYPE EV3
    REFERENCE SCENE/02/AK00/0080A.PKF/0080.EV3
    ENTRY s2/areas/area_0080.cpp
    SOURCES s2/areas/area_0080.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0139 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR03/0139A.PKF/0139.EV3
    ENTRY s2/areas/area_0139.cpp
    SOURCES s2/areas/area_0139.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0140 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0140A.PKF/0140.EV3
    ENTRY s2/areas/area_0140.cpp
    SOURCES s2/areas/area_0140.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0173 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WB00/0173A.PKF/0173.EV3
    ENTRY s2/areas/area_0173.cpp
    SOURCES s2/areas/area_0173.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=30)

  shenmue_add_event_overlay(PACK_0184 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WS00/0184A.PKF/0184.EV3
    ENTRY s2/areas/area_0184.cpp
    SOURCES s2/areas/area_0184.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=5)

  shenmue_add_event_overlay(PACK_0201 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QR00/0201A1.PKF/0201.EV3
    ENTRY s2/areas/area_0201.cpp
    SOURCES s2/areas/area_0201.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_0222 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QB00/0222A1.PKF/0222.EV3
    ENTRY s2/areas/area_0222.cpp
    SOURCES s2/areas/area_0222.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=20)

  shenmue_add_event_overlay(PACK_0257 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QR00/0257A.PKF/0257.EV3
    ENTRY s2/areas/area_0257.cpp
    SOURCES s2/areas/area_0257.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=27)

  shenmue_add_event_overlay(PACK_0262 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QUG0/0262A.PKF/0262.EV3
    ENTRY s2/areas/area_0262.cpp
    SOURCES s2/areas/area_0262.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=30)

  shenmue_add_event_overlay(PACK_0265 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QUG0/0265A.PKF/0265.EV3
    ENTRY s2/areas/area_0265.cpp
    SOURCES s2/areas/area_0265.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=28)

  shenmue_add_event_overlay(PACK_0297 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QF02/0297A.PKF/0297.EV3
    ENTRY s2/areas/area_0297.cpp
    SOURCES s2/areas/area_0297.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=30)

  shenmue_add_event_overlay(PACK_0307 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/Q300/0307A.PKF/0307.EV3
    ENTRY s2/areas/area_0307.cpp
    SOURCES s2/areas/area_0307.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10)

  shenmue_add_event_overlay(PACK_0337 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/0337/0337A.PKF/0337.EV3
    ENTRY s2/areas/area_0337.cpp
    SOURCES s2/areas/area_0337.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_0337_PACKAGED=1 SHENMUE_EVENT_LIBRARY_REVISION=27)

  shenmue_add_event_overlay(PACK_0339 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QR00/0339A.PKF/0339.EV3
    ENTRY s2/areas/area_0339.cpp
    SOURCES s2/areas/area_0339.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_0361 C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QF00/0361A.PKF/0361.EV3
    ENTRY s2/areas/area_0361.cpp
    SOURCES s2/areas/area_0361.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=5)

  shenmue_add_event_overlay(PACK_0502 C OPTIONAL TYPE EV3
    REFERENCE SCENE/04/KRH1/MPK00.PKF/0502.EV3
    ENTRY s2/areas/area_0502.cpp
    SOURCES s2/areas/area_0502.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_054E C OPTIONAL TYPE EV3
    REFERENCE SCENE/02/AR02/0054E.PKF/054E.EV3
    ENTRY s2/areas/area_054e.cpp
    SOURCES s2/areas/area_054e.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_192E C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WT00/0192E.PKF/192E.EV3
    ENTRY s2/areas/area_192e.cpp
    SOURCES s2/areas/area_192e.cpp
    DEFINES SHENMUE_EVENT_192E_PACKAGED=1)

  shenmue_add_event_overlay(PACK_329E C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QR00/0329E.PKF/329E.EV3
    ENTRY s2/areas/area_329e.cpp
    SOURCES s2/areas/area_329e.cpp s2/event_queued_dc.c s2/event_utility_dc.c
    RUNTIME memcpy strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_552E C OPTIONAL TYPE EV3
    REFERENCE SCENE/04/552E/MPK00.PKF/552E.EV3
    SOURCES s2/areas/area_552e.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_AKA3 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKA3/MPK00.PKF/AKA3.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_aka3.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_AKS0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKS0/MPK00.PKF/AKS0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_aks0.cpp s2/areas/common/src/et_areabase.c s2/event_queued_dc.c s2/areas/common/et_darts.c s2/event_library_tail_dc.c
      s2/areas/common/src/et_npc_load.c s2/areas/area_entities.cpp
    RUNTIME memcpy memset strcat strcpy vsprintf memchr strlen sdivsi3_i4 udivsi3_i4
    DEFINES SHENMUE_AREA_BASE_REVISION=10 SHENMUE_EVENT_LIBRARY_REVISION=40 SHENMUE_ET_DARTS_REVISION=2 SHENMUE_CONDITIONAL_NPC_LOAD_REVISION=2
    OPTIONS -fno-math-errno)

  shenmue_add_event_overlay(PACK_AKT0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKT0/MPK00.PKF/AKT0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_akt0.cpp s2/areas/common/src/et_areabase.c s2/areas/area_jukebox.cpp s2/areas/common/et_setgamble.c
    RUNTIME movstr_i4 sdivsi3_i4)

  shenmue_add_event_overlay(PACK_AKT2 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKT2/MPK00.PKF/AKT2.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_akt2.cpp s2/areas/common/src/et_areabase.c s2/areas/common/et_setgamble.c
    RUNTIME movstr_i4 sdivsi3_i4)

  shenmue_add_event_overlay(PACK_AKT3 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKT3/MPK00.PKF/AKT3.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_akt3.cpp s2/areas/common/src/et_areabase.c s2/areas/common/et_setgamble.c
    RUNTIME movstr_i4 sdivsi3_i4)

  shenmue_add_event_overlay(PACK_AKY0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKY0/MPK00.PKF/AKY0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_aky0.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_ARM0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/ARM0/MPK00.PKF/ARM0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_arm0.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_ARZ0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/ARZ0/MPK00.PKF/ARZ0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_arz0.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_DAI2 C OPTIONAL TYPE EV2
    REFERENCE SCENE/01/WESM/MPK00.PKF/DAI2.EV2
    SOURCES s2/dai2_event.cpp
    DEFINES SHENMUE_DAI2_HIDE_FILM_BARS=1)

  shenmue_add_event_overlay(PACK_HIDE C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QA00/HIDE.PKF/HIDE.EV3
    SOURCES s2/hide_event.cpp)

  shenmue_add_event_overlay(PACK_KES1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KES1/MPK00.PKF/KES1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_kes1.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_KRC1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KRC1/MPK00.PKF/KRC1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_krc1.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_KRH1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KRH1/MPK00.PKF/KRH1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_krh1.cpp s2/areas/common/src/et_areabase.c s2/butterfly.cpp
    RUNTIME sdivsi3_i4)

  shenmue_add_event_overlay(PACK_KRHT C OPTIONAL TYPE EV2
    REFERENCE SCENE/04/KRH1/MPK00.PKF/KRHT.EV2
    ENTRY s2/krht_event.cpp
    SOURCES s2/krht_event.cpp)

  shenmue_add_event_overlay(PACK_KRK6 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KRK6/MPK00.PKF/KRK6.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_krk6.cpp s2/areas/common/src/et_areabase.c s2/event_queued_dc.c s2/areas/area_fall_events.cpp
    RUNTIME memcpy memset strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=40)

  shenmue_add_event_overlay(PACK_KRM3 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KRM3/MPK00.PKF/KRM3.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_krm3.cpp s2/areas/common/src/et_areabase.c s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_KSH1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KSH1/MPK00.PKF/KSH1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_ksh1.cpp s2/areas/common/src/et_areabase.c s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_KWM1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KWM1/MPK00.PKF/KWM1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_kwm1.cpp s2/areas/common/src/et_areabase.c s2/butterfly.cpp
    RUNTIME sdivsi3_i4)

  shenmue_add_event_overlay(PACK_KWW1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KWW1/MPK00.PKF/KWW1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_kww1.cpp s2/areas/common/src/et_areabase.c s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_KWW4 C OPTIONAL TYPE EV1
    REFERENCE SCENE/04/KWW4/MPK00.PKF/KWW4.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_kww4.cpp s2/areas/common/src/et_areabase.c s2/event_utility_dc.c s2/areas/area_loop_dc.c@SHENMUE_AREA_LOOP_HAND_POSE_TABLE=1)

  shenmue_add_event_overlay(PACK_KWWT C OPTIONAL TYPE EV2
    REFERENCE SCENE/04/KWW4/MPK00.PKF/KWWT.EV2
    ENTRY s2/areas/area_kwwt.cpp
    SOURCES s2/areas/area_kwwt.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_QA11 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QA11/MPK00.PKF/QA11.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qa11.cpp s2/areas/common/src/et_areabase.c s2/areas/area_pigeons.cpp
    DEFINES SHENMUE_PIGEON_REVISION=40)

  shenmue_add_event_overlay(PACK_QA22 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QA22/MPK00.PKF/QA22.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qa22.cpp s2/areas/common/src/et_areabase.c s2/event_library_tail_dc.c s2/areas/area_jukebox.cpp
    RUNTIME vsprintf memchr memcpy strcpy strlen sdivsi3_i4 udivsi3_i4)

  shenmue_add_event_overlay(PACK_QAE1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QAE1/MPK00.PKF/QAE1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qae1.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QAE6 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QAE6/MPK00.PKF/QAE6.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qae6.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QAW1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QAW1/MPK00.PKF/QAW1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qaw1.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QAW6 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QAW6/MPK00.PKF/QAW6.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qaw6.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QB00 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QB00/MPK00.PKF/QB00.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qb00.cpp s2/areas/common/src/et_areabase.c s2/areas/area_jukebox.cpp)

  shenmue_add_event_overlay(PACK_QCAE C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QCAE/MPK00.PKF/QCAE.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qcae.cpp s2/areas/common/src/et_areabase.c s2/areas/area_jukebox.cpp)

  shenmue_add_event_overlay(PACK_QDFO C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QD00/QDFO.PKF/QDFO.EV3
    ENTRY s2/areas/area_qdfo.cpp
    SOURCES s2/areas/area_qdfo.cpp s2/event_utility_v1_dc.c)

  shenmue_add_event_overlay(PACK_QDKJ C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QDKJ/MPK00.PKF/QDKJ.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qdkj.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QE01 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QE01/MPK00.PKF/QE01.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qe01.cpp s2/areas/common/src/et_areabase.c s2/areas/area_chime.cpp
    RUNTIME sdivsi3_i4)

  shenmue_add_event_overlay(PACK_QE06 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QE06/MPK00.PKF/QE06.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qe06.cpp s2/areas/common/src/et_areabase.c s2/areas/area_chime.cpp
    RUNTIME sdivsi3_i4)

  shenmue_add_event_overlay(PACK_QEDJ C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QEDJ/MPK00.PKF/QEDJ.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qedj.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QEHI C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QEHI/MPK00.PKF/QEHI.EV1
    ENTRY s2/areas/area_qehi.cpp
    SOURCES s2/areas/area_qehi.cpp s2/qte_game_entry.cpp
    RUNTIME movstr_i4 sdivsi3_i4)

  shenmue_add_event_overlay(PACK_QF39 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QF39/MPK00.PKF/QF39.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qf39.cpp s2/areas/common/src/et_areabase.c s2/areas/area_elevator_lamp.cpp)

  shenmue_add_event_overlay(PACK_QFRR C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QFRR/MPK00.PKF/QFRR.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qfrr.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_QTB4 C OPTIONAL TYPE EV1
    REFERENCE SCENE/03/QTB4/MPK00.PKF/QTB4.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_qtb4.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_TEST C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/MAP0/MPK00.PKS/TEST.EV1
    SOURCES s2/areas/area_test.cpp)

  shenmue_add_event_overlay(PACK_TOPA C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QA00/TOPA.PKF/TOPA.EV3
    ENTRY s2/areas/area_topa.cpp
    SOURCES s2/areas/area_topa.cpp s2/event_utility_v1_dc.c)

  shenmue_add_event_overlay(PACK_TTRP C OPTIONAL TYPE EV3
    REFERENCE SCENE/03/QA00/TTRP.PKF/TTRP.EV3
    ENTRY s2/areas/area_ttrp.cpp
    SOURCES s2/areas/area_ttrp.cpp s2/event_utility_dc.c)

  shenmue_add_event_overlay(PACK_WE00 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WE00/MPK00.PKF/WE00.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_we00.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_WECF C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WECF/MPK00.PKF/WECF.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_wecf.cpp s2/areas/common/src/et_areabase.c s2/areas/area_jukebox.cpp)

  shenmue_add_event_overlay(PACK_WET0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WET0/MPK00.PKF/WET0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_wet0.cpp s2/areas/common/src/et_areabase.c)

  shenmue_add_event_overlay(PACK_WSG1 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WSG1/MPK00.PKF/WSG1.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_wsg1.cpp s2/areas/common/src/et_areabase.c)

endif()

if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_GAMEJAM")
  shenmue_add_event_overlay(PACK_0013 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0013A.PKF/0013.EV3
    ENTRY s2/areas/area_0013.cpp
    SOURCES s2/areas/area_0013.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10)

  shenmue_add_event_overlay(PACK_0014 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0014A.PKS/0014.EV3
    ENTRY s2/areas/area_0014.cpp
    SOURCES s2/areas/area_0014.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10)

  shenmue_add_event_overlay(PACK_0139 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR03/0139A.PKS/0139.EV3
    ENTRY s2/areas/area_0139.cpp
    SOURCES s2/areas/area_0139.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=5)

  shenmue_add_event_overlay(PACK_0140 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/AR02/0140A.PKF/0140.EV3
    ENTRY s2/areas/area_0140.cpp
    SOURCES s2/areas/area_0140.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=10)

  shenmue_add_event_overlay(PACK_0173 C OPTIONAL TYPE EV3
    REFERENCE SCENE/01/WB00/0173A.PKS/0173.EV3
    ENTRY s2/areas/area_0173.cpp
    SOURCES s2/areas/area_0173.cpp s2/event_queued_dc.c s2/event_utility_v1_dc.c
    RUNTIME strcat strcpy
    DEFINES SHENMUE_EVENT_LIBRARY_REVISION=5)

  shenmue_add_event_overlay(PACK_AKA3 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKA3/MPK00.PKS/AKA3.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_aka3.cpp s2/areas/common/src/et_areabase.c
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=5)

  shenmue_add_event_overlay(PACK_AKS0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKS0/MPK00.PKS/AKS0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_aks0.cpp s2/areas/common/src/et_areabase.c s2/event_queued_dc.c s2/areas/common/et_darts.c s2/event_library_tail_dc.c
      s2/areas/common/src/et_npc_load.c s2/areas/area_entities.cpp
    RUNTIME memcpy memset strcat strcpy vsprintf memchr strlen sdivsi3_i4 udivsi3_i4
    DEFINES SHENMUE_AREA_BASE_REVISION=10 SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_EVENT_LIBRARY_REVISION=10 SHENMUE_ET_DARTS_REVISION=1
      SHENMUE_ET_DARTS_RESET_REQUESTS SHENMUE_AREA_ENTITIES_EARLY_AR03
    OPTIONS -fno-math-errno)

  shenmue_add_event_overlay(PACK_AKT0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKT0/MPK00.PKS/AKT0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_akt0.cpp s2/areas/common/src/et_areabase.c s2/areas/area_jukebox.cpp@SHENMUE_JUKEBOX_REVISION=10 s2/areas/common/et_setgamble.c
    RUNTIME movstr_i4 sdivsi3_i4
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=5 SHENMUE_GAMBLING_REVISION=8)

  shenmue_add_event_overlay(PACK_AKT2 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKT2/MPK00.PKS/AKT2.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_akt2.cpp s2/areas/common/src/et_areabase.c s2/areas/common/et_setgamble.c
    RUNTIME movstr_i4 sdivsi3_i4
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10 SHENMUE_GAMBLING_REVISION=10)

  shenmue_add_event_overlay(PACK_AKT3 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKT3/MPK00.PKF/AKT3.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_akt3.cpp s2/areas/common/src/et_areabase.c s2/areas/common/et_setgamble.c
    RUNTIME movstr_i4 sdivsi3_i4
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=5 SHENMUE_GAMBLING_REVISION=5)

  shenmue_add_event_overlay(PACK_AKY0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/AKY0/MPK00.PKS/AKY0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_aky0.cpp s2/areas/common/src/et_areabase.c
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=5)

  shenmue_add_event_overlay(PACK_ARM0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/ARM0/MPK00.PKS/ARM0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_arm0.cpp s2/areas/common/src/et_areabase.c
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10)

  shenmue_add_event_overlay(PACK_ARZ0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/ARZ0/MPK00.PKF/ARZ0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_arz0.cpp s2/areas/common/src/et_areabase.c
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10)

  shenmue_add_event_overlay(PACK_WE00 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WE00/MPK00.PKF/WE00.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_we00.cpp s2/areas/common/src/et_areabase.c
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=15)

  shenmue_add_event_overlay(PACK_WECF C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WECF/MPK00.PKS/WECF.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_wecf.cpp s2/areas/common/src/et_areabase.c s2/areas/area_jukebox.cpp@SHENMUE_JUKEBOX_REVISION=10
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10)

  shenmue_add_event_overlay(PACK_WET0 C OPTIONAL TYPE EV1
    REFERENCE SCENE/01/WET0/MPK00.PKF/WET0.EV1
    ENTRY s2/areas/common/src/et_areabase.c
    SOURCES s2/areas/area_wet0.cpp s2/areas/common/src/et_areabase.c
    DEFINES SHENMUE_AREA_BASE_CLOCK_NIGHT SHENMUE_AREA_BASE_REVISION=10)

endif()

get_property(event_targets DIRECTORY PROPERTY SHENMUE_EVENT_TARGETS)
if(SHENMUE_DC_EVENT_TARGET STREQUAL "S2_JP_GAMEJAM" AND NOT event_targets)
  message(FATAL_ERROR "No GameJam event overlay references were found in SHENMUE_DC_EVENT_REFERENCE_DIR")
endif()
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
