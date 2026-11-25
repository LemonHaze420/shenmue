get_filename_component(_gcc2952_build "${SHENMUE_DC_GCC2952_DIR}" DIRECTORY)
get_filename_component(_gcc2952_prefix "${_gcc2952_build}" DIRECTORY)
set(_gcc2952_source "${_gcc2952_prefix}/shenmue_event_gcc/gcc")
if(NOT EXISTS "${_gcc2952_source}/libgcc2.c")
  message(FATAL_ERROR "Missing pinned GCC runtime source: ${_gcc2952_source}/libgcc2.c")
endif()

set(_gcc2952_runtime_dir "${CMAKE_CURRENT_BINARY_DIR}/gcc2952-runtime")
file(GENERATE OUTPUT "${_gcc2952_runtime_dir}/cstdint" CONTENT "#pragma once\n#include \"common/legacy_types.hpp\"\n")
if(NOT EXISTS "${_gcc2952_runtime_dir}/_G_config.h")
  file(MAKE_DIRECTORY "${_gcc2952_runtime_dir}")
  set(_gcc2952_config_flags "${_shenmue_gcc2952_prefix} -O1 -ml -m4-single-only -I${SHENMUE_DC_GCC2952_DIR}/include -I${SHENMUE_DC_NEWLIB_SOURCE}/include")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E env "CC=${CMAKE_C_COMPILER} ${_gcc2952_config_flags}"
    "CXX=${CMAKE_CXX_COMPILER} ${_gcc2952_config_flags}" "CONFIG_NM=${SHENMUE_DC_TOOLCHAIN_ROOT}/bin/sh4-elf-nm"
    sh "${SHENMUE_DC_GXX2952_SOURCE}/libio/gen-params" LIB_VERSION=2.8.0
    WORKING_DIRECTORY "${_gcc2952_runtime_dir}" OUTPUT_FILE "${_gcc2952_runtime_dir}/_G_config.h.tmp"
    ERROR_VARIABLE _gcc2952_config_error RESULT_VARIABLE _gcc2952_config_result)
  if(NOT _gcc2952_config_result EQUAL 0)
    message(FATAL_ERROR "Cannot configure the pinned C++ library: ${_gcc2952_config_error}")
  endif()
  file(RENAME "${_gcc2952_runtime_dir}/_G_config.h.tmp" "${_gcc2952_runtime_dir}/_G_config.h")
endif()

set(_gcc2952_objects)
foreach(_member _muldi3 _divdi3 _moddi3 _udivdi3 _umoddi3 _negdi2 _lshrdi3 _ashldi3 _ashrdi3 _ffsdi2
    _udiv_w_sdiv _udivmoddi4 _cmpdi2 _ucmpdi2 _floatdidf _floatdisf _fixunsdfsi _fixunssfsi _fixunsdfdi _fixdfdi
    _fixunssfdi _fixsfdi _fixxfdi _fixunsxfdi _floatdixf _fixunsxfsi _fixtfdi _fixunstfdi _floatditf
    __gcc_bcmp _varargs __dummy _eprintf _bb _shtab _clear_cache _trampoline __main _exit _ctors _pure)
  set(_target "shenmue_dc_gcc2952${_member}")
  add_library(${_target} OBJECT "${_gcc2952_source}/libgcc2.c")
  target_compile_definitions(${_target} PRIVATE IN_GCC IN_LIBGCC2 __GCC_FLOAT_NOT_NEEDED inhibit_libc "L${_member}")
  target_include_directories(${_target} PRIVATE "${SHENMUE_DC_GCC2952_DIR}" "${_gcc2952_source}"
    "${_gcc2952_source}/config" "${_gcc2952_source}/../include")
  target_compile_options(${_target} PRIVATE -O2)
  list(APPEND _gcc2952_objects "$<TARGET_OBJECTS:${_target}>")
endforeach()

foreach(_precision sf df)
  set(_target "shenmue_dc_gcc2952_${_precision}")
  add_library(${_target} OBJECT "${_gcc2952_source}/config/fp-bit.c")
  target_compile_definitions(${_target} PRIVATE FLOAT_BIT_ORDER_MISMATCH)
  if(_precision STREQUAL "sf")
    target_compile_definitions(${_target} PRIVATE FLOAT)
  endif()
  target_compile_options(${_target} PRIVATE -O2)
  list(APPEND _gcc2952_objects "$<TARGET_OBJECTS:${_target}>")
endforeach()

foreach(_member _ashiftrt _ashiftrt_n _ashiftlt _lshiftrt _movstr _movstr_i4 _mulsi3 _sdivsi3 _udivsi3 _set_fpscr)
  set(_target "shenmue_dc_gcc2952${_member}")
  set(_source "${CMAKE_CURRENT_BINARY_DIR}/gcc2952-runtime/${_member}.S")
  file(GENERATE OUTPUT "${_source}" CONTENT "#define L${_member}\n#include \"${_gcc2952_source}/config/sh/lib1funcs.asm\"\n")
  add_library(${_target} OBJECT "${_source}")
  target_compile_options(${_target} PRIVATE -O2)
  list(APPEND _gcc2952_objects "$<TARGET_OBJECTS:${_target}>")
endforeach()

foreach(_member sdivsi3_i4 udivsi3_i4)
  set(_target "shenmue_dc_gcc2952_${_member}")
  add_library(${_target} OBJECT "${PROJECT_SOURCE_DIR}/src/s2/events/runtime/division_fpu.S")
  target_compile_definitions(${_target} PRIVATE "L_${_member}")
  list(APPEND _gcc2952_objects "$<TARGET_OBJECTS:${_target}>")
endforeach()


add_library(shenmue_dc_newlib_support OBJECT
  "${SHENMUE_DC_NEWLIB_SOURCE}/ctype/ctype_.c"
  "${SHENMUE_DC_NEWLIB_SOURCE}/errno/errno.c"
  "${SHENMUE_DC_NEWLIB_SOURCE}/reent/impure.c"
  "${SHENMUE_DC_NEWLIB_SOURCE}/../libm/math/sf_isnan.c"
  "${SHENMUE_DC_NEWLIB_SOURCE}/../libm/math/sf_isinf.c")
foreach(_member sprintf vsprintf printf vprintf fprintf vfprintf findfp stdio fflush fwalk fvwrite wsetup makebuf)
  target_sources(shenmue_dc_newlib_support PRIVATE "${SHENMUE_DC_NEWLIB_SOURCE}/stdio/${_member}.c")
endforeach()
foreach(_member dtoa mprec mbtowc_r)
  target_sources(shenmue_dc_newlib_support PRIVATE "${SHENMUE_DC_NEWLIB_SOURCE}/stdlib/${_member}.c")
endforeach()
foreach(_member readr writer lseekr closer fstatr)
  target_sources(shenmue_dc_newlib_support PRIVATE "${SHENMUE_DC_NEWLIB_SOURCE}/reent/${_member}.c")
endforeach()
target_sources(shenmue_dc_newlib_support PRIVATE "${SHENMUE_DC_NEWLIB_SOURCE}/locale/locale.c")
target_compile_definitions(shenmue_dc_newlib_support PRIVATE _mult=shenmue_newlib_mult)
target_compile_options(shenmue_dc_newlib_support PRIVATE -O2)
list(APPEND _gcc2952_objects "$<TARGET_OBJECTS:shenmue_dc_newlib_support>")

add_library(shenmue_dc_gcc2952 STATIC ${_gcc2952_objects})
set_target_properties(shenmue_dc_gcc2952 PROPERTIES LINKER_LANGUAGE C)
