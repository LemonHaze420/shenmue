include("${CMAKE_CURRENT_LIST_DIR}/Dreamcast.cmake")

set(SHENMUE_DC_GCC2952 ON)
set(SHENMUE_DC_TARGET "S2_JP_GAMEJAM" CACHE STRING "Recovered Dreamcast reference target")
if(NOT SHENMUE_DC_TARGET MATCHES "^S2_")
  message(FATAL_ERROR "The pinned GCC 2.95.2 toolchain requires an S2 Dreamcast target")
endif()
set(SHENMUE_DC_GCC2952_DIR "${_shenmue_source_dir}/build/s2-dc-compiler/event-gcc/src/shenmue_event_gcc-build/gcc"
  CACHE PATH "Pinned S2 resident compiler; configure build/s2-dc-compiler with SHENMUE_BUILD_S2_DC_COMPILER=ON first")
set(SHENMUE_DC_GXX2952_SOURCE "${_shenmue_source_dir}/build/s2-dc-compiler/event-gcc/src/shenmue_event_gxx_source"
  CACHE PATH "Pinned GCC 2.95.2 C++ library source directory")
set(SHENMUE_DC_NEWLIB_SOURCE "${_shenmue_source_dir}/build/s2-dc-compiler/event-newlib/src/shenmue_event_newlib/newlib/libc"
  CACHE PATH "Pinned newlib 1.8.1 source directory")

foreach(_driver xgcc g++ cpp cc1 cc1plus)
  if(NOT EXISTS "${SHENMUE_DC_GCC2952_DIR}/${_driver}")
    message(FATAL_ERROR "Configure build/s2-dc-compiler with -DSHENMUE_BUILD_S2_DC_COMPILER=ON, then build shenmue_event_gcc and shenmue_event_newlib; missing ${SHENMUE_DC_GCC2952_DIR}/${_driver}")
  endif()
endforeach()

set(_shenmue_gcc2952_prefix "-B${SHENMUE_DC_GCC2952_DIR}/ -B${SHENMUE_DC_TOOLCHAIN_ROOT}/sh4-elf/bin/")
set(CMAKE_C_COMPILER "${SHENMUE_DC_GCC2952_DIR}/xgcc")
set(CMAKE_CXX_COMPILER "${SHENMUE_DC_GCC2952_DIR}/g++")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")
set(CMAKE_C_FLAGS_INIT "${_shenmue_gcc2952_prefix} -O1 -ml -m4-single-only")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS_INIT "${_shenmue_gcc2952_prefix} -ml -m4-single-only")
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES "${SHENMUE_DC_GCC2952_DIR}/include;${SHENMUE_DC_NEWLIB_SOURCE}/include")
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/gcc2952-runtime"
  "${SHENMUE_DC_GXX2952_SOURCE}/libstdc++" "${SHENMUE_DC_GXX2952_SOURCE}/libstdc++/stl"
  "${SHENMUE_DC_GXX2952_SOURCE}/libio" ${CMAKE_C_STANDARD_INCLUDE_DIRECTORIES})
set(CMAKE_DEPENDS_USE_COMPILER FALSE)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/../Gcc2952Rules.cmake")
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES SHENMUE_DC_GCC2952_DIR SHENMUE_DC_GXX2952_SOURCE SHENMUE_DC_NEWLIB_SOURCE
  SHENMUE_DC_TOOLCHAIN_ROOT SHENMUE_DC_TARGET)
