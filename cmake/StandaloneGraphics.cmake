if(SHENMUE_GFX_DXVK_NATIVE)
  find_path(SHENMUE_VKD3D_SHADER_INCLUDE_DIR vkd3d_shader.h PATH_SUFFIXES vkd3d REQUIRED)
  find_library(SHENMUE_VKD3D_SHADER_LIBRARY NAMES vkd3d-shader REQUIRED)
  add_executable(shenmue_shader_compiler tools/gfx/compile-shader.c)
  target_include_directories(shenmue_shader_compiler PRIVATE "${SHENMUE_VKD3D_SHADER_INCLUDE_DIR}")
  target_link_libraries(shenmue_shader_compiler PRIVATE "${SHENMUE_VKD3D_SHADER_LIBRARY}")
  target_compile_features(shenmue_shader_compiler PRIVATE c_std_99)
else()
  get_filename_component(windows_kits "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE)
  file(GLOB sdk_directories LIST_DIRECTORIES TRUE "${windows_kits}/bin/10.*")
  list(SORT sdk_directories COMPARE NATURAL ORDER DESCENDING)
  set(shader_compiler_directories)
  foreach(directory IN LISTS sdk_directories)
    list(APPEND shader_compiler_directories "${directory}/x64")
  endforeach()
  find_program(SHENMUE_FXC fxc HINTS "$ENV{WindowsSdkVerBinPath}/x64" ${shader_compiler_directories} REQUIRED)
endif()

set(shader_directory "${PROJECT_SOURCE_DIR}/src/gfx/shaders")
set(shader_output_directory "${CMAKE_CURRENT_BINARY_DIR}/generated/gfx")
file(GLOB_RECURSE shader_sources CONFIGURE_DEPENDS "${shader_directory}/*.hlsl")
file(GLOB_RECURSE shader_includes CONFIGURE_DEPENDS "${shader_directory}/*.hlsli")
set(shader_headers)
set(shader_declarations)
set(shader_entries)
foreach(source IN LISTS shader_sources)
  file(RELATIVE_PATH name "${shader_directory}" "${source}")
  string(MAKE_C_IDENTIFIER "gfx_${name}" identifier)
  if(name MATCHES "_vs\\.hlsl$")
    set(profile vs_5_0)
    set(stage 0)
  elseif(name MATCHES "_ps\\.hlsl$")
    set(profile ps_5_0)
    set(stage 1)
  else()
    message(FATAL_ERROR "Unknown shader stage: ${name}")
  endif()
  if(name MATCHES "^engine/")
    string(REGEX REPLACE "^engine/" "/engine/assets/shaders/" path "${name}")
  else()
    set(path "/shaders/${name}")
  endif()
  set(header "${shader_output_directory}/${identifier}.h")
  if(SHENMUE_GFX_DXVK_NATIVE)
    add_custom_command(OUTPUT "${header}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${shader_output_directory}"
      COMMAND shenmue_shader_compiler "${source}" "${header}" "${identifier}" "${profile}" "${shader_directory}"
      DEPENDS "${source}" ${shader_includes} shenmue_shader_compiler
      VERBATIM)
  else()
    add_custom_command(OUTPUT "${header}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${shader_output_directory}"
      COMMAND "${SHENMUE_FXC}" /nologo /T "${profile}" /E main /O3 /Gis /D SHENMUE_GFX_STANDALONE=1
        /I "${shader_directory}" /Vn "${identifier}" /Fh "${header}" "${source}"
      DEPENDS "${source}" ${shader_includes}
      VERBATIM)
  endif()
  list(APPEND shader_headers "${header}")
  string(APPEND shader_declarations "#include \"${identifier}.h\"\n")
  string(APPEND shader_entries "    {\"${path}\", ${stage}, ${identifier}, sizeof(${identifier})},\n")
endforeach()
set(shader_aliases
  "engine/simple_ps.hlsl=engine/debugdraw_ps.hlsl"
  "engine/simple_vs.hlsl=engine/debugdraw_vs.hlsl"
  "engine/texturedquad_gammacorrect_vs.hlsl=engine/texturedquad_vs.hlsl"
  "contrast_vs.hlsl=abuffer_clear_vs.hlsl"
  "caf_ps.hlsl=blur_ps.hlsl"
  "caf_screen_ps.hlsl=blur_ps.hlsl"
  "gray_ps.hlsl=blur_ps.hlsl"
  "msf_ps.hlsl=blur_ps.hlsl"
  "fvf/fvf_pntds_ps.hlsl=fvf/fvf_pnt_ps.hlsl")
foreach(alias IN LISTS shader_aliases)
  string(REPLACE "=" ";" names "${alias}")
  list(GET names 0 name)
  list(GET names 1 source)
  string(MAKE_C_IDENTIFIER "gfx_${source}" identifier)
  if(name MATCHES "_vs\\.hlsl$")
    set(stage 0)
  else()
    set(stage 1)
  endif()
  if(name MATCHES "^engine/")
    string(REGEX REPLACE "^engine/" "/engine/assets/shaders/" path "${name}")
  else()
    set(path "/shaders/${name}")
  endif()
  string(APPEND shader_entries "    {\"${path}\", ${stage}, ${identifier}, sizeof(${identifier})},\n")
endforeach()
file(GENERATE OUTPUT "${shader_output_directory}/gfx_shaders.h"
  CONTENT "${shader_declarations}\nstatic const GfxShader gfxShaders[] = {\n${shader_entries}};\n")

add_library(shenmue_gfx_standalone STATIC src/gfx/standalone.c ${shader_headers})
target_compile_features(shenmue_gfx_standalone PRIVATE c_std_99)
target_compile_definitions(shenmue_gfx_standalone PRIVATE SHENMUE_GFX_STANDALONE=1)
target_include_directories(shenmue_gfx_standalone PRIVATE src "${shader_output_directory}")
if(MSVC)
  target_compile_options(shenmue_gfx_standalone PRIVATE /W4)
else()
  target_compile_options(shenmue_gfx_standalone PRIVATE -Wall -Wextra -Wpedantic)
endif()
