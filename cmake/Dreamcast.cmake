if(NOT CMAKE_SYSTEM_NAME STREQUAL "Generic" OR NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "SH4")
  message(FATAL_ERROR "Select cmake/toolchains/DreamcastS2.cmake in a separate build directory")
endif()

set(SHENMUE_DC_TARGET "S2_JP_GAMEJAM" CACHE STRING "Dreamcast resident executable")
set_property(CACHE SHENMUE_DC_TARGET PROPERTY STRINGS S2_JP_GAMEJAM)
if(NOT SHENMUE_DC_TARGET STREQUAL "S2_JP_GAMEJAM")
  message(FATAL_ERROR "Unsupported Dreamcast resident target: ${SHENMUE_DC_TARGET}")
endif()
if(NOT SHENMUE_DC_GCC2952)
  message(FATAL_ERROR "S2_JP_GAMEJAM requires cmake/toolchains/DreamcastS2.cmake")
endif()
set(_shenmue_dc_definition SHENMUE_TARGET_S2_DC_JP_GAMEJAM=1)

option(SHEN_AUTO_TEST_ENABLED "Capture a fixed Press Start presentation" OFF)
if(SHEN_AUTO_TEST_ENABLED)
  set_property(SOURCE src/s2/title_control_dc.cpp src/s2/title_sprite_dc.cpp src/platform/dc/sdk.cpp
    APPEND PROPERTY COMPILE_DEFINITIONS SHEN_AUTO_TEST_ENABLED=1)
endif()

find_package(Git QUIET)
set(SHENMUE_COMMIT_HASH "" CACHE STRING "Source commit for builds without Git metadata")
add_custom_target(shenmue_build_info
  COMMAND "${CMAKE_COMMAND}" "-DSHENMUE_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
    "-DSHENMUE_GENERATED_DIR=${CMAKE_CURRENT_BINARY_DIR}/generated"
    "-DSHENMUE_COMMIT_HASH=${SHENMUE_COMMIT_HASH}" "-DGIT_EXECUTABLE=${GIT_EXECUTABLE}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommitHash.cmake"
  BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/generated/shenmue_internal.h"
  VERBATIM
)

set(_dc_zlib_dir "${PROJECT_SOURCE_DIR}/reference/snd/aosdk/zlib")
if(SHENMUE_DC_GCC2952)
  set(_dc_zlib_input "${_dc_zlib_dir}")
  set(_dc_zlib_dir "${CMAKE_CURRENT_BINARY_DIR}/zlib")
  file(GLOB _dc_zlib_files CONFIGURE_DEPENDS "${_dc_zlib_input}/*.c" "${_dc_zlib_input}/*.h")
  foreach(_dc_zlib_file IN LISTS _dc_zlib_files)
    get_filename_component(_dc_zlib_name "${_dc_zlib_file}" NAME)
    configure_file("${_dc_zlib_file}" "${_dc_zlib_dir}/${_dc_zlib_name}" @ONLY NEWLINE_STYLE UNIX)
  endforeach()
endif()
add_library(shenmue_dc_zlib STATIC
  "${_dc_zlib_dir}/adler32.c" "${_dc_zlib_dir}/crc32.c" "${_dc_zlib_dir}/inflate.c"
  "${_dc_zlib_dir}/inffast.c" "${_dc_zlib_dir}/inftrees.c" "${_dc_zlib_dir}/zutil.c")
target_include_directories(shenmue_dc_zlib PUBLIC "${_dc_zlib_dir}")

include("${CMAKE_CURRENT_LIST_DIR}/DreamcastS2.cmake")

option(SHENMUE_DC_BUILD_HASH "Draw the source commit with the Dreamcast debug font" OFF)
foreach(_dc_target ${_shenmue_dc_core} ${_shenmue_dc_executable})
  add_dependencies(${_dc_target} shenmue_build_info)
  target_include_directories(${_dc_target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
  target_compile_definitions(${_dc_target} PRIVATE SHENMUE_DC_BUILD_HASH=$<BOOL:${SHENMUE_DC_BUILD_HASH}>)
endforeach()

target_sources(${_shenmue_dc_core} PRIVATE src/platform/dc/stack_spline.cpp)
target_link_libraries(${_shenmue_dc_executable} PRIVATE shenmue_dc_zlib)

set(SHENMUE_DC_SDK_LIBRARY_DIR "" CACHE PATH "External Katana SDK GNU archive directory")
if(NOT SHENMUE_DC_SDK_LIBRARY_DIR)
  message(FATAL_ERROR "Set SHENMUE_DC_SDK_LIBRARY_DIR to the external Katana GNU archive directory")
endif()
include("${CMAKE_CURRENT_LIST_DIR}/Gcc2952Runtime.cmake")
set(_dc_runtime_libraries shenmue_dc_gcc2952 "${SHENMUE_DC_TOOLCHAIN_ROOT}/sh4-elf/lib/libnosys.a")
set(_sdk_archives)
set(_sdk_runtime_aliases)
foreach(_runtime_function printf fprintf sprintf snprintf vprintf vfprintf vsprintf vsnprintf scanf fscanf sscanf vscanf vfscanf vsscanf
    acos acosf asin asinf atan atanf atan2 atan2f ceil ceilf cos cosf cosh coshf exp expf fabs fabsf floor floorf fmod fmodf
    frexp frexpf ldexp ldexpf log logf log10 log10f modf modff pow powf sin sinf sinh sinhf sqrt sqrtf tan tanf tanh tanhf)
  if(NOT SHENMUE_DC_GCC2952 OR _runtime_function MATCHES "^(v?(f|s)?printf|snprintf|vsnprintf|v?(f|s)?scanf)$")
    list(APPEND _sdk_runtime_aliases --redefine-sym "_${_runtime_function}=_shenmue_sdk_${_runtime_function}")
  endif()
endforeach()
set(_sdk_runtime_dir "${CMAKE_CURRENT_BINARY_DIR}/sdk-runtime")
file(MAKE_DIRECTORY "${_sdk_runtime_dir}")
foreach(_sdk_library ninja shinobi cri_adxs cri_san cri_fsel sg_mw sg_sd sh4nlfzn)
  set(_sdk_archive "${SHENMUE_DC_SDK_LIBRARY_DIR}/lib${_sdk_library}.a")
  if(NOT EXISTS "${_sdk_archive}")
    message(FATAL_ERROR "Missing Katana GNU library: ${_sdk_archive}")
  endif()
  set(_sdk_runtime_archive "${_sdk_runtime_dir}/lib${_sdk_library}.a")
  add_custom_command(OUTPUT "${_sdk_runtime_archive}"
    COMMAND "${CMAKE_OBJCOPY}" ${_sdk_runtime_aliases} "${_sdk_archive}" "${_sdk_runtime_archive}"
    COMMAND "${CMAKE_RANLIB}" "${_sdk_runtime_archive}"
    DEPENDS "${_sdk_archive}"
    VERBATIM)
  list(APPEND _sdk_archives "${_sdk_runtime_archive}")
endforeach()
add_custom_target(shenmue_dc_sdk DEPENDS ${_sdk_archives})
add_dependencies(${_shenmue_dc_executable} shenmue_dc_sdk)
target_include_directories(${_shenmue_dc_core} PUBLIC "${SHENMUE_DC_SDK_LIBRARY_DIR}/../Include")
target_link_libraries(${_shenmue_dc_executable} PRIVATE "${SHENMUE_DC_SDK_LIBRARY_DIR}/syscalls.obj" -Wl,--start-group ${_sdk_archives} ${_dc_runtime_libraries} -Wl,--end-group)

if(SHENMUE_DC_TARGET STREQUAL "S2_JP_GAMEJAM")
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SHENMUE_DC_SPLAT_ROOT "" CACHE PATH "Optional splat64 source checkout")
set(_s2_config_dir "${PROJECT_SOURCE_DIR}/config/S2_JP_GAMEJAM")
set(_s2_link_tool "${PROJECT_SOURCE_DIR}/tools/dc/configure.py")
set(_s2_sections "${CMAKE_CURRENT_BINARY_DIR}/s2-sections.ld")
set(_s2_splat_args)
if(SHENMUE_DC_SPLAT_ROOT)
  list(APPEND _s2_splat_args --splat-root "${SHENMUE_DC_SPLAT_ROOT}")
endif()
add_custom_command(TARGET shenmue2_dc PRE_LINK
  COMMAND "${Python3_EXECUTABLE}" "${_s2_link_tool}"
    --config "${_s2_config_dir}/config.yml" --build-dir "${CMAKE_CURRENT_BINARY_DIR}"
    --sdk-dir "${_sdk_runtime_dir}" --syscalls "${SHENMUE_DC_SDK_LIBRARY_DIR}/syscalls.obj"
    --library "$<TARGET_FILE:shenmue_dc_zlib>" --library "$<TARGET_FILE:shenmue_dc_gcc2952>"
    --library "${SHENMUE_DC_TOOLCHAIN_ROOT}/sh4-elf/lib/libnosys.a" ${_s2_splat_args}
  BYPRODUCTS "${_s2_sections}"
  VERBATIM)
set_property(TARGET shenmue2_dc APPEND PROPERTY LINK_DEPENDS
  ${_sdk_archives} "$<TARGET_FILE:shenmue_dc_zlib>" "$<TARGET_FILE:shenmue_dc_gcc2952>"
    "${SHENMUE_DC_TOOLCHAIN_ROOT}/sh4-elf/lib/libnosys.a" "${SHENMUE_DC_SDK_LIBRARY_DIR}/syscalls.obj"
    "${_s2_config_dir}/config.yml" "${_s2_config_dir}/objects.txt" "${_s2_config_dir}/ctors.txt"
    "${_s2_config_dir}/function_order.txt" "${_s2_config_dir}/data_order.txt" "${_s2_config_dir}/symbols.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" "${_s2_link_tool}"
    "${PROJECT_SOURCE_DIR}/tools/dc/splat_ext/object.py"
)
add_dependencies(shenmue2_dc shenmue2_dc_core shenmue_dc_sdk shenmue_dc_zlib shenmue_dc_gcc2952)
add_custom_target(shenmue2_dc_link)
add_dependencies(shenmue2_dc_link shenmue2_dc)

set(SHENMUE_DC_MATCH_REFERENCE "" CACHE FILEPATH "Unmodified GameJam GDI or 1ST_READ.BIN for matching")
if(SHENMUE_DC_MATCH_REFERENCE)
  if(NOT EXISTS "${SHENMUE_DC_MATCH_REFERENCE}")
    message(FATAL_ERROR "Missing reference: ${SHENMUE_DC_MATCH_REFERENCE}")
  endif()
  find_program(SHENMUE_DC_OBJDIFF objdiff-cli HINTS "${CMAKE_CURRENT_BINARY_DIR}/tools" REQUIRED)
  add_custom_target(shenmue2_dc_match
    COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/dc/match_image.py"
      --config "${_s2_config_dir}/config.yml" --reference "${SHENMUE_DC_MATCH_REFERENCE}"
      --elf "$<TARGET_FILE:shenmue2_dc>" --candidate "${CMAKE_CURRENT_BINARY_DIR}/1ST_READ.BIN"
      --output-dir "${CMAKE_CURRENT_BINARY_DIR}/matching" --objdiff "${SHENMUE_DC_OBJDIFF}"
    DEPENDS shenmue2_dc
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/matching/objdiff.json"
      "${CMAKE_CURRENT_BINARY_DIR}/matching/report.json" "${CMAKE_CURRENT_BINARY_DIR}/matching/image.json"
    VERBATIM)
endif()
endif()
