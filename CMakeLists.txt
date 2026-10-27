cmake_minimum_required(VERSION 3.22)

project(shenmue_recovery LANGUAGES CXX)

option(SHENMUE_STEAM "Build Steam integration modules" OFF)
if(SHENMUE_STEAM AND WIN32 AND DEFINED VCPKG_TARGET_TRIPLET AND NOT VCPKG_TARGET_TRIPLET MATCHES "static")
  message(FATAL_ERROR "Steam ASIs require static third-party libraries; select a static vcpkg triplet")
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

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
find_package(jsoncpp CONFIG QUIET)

find_path(SHENMUE_STB_INCLUDE_DIR stb_image.h PATH_SUFFIXES stb)
if(NOT SHENMUE_STB_INCLUDE_DIR)
  include(FetchContent)
  FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG 2c980bb59875b0d32144a71867fbdebb2f77cd20
    GIT_SHALLOW FALSE
  )
  FetchContent_MakeAvailable(stb)
  set(SHENMUE_STB_INCLUDE_DIR "${stb_SOURCE_DIR}")
endif()

if(WIN32)
  find_package(directxtex CONFIG REQUIRED)
endif()

if(NOT TARGET JsonCpp::JsonCpp)
  include(FetchContent)
  set(JSONCPP_WITH_TESTS OFF CACHE BOOL "" FORCE)
  set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "" FORCE)
  set(JSONCPP_WITH_EXAMPLE OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG 1.9.6
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(jsoncpp)
endif()

if(TARGET JsonCpp::JsonCpp)
  set(SHENMUE_JSONCPP_TARGET JsonCpp::JsonCpp)
elseif(TARGET jsoncpp_static)
  set(SHENMUE_JSONCPP_TARGET jsoncpp_static)
elseif(TARGET jsoncpp_lib)
  set(SHENMUE_JSONCPP_TARGET jsoncpp_lib)
else()
  message(FATAL_ERROR "JsonCpp was found but did not provide a usable target")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)
if(WIN32)
  add_compile_definitions(NOMINMAX)
endif()
option(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS "Build the static Shenmue II map tile seam guardband" ON)
option(SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX "Scale NPC steering and vehicle movement in 60 Hz mode" ON)
option(SHENMUE_ENHANCEMENT_S1_SKY_FIX "Correct the Shenmue I sky yaw scale" ON)
option(SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE "Show the targeted enemy health gauge in Shenmue I fights" ON)
option(SHENMUE_ENHANCEMENT_S1_CASSETTES_ANYWHERE "Keep menu cassette playback active while roaming" ON)
option(SHENMUE_RT_ARCADE "Run the original arcade cores on Shenmue I cabinet screens in realtime" OFF)
option(SHENMUE_LOAD_HANGON_DLL "Resolve the original Hang-On DLL explicitly instead of native imports" OFF)


add_subdirectory(libs/ShenmueDK)

add_library(shenmue_common STATIC
  src/common/archive_data.cpp
  src/common/math.cpp
  src/common/matrix_stack.cpp
  src/platform/performance_counter.cpp
)
target_compile_features(shenmue_common PUBLIC cxx_std_20)
target_include_directories(shenmue_common PUBLIC src)
target_include_directories(shenmue_common PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/generated")
add_dependencies(shenmue_common shenmue_build_info)
target_link_libraries(shenmue_common PUBLIC ${SHENMUE_JSONCPP_TARGET} ShenmueDKStatic)
shenmue_set_project_warnings(shenmue_common)


add_library(shenmue_d3t STATIC
  src/third_party/cjson.cpp
  src/third_party/stb_image.cpp
  src/platform/file.cpp
  src/d3t/archive_handles.cpp
  src/d3t/archives.cpp
  src/d3t/asset_remap.cpp
  src/d3t/buffer.cpp
  src/d3t/compression.cpp
  src/d3t/dx11_renderer.cpp
  src/d3t/fvf.cpp
  src/d3t/material.cpp
  src/d3t/mip_streaming.cpp
  src/d3t/model.cpp
  src/d3t/image_texture_loader.cpp
  src/d3t/pipeline_state.cpp
  src/d3t/pvr_decoder.cpp
  src/d3t/render_handles.cpp
  src/d3t/shader.cpp
  src/d3t/shader_load.cpp
  src/d3t/file_state.cpp
  src/d3t/threads.cpp
  src/d3t/texture.cpp
  src/d3t/light_tiles.cpp
  src/d3t/ui_definition.cpp
  src/d3t/ui_nuklear.cpp
  src/d3t/ui_renderer.cpp
  src/d3t/ui_view.cpp
)
target_include_directories(shenmue_d3t PRIVATE "${SHENMUE_STB_INCLUDE_DIR}")
target_compile_features(shenmue_d3t PUBLIC cxx_std_20)
# Classic cJSON (MIT, third_party/cjson), the parser both games' D3T .ui and settings readers use.
target_include_directories(shenmue_d3t PUBLIC src third_party/cjson)
# Nuklear v1.156 with the games' local changes (public domain, third_party/nuklear), configured by src/d3t/ui_nuklear.hpp.
target_include_directories(shenmue_d3t PUBLIC third_party/nuklear)
set_source_files_properties(src/d3t/ui_nuklear.cpp PROPERTIES
  COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/W0>;$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>"
)
target_link_libraries(shenmue_d3t PUBLIC shenmue_common)
if(WIN32)
  target_link_libraries(shenmue_d3t PUBLIC d3d11 dxgi windowscodecs)
  if(TARGET Microsoft::DirectXTex)
    target_link_libraries(shenmue_d3t PUBLIC Microsoft::DirectXTex)
    target_compile_definitions(shenmue_d3t PUBLIC SHENMUE_HAS_DIRECTXTEX=1)
  endif()
endif()
shenmue_set_project_warnings(shenmue_d3t)

# Dear ImGui 1.90.6 (MIT, third_party/imgui) for the S1 debug view.
if(WIN32)
  add_library(shenmue_imgui STATIC
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/backends/imgui_impl_dx11.cpp
    third_party/imgui/backends/imgui_impl_win32.cpp
  )
  target_include_directories(shenmue_imgui PUBLIC third_party/imgui third_party/imgui/backends)
  target_link_libraries(shenmue_imgui PUBLIC d3d11 dxgi d3dcompiler dwmapi)
  target_compile_options(shenmue_imgui PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/W0>")
endif()

add_library(shenmue1_game STATIC
  src/d3t/content.cpp
  src/d3t/model_load.cpp
  src/d3t/ui_runtime.cpp
  src/d3t/ui_animation.cpp
  src/d3t/ui_input.cpp
  src/d3t/ui_layout.cpp
  src/d3t/ui_lists.cpp
  src/d3t/ui_panels.cpp
  src/d3t/ui_render.cpp
  src/d3t/ui_text.cpp
  src/d3t/ui_views.cpp
  src/d3t/ui_widgets.cpp
  src/d3t/model_paths.cpp
  src/s1/save_format.cpp
  src/s1/task/btpk.cpp
  src/s1/task/atck.cpp
  src/s1/task/cstc.cpp
  src/s1/task/cuff.cpp
  src/s1/task/drwp.cpp
  src/s1/task/drwh.cpp
  src/s1/task/drwo.cpp
  src/s1/task/dwmz.cpp
  src/s1/task/edtk.cpp
  src/s1/task/fbgg.cpp
  src/s1/task/fork.cpp
  src/s1/task/gull.cpp
  src/s1/task/hato.cpp
  src/s1/task/kuti.cpp
  src/s1/task/lamp.cpp
  src/s1/task/mbst.cpp
  src/s1/task/metk.cpp
  src/s1/task/mitk.cpp
  src/s1/task/mmtk.cpp
  src/s1/task/motf.cpp
  src/s1/task/obje.cpp
  src/s1/task/tmlt.cpp
  src/s1/task/tmex.cpp
  src/s1/task/cdtt.cpp
  src/s1/task/warn.cpp
  src/s1/adfs.cpp
  src/s1/audio_stream.cpp
  src/s1/task/sond.cpp
  src/s1/boot.cpp
  src/s1/task/colb.cpp
  src/s1/task/came.cpp
  src/s1/task/conv.cpp
  src/s1/task/cach.cpp
  src/s1/task/esop.cpp
  src/s1/task/updb.cpp
  src/s1/task/char.cpp
  src/s1/task/char_hand.cpp
  src/s1/task/char_cloth.cpp
  src/s1/task/char_osage.cpp
  src/s1/task/char_navigation.cpp
  src/s1/task/char_image.cpp
  src/s1/task/telm.cpp
  src/s1/task/gcpn.cpp
  src/s1/chrd.cpp
  src/s1/collision.cpp
  src/s1/navigation.cpp
  src/s1/cycle_stack.cpp
  src/s1/task/cycle_man.cpp
    src/s1/task/mobj.cpp
  src/s1/task/cycle_man_commands.cpp
  src/s1/task/cycle_man_door.cpp
  src/s1/task/cycle_man_motion.cpp
  src/s1/task/cycle_man_objects.cpp
  src/s1/task/cycle_man_render.cpp
  src/s1/task/cycle_man_schedule.cpp
  src/s1/task/cycle_man_talk.cpp
  src/s1/task/cycle_man_umbrella.cpp
  src/s1/task/cycle_man_vehicle.cpp
  src/s1/task/cycle_man_steering.cpp
  src/s1/task/cycle_man_mnrb.cpp
  src/s1/d3t/render_offsets.cpp
  src/s1/d3t/renderer.cpp
  src/s1/d3t/settings.cpp
  src/s1/d3t/d3t_shader_load.cpp
  src/s1/d3t/scratch_memory.cpp
  src/s1/task/dswk.cpp
  src/s1/dynamic_map.cpp
  src/s1/task/effe.cpp
  src/s1/task/effe_weather.cpp
  src/s1/task/effe_snow.cpp
  src/s1/task/effe_rain.cpp
  src/s1/task/effe_resources.cpp
  src/s1/task/effe_fft.cpp
  src/s1/task/effe_particles.cpp
  src/s1/task/effe_emitters.cpp
  src/s1/task/effe_particle_draw.cpp
  src/s1/task/effe_weather_draw.cpp
  src/s1/task/effe_script.cpp
  src/s1/task/effe_queue.cpp
  src/s1/task/effe_fight.cpp
  src/s1/task/ewzo.cpp
  src/s1/face.cpp
  src/s1/file_loader.cpp
  src/s1/fvf_layout.cpp
  src/s1/frame_pipeline.cpp
  src/s1/graphics_resources.cpp
  src/s1/gzip.cpp
  src/s1/hlib.cpp
  src/s1/hlib_memory.cpp
  src/s1/init_main.cpp
  src/s1/input.cpp
  src/s1/initialization.cpp
  src/s1/initialization_2.cpp
  src/s1/launch_options.cpp
  src/s1/map_entries.cpp
  src/s1/task/krep.cpp
  src/s1/task/lcdm.cpp
  src/s1/task/lght.cpp
  src/s1/legacy_renderer.cpp
  src/s1/task/ctrl.cpp
  src/s1/task/meve.cpp
  src/s1/map_info.cpp
  src/s1/task/kdan.cpp
  src/s1/task/paus.cpp
  src/s1/task/wind.cpp
  src/s1/task/film.cpp
  src/s1/task/srch.cpp
  src/s1/task/time.cpp
  src/s1/task/mdlm.cpp
  src/s1/mip_streaming.cpp
  src/s1/modern_scene.cpp
  src/s1/modern_material.cpp
  src/s1/modern_lighting.cpp
  src/s1/modern_probes.cpp
  src/s1/model_loader.cpp
  src/s1/model_lighting.cpp
  src/s1/mt5_mesh.cpp
  src/s1/mt5_render.cpp
  src/s1/modules/modules.cpp
  src/s1/modules/arcade.cpp
  src/s1/game_utils.cpp
  src/s1/task/modu.cpp
  src/s1/task/hplg.cpp
  src/s1/task/disp.cpp
  src/s1/task/staf.cpp
  src/s1/task/sygh.cpp
  src/s1/task/sygu.cpp
  src/s1/task/vfcl.cpp
  src/s1/task/vf_core.cpp
  src/s1/task/vfma.cpp
  src/s1/task/vf_actor.cpp
  src/s1/task/vf_contact.cpp
  src/s1/task/vf_hit.cpp
  src/s1/task/vf_reaction_state.cpp
  src/s1/task/vf_fighting_style.cpp
  src/s1/task/vf_presentation.cpp
  src/s1/task/vf_reaction.cpp
  src/s1/task/vf_reaction_vm.cpp
  src/s1/task/vf_contact_reaction.cpp
  src/s1/task/vf_camera.cpp
  src/s1/task/vf_formation.cpp
  src/s1/task/vf_stage.cpp
  src/s1/motion_registry.cpp
  src/s1/motion.cpp
  src/s1/motion_parse.cpp
  src/s1/motion_keyframes.cpp
  src/s1/motion_pose.cpp
  src/s1/motion_raw_pose.cpp
  src/s1/motion_root.cpp
  src/s1/motion_dispatch.cpp
  src/s1/motion_tmnm.cpp
  src/s1/nuklear_integration.cpp
  src/s1/item_catalog.cpp
  src/s1/ui_definition.cpp
  src/s1/ui_runtime.cpp
  src/d3t/debug_font.cpp
  src/s1/ui_assets.cpp
  src/s1/ui_legacy_widgets.cpp
  src/s1/ui_hud.cpp
  src/s1/ui_qte.cpp
  src/s1/ui_icon.cpp
  src/s1/ui_action_selector.cpp
  src/s1/ui_cash.cpp
  src/s1/ui_messages.cpp
  src/s1/ui_training.cpp
  src/s1/ui_frontend.cpp
  src/s1/ui_grid.cpp
  src/s1/ui_help.cpp
  src/s1/ui_credits.cpp
  src/s1/ui_subtitles.cpp
  src/s1/ui_loadsave.cpp
  src/s1/ui_inventory.cpp
  src/s1/ui_collection.cpp
  src/s1/task/pack.cpp
  src/s1/task/padc.cpp
  src/s1/task/qte.cpp
  src/s1/task/ppad.cpp
  src/s1/player_control.cpp
  src/s1/player_stats.cpp
  src/s1/pvr_decoder.cpp
  src/s1/render_passes.cpp
  src/s1/scene.cpp
  src/s1/scene_memory.cpp
  src/s1/shader_initialization.cpp
  src/s1/shadow_volume.cpp
  src/s1/simple_shader.cpp
  src/s1/skinning_cache.cpp
  src/s1/task/save.cpp
  src/s1/task/scen.cpp
  src/s1/task/scen_dialogue.cpp
  src/s1/task/scen_interaction.cpp
  src/s1/task/scen_objects.cpp
  src/s1/task/scen_selection.cpp
  src/s1/task/scen_text.cpp
  src/s1/task/scen_transitions.cpp
  src/s1/task/scen_vm.cpp
  src/s1/task/scnf_bytecode.cpp
  src/s1/task/scenario_sequence.cpp
  src/s1/task/scen_sleep.cpp
  src/s1/task/scen_commands_character.cpp
  src/s1/task/scen_commands_camera.cpp
  src/s1/task/scen_commands_system.cpp
  src/s1/task/scnt.cpp
  src/s1/screen_space.cpp
  src/s1/task/scrl.cpp
  src/s1/steam.cpp
  src/s1/timing.cpp
  src/s1/task/trwv.cpp
  src/s1/texture_loader.cpp
  src/s1/task/wexe.cpp
  src/s1/task/wzog.cpp
  src/s1/window.cpp
)
target_compile_definitions(shenmue1_game PUBLIC SHENMUE_TARGET_S1_PC=1)
target_sources(shenmue1_game PRIVATE
  src/s1/modules/harrier.cpp
  src/s1/modules/harrier_cpu.cpp
  src/s1/modules/harrier_render.cpp
  src/s1/modules/harrier_audio.cpp
)
if(WIN32)
  if(SHENMUE_LOAD_HANGON_DLL)
    target_compile_definitions(shenmue1_game PRIVATE SHENMUE_LOAD_HANGON_DLL=1)
  elseif(MSVC)
    set(SHENMUE_HANGON_IMPORT "${CMAKE_CURRENT_BINARY_DIR}/GameHangon_import.lib")
    add_custom_command(OUTPUT "${SHENMUE_HANGON_IMPORT}"
      COMMAND "${CMAKE_AR}" /nologo /machine:x64 "/def:${CMAKE_CURRENT_SOURCE_DIR}/src/s1/modules/hangon.def"
              "/out:${SHENMUE_HANGON_IMPORT}"
      DEPENDS src/s1/modules/hangon.def VERBATIM)
    add_custom_target(shenmue_hangon_import DEPENDS "${SHENMUE_HANGON_IMPORT}")
    add_dependencies(shenmue1_game shenmue_hangon_import)
    target_link_libraries(shenmue1_game PUBLIC "${SHENMUE_HANGON_IMPORT}")
  else()
    message(FATAL_ERROR "Native Hang-On imports require MSVC; enable SHENMUE_LOAD_HANGON_DLL for another Windows toolchain")
  endif()
endif()
if(SHENMUE_RT_ARCADE)
  if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "SHENMUE_RT_ARCADE requires a Windows x64 build and the original Hang-On DLL")
  endif()
  target_sources(shenmue1_game PRIVATE src/s1/enhancements/rt_arcade.cpp)
  target_compile_definitions(shenmue1_game PUBLIC SHENMUE_RT_ARCADE=1)
endif()
set_source_files_properties(src/third_party/stb_image.cpp PROPERTIES
  COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/W0>;$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>"
)
target_compile_features(shenmue1_game PUBLIC cxx_std_20)
target_include_directories(shenmue1_game PUBLIC src)
target_sources(shenmue1_game PRIVATE src/s1/enhancements/enemy_gauge.cpp)
if(SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX)
  target_compile_definitions(shenmue1_game PRIVATE SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX=1)
endif()
if(SHENMUE_ENHANCEMENT_S1_SKY_FIX)
  target_compile_definitions(shenmue1_game PRIVATE SHENMUE_ENHANCEMENT_S1_SKY_FIX=1)
endif()
if(SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE)
  target_compile_definitions(shenmue1_game PRIVATE SHENMUE_ENHANCEMENT_S1_ENEMY_HP_GAUGE=1)
endif()
if(SHENMUE_ENHANCEMENT_S1_CASSETTES_ANYWHERE)
  target_compile_definitions(shenmue1_game PRIVATE SHENMUE_ENHANCEMENT_S1_CASSETTES_ANYWHERE=1)
endif()
target_include_directories(shenmue1_game PRIVATE "${SHENMUE_STB_INCLUDE_DIR}")
target_link_libraries(shenmue1_game PUBLIC shenmue_common shenmue_d3t ZLIB::ZLIB Threads::Threads
                                           ${SHENMUE_JSONCPP_TARGET})
if(WIN32)
  target_link_libraries(shenmue1_game PUBLIC d3d11 d3dcompiler dxgi dwmapi user32 gdi32
                                               xinput9_1_0 xaudio2 shenmue_imgui)
endif()
shenmue_set_project_warnings(shenmue1_game)

add_executable(shenmue1 EXCLUDE_FROM_ALL src/s1/main.cpp)
target_compile_features(shenmue1 PRIVATE cxx_std_20)
target_link_libraries(shenmue1 PRIVATE shenmue1_game)
if(WIN32 AND NOT SHENMUE_LOAD_HANGON_DLL)
  add_custom_command(TARGET shenmue1 POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$ENV{SHEN_ROOT}/sm1/GameHangon_Winx64_Release.dll" "$<TARGET_FILE_DIR:shenmue1>"
    VERBATIM)
endif()
if(MSVC)
  target_link_options(shenmue1 PRIVATE /MAP)
endif()
shenmue_set_project_warnings(shenmue1)
add_custom_command(TARGET shenmue1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_CURRENT_SOURCE_DIR}/reference/filename_database.json"
          "$<TARGET_FILE_DIR:shenmue1>/filename_database.json"
)

# PC/Xbox area and event implementation objects (native event subroutines compiled into the executable). Kept as an object
# library so the DC event modules can later build the same sources into separately linked EV overlays.
set_source_files_properties(src/s2/areas/common/et_setgamble.c PROPERTIES LANGUAGE CXX)
add_library(shenmue2_events OBJECT
  src/s2/areas/area_0004.cpp
  src/s2/areas/area_0005.cpp
  src/s2/areas/area_0209.cpp
  src/s2/areas/area_ar01.cpp
  src/s2/areas/area_arsf.cpp
  src/s2/areas/area_arc0.cpp
  src/s2/areas/area_qf06.cpp
  src/s2/areas/area_qtb4.cpp
  src/s2/areas/area_qf39.cpp
  src/s2/areas/area_qug0.cpp
  src/s2/areas/area_elevator_lamp.cpp
  src/s2/areas/area_kww1.cpp
  src/s2/areas/area_kwm1.cpp
  src/s2/areas/area_aka3.cpp
  src/s2/areas/area_aky0.cpp
  src/s2/areas/area_kwwq.cpp
  src/s2/areas/area_wsg1.cpp
  src/s2/areas/area_arm0.cpp
  src/s2/areas/area_arz0.cpp
  src/s2/areas/area_wet0.cpp
  src/s2/areas/area_wka0.cpp
  src/s2/areas/area_qaw6.cpp
  src/s2/areas/area_qc06.cpp
  src/s2/areas/area_krk4.cpp
  src/s2/areas/area_qae1.cpp
  src/s2/areas/area_qae6.cpp
  src/s2/areas/area_qaw1.cpp
  src/s2/areas/area_qdkj.cpp
  src/s2/areas/area_wecf.cpp
  src/s2/areas/area_qe01.cpp
  src/s2/areas/area_qe06.cpp
  src/s2/areas/area_qedj.cpp
  src/s2/areas/area_qfrr.cpp
  src/s2/areas/area_qaxx.cpp
  src/s2/areas/area_we00.cpp
  src/s2/areas/area_qb00.cpp
  src/s2/areas/area_chime.cpp
  src/s2/areas/area_0266.cpp
  src/s2/areas/area_0271.cpp
  src/s2/areas/area_0273.cpp
  src/s2/areas/area_0274.cpp
  src/s2/areas/area_0275.cpp
  src/s2/areas/area_0276.cpp
  src/s2/areas/area_0277.cpp
  src/s2/areas/area_0278.cpp
  src/s2/areas/area_0282.cpp
  src/s2/areas/area_0289.cpp
  src/s2/areas/area_028e.cpp
  src/s2/areas/area_0290.cpp
  src/s2/areas/area_0291.cpp
  src/s2/areas/area_0297.cpp
  src/s2/areas/area_0307.cpp
  src/s2/areas/area_0308.cpp
  src/s2/areas/area_0309.cpp
  src/s2/areas/area_031e.cpp
  src/s2/areas/area_0324.cpp
  src/s2/areas/area_0337.cpp
  src/s2/areas/area_0339.cpp
  src/s2/areas/area_0340.cpp
  src/s2/areas/area_0361.cpp
  src/s2/areas/area_0362.cpp
  src/s2/areas/area_0364.cpp
  src/s2/areas/area_039e.cpp
  src/s2/areas/area_0404.cpp
  src/s2/areas/area_048e.cpp
  src/s2/areas/area_0500.cpp
  src/s2/areas/area_0502.cpp
  src/s2/areas/area_0504.cpp
  src/s2/areas/area_0505.cpp
  src/s2/areas/area_0507.cpp
  src/s2/areas/area_0509.cpp
  src/s2/areas/area_0510.cpp
  src/s2/areas/area_0512.cpp
  src/s2/areas/area_0514.cpp
  src/s2/areas/area_0516_event.cpp
  src/s2/areas/area_0517.cpp
  src/s2/areas/area_0518.cpp
  src/s2/areas/area_0519.cpp
  src/s2/areas/area_0521.cpp
  src/s2/areas/area_0523.cpp
  src/s2/areas/area_0524.cpp
  src/s2/areas/area_0525.cpp
  src/s2/areas/area_0527.cpp
  src/s2/areas/area_0528.cpp
  src/s2/areas/area_0529.cpp
  src/s2/areas/area_0530.cpp
  src/s2/areas/area_0531.cpp
  src/s2/areas/area_0537.cpp
  src/s2/areas/area_0538.cpp
  src/s2/areas/area_053e.cpp
  src/s2/areas/area_0540.cpp
  src/s2/areas/area_0541.cpp
  src/s2/areas/area_0543.cpp
  src/s2/areas/area_0545.cpp
  src/s2/areas/area_0547.cpp
  src/s2/areas/area_0549.cpp
  src/s2/areas/area_054e.cpp
  src/s2/areas/area_063e.cpp
  src/s2/areas/area_122e.cpp
  src/s2/areas/area_188e.cpp
  src/s2/areas/area_192e.cpp
  src/s2/areas/area_194e.cpp
  src/s2/areas/area_202e.cpp
  src/s2/areas/area_234e.cpp
  src/s2/areas/area_235e.cpp
  src/s2/areas/area_246e.cpp
  src/s2/areas/area_253e.cpp
  src/s2/areas/area_275e.cpp
  src/s2/areas/area_280e.cpp
  src/s2/areas/area_282e.cpp
  src/s2/areas/area_284e.cpp
  src/s2/areas/area_290e.cpp
  src/s2/areas/area_301e.cpp
  src/s2/areas/area_302e.cpp
  src/s2/areas/area_303e.cpp
  src/s2/areas/area_329e.cpp
  src/s2/areas/area_347e.cpp
  src/s2/areas/area_527e.cpp
  src/s2/areas/area_528e.cpp
  src/s2/areas/area_529e.cpp
  src/s2/areas/area_545e.cpp
  src/s2/areas/area_552e.cpp
  src/s2/areas/area_ab00.cpp
  src/s2/areas/area_ak00.cpp
  src/s2/areas/area_aks0.cpp
  src/s2/areas/area_aks1.cpp
  src/s2/areas/area_akt0.cpp
  src/s2/areas/area_akt2.cpp
  src/s2/areas/area_akt3.cpp
  src/s2/areas/area_ar02.cpp
  src/s2/areas/area_ar03.cpp
  src/s2/areas/area_ara0.cpp
  src/s2/areas/area_kes1.cpp
  src/s2/areas/area_kmz1.cpp
  src/s2/areas/area_kmz2.cpp
  src/s2/areas/area_kmz3.cpp
  src/s2/areas/area_kmz4.cpp
  src/s2/areas/area_krc1.cpp
  src/s2/areas/area_krh1.cpp
  src/s2/areas/area_krk2.cpp
  src/s2/areas/area_krm3.cpp
  src/s2/areas/area_ksh1.cpp
  src/s2/areas/area_kww4.cpp
  src/s2/areas/area_q100.cpp
  src/s2/areas/area_q200.cpp
  src/s2/areas/area_q300.cpp
  src/s2/areas/area_qa00.cpp
  src/s2/areas/area_qa11.cpp
  src/s2/areas/area_qa22.cpp
  src/s2/areas/area_qc00.cpp
  src/s2/areas/area_qc01.cpp
  src/s2/areas/area_qcae.cpp
  src/s2/areas/area_qd01.cpp
  src/s2/areas/area_qe00.cpp
  src/s2/areas/area_qf00.cpp
  src/s2/areas/area_qf01.cpp
  src/s2/areas/area_qf02.cpp
  src/s2/areas/area_qf40.cpp
  src/s2/areas/area_qgbt.cpp
  src/s2/areas/area_qjbt.cpp
  src/s2/areas/area_qkbt.cpp
  src/s2/areas/area_qlbt.cpp
  src/s2/areas/area_qr00.cpp
  src/s2/areas/area_qrr0.cpp
  src/s2/areas/area_qsfa.cpp
  src/s2/areas/area_qsfb.cpp
  src/s2/areas/area_qsfc.cpp
  src/s2/areas/area_qsfd.cpp
  src/s2/areas/area_qtb1.cpp
  src/s2/areas/area_qtb2.cpp
  src/s2/areas/area_qtb3.cpp
  src/s2/areas/area_tbak.cpp
  src/s2/areas/area_wb00.cpp
  src/s2/areas/area_wb01.cpp
  src/s2/areas/area_weg0.cpp
  src/s2/areas/area_wem1.cpp
  src/s2/areas/area_wes1.cpp
  src/s2/areas/area_wesm.cpp
  src/s2/areas/area_wk00.cpp
  src/s2/areas/area_wn00.cpp
  src/s2/areas/area_wr00.cpp
  src/s2/areas/area_wrs2.cpp
  src/s2/areas/area_ws00.cpp
  src/s2/areas/area_wsy0.cpp
  src/s2/areas/area_wt00.cpp
  src/s2/areas/area_wta0.cpp
  src/s2/areas/area_street_watch.cpp
  src/s2/areas/area_maze_events.cpp
  src/s2/areas/area_elevator.cpp
  src/s2/areas/area_jukebox.cpp
  src/s2/areas/area_vend.cpp
  src/s2/areas/area_q7qb.cpp
  src/s2/areas/area_q7qb_choreography.cpp
  src/s2/areas/area_frbt.cpp
  src/s2/areas/area_frbt_authored.cpp
  src/s2/areas/area_brib.cpp
  src/s2/areas/area_arkq.cpp
  src/s2/areas/area_arkq_choreography.cpp
  src/s2/areas/area_9kan.cpp
  src/s2/areas/area_9kan_authored.cpp
  src/s2/areas/area_arbt.cpp
  src/s2/areas/arbt_crates.cpp
  src/s2/areas/arbt_authored.cpp
  src/s2/areas/arbt_routes.cpp
  src/s2/areas/area_mqte.cpp
  src/s2/areas/mqte_choreography.cpp
  src/s2/areas/mqte_effects.cpp
  src/s2/areas/area_mdtp.cpp
  src/s2/areas/mdtp_authored.cpp
  src/s2/areas/mdtp_motion.cpp
  src/s2/areas/area_bgsm.cpp
  src/s2/areas/bgsm_authored.cpp
  src/s2/areas/bgsm_dialogue.cpp
  src/s2/areas/bgsm_selection.cpp
  src/s2/areas/bgsm_dice.cpp
  src/s2/areas/bgsm_results.cpp
  src/s2/areas/bgsm_voice.cpp
  src/s2/areas/area_armw.cpp
  src/s2/areas/armw_tables.cpp
  src/s2/areas/armw_gameplay.cpp
  src/s2/areas/armw_authored.cpp
  src/s2/areas/area_qabt.cpp
  src/s2/areas/qabt_crates.cpp
  src/s2/areas/qabt_authored.cpp
  src/s2/areas/area_bkw0.cpp
  src/s2/areas/bkw0_tables.cpp
  src/s2/areas/bkw0_training.cpp
  src/s2/areas/bkw0_crowd.cpp
  src/s2/areas/bkw0_practice.cpp
  src/s2/areas/bkw0_decision.cpp
  src/s2/areas/bkw0_authored.cpp
  src/s2/areas/bkw0_bow.cpp
  src/s2/areas/bkw0_dialogue.cpp
  src/s2/areas/area_exqt.cpp
  src/s2/areas/area_dgct.cpp
  src/s2/areas/exqt_input.cpp
  src/s2/areas/exqt_presentation.cpp
  src/s2/areas/exqt_choreography.cpp
  src/s2/areas/area_khin.cpp
  src/s2/areas/khin_purchase.cpp
  src/s2/areas/khin_tokens.cpp
  src/s2/areas/khin_withdrawal.cpp
  src/s2/areas/khin_medals.cpp
  src/s2/areas/area_qlgt.cpp
  src/s2/areas/qlgt_tables.cpp
  src/s2/areas/qlgt_choreography.cpp
  src/s2/areas/qlgt_resources.cpp
  src/s2/areas/qlgt_battle.cpp
  src/s2/areas/area_pati.cpp
  src/s2/areas/pati_dialog.cpp
  src/s2/areas/pati_schedule.cpp
  src/s2/areas/pati_lifecycle.cpp
  src/s2/areas/pati_boards.cpp
  src/s2/areas/pati_selection.cpp
  src/s2/areas/pati_camera.cpp
  src/s2/areas/pati_nail_models.cpp
  src/s2/areas/pati_payout.cpp
  src/s2/areas/pati_ball.cpp
  src/s2/areas/pati_ball_input.cpp
  src/s2/areas/pati_ball_transform.cpp
  src/s2/areas/pati_board_data.cpp
  src/s2/areas/pati_physics.cpp
  src/s2/areas/pati_collision.cpp
  src/s2/areas/pati_response.cpp
  src/s2/areas/area_anan.cpp
  src/s2/areas/anan_authored.cpp
  src/s2/areas/anan_game.cpp
  src/s2/areas/anan_lifecycle.cpp
  src/s2/areas/area_cb00.cpp
  src/s2/areas/cb00_choreography.cpp
  src/s2/areas/cb00_bridge.cpp
  src/s2/areas/cb00_effects.cpp
  src/s2/areas/area_etrk.cpp
  src/s2/areas/etrk_tables.cpp
  src/s2/areas/etrk_keys.cpp
  src/s2/areas/etrk_switches.cpp
  src/s2/areas/area_gach.cpp
  src/s2/areas/area_kach.cpp
  src/s2/areas/kach_physics.cpp
  src/s2/areas/area_titl.cpp
  src/s2/areas/titl_gameplay.cpp
  src/s2/areas/titl_difficulty.cpp
  src/s2/areas/area_omak.cpp
  src/s2/areas/omak_main.cpp
  src/s2/areas/omak_sprites.cpp
  src/s2/areas/area_pawn.cpp
  src/s2/areas/pawn_scene_data.cpp
  src/s2/areas/pawn_flyer.cpp
  src/s2/areas/area_qdss.cpp
  src/s2/areas/qdss_callbacks.cpp
  src/s2/areas/qdss_selection.cpp
  src/s2/areas/qdss_insertion.cpp
  src/s2/areas/qdss_discovery.cpp
  src/s2/areas/area_ro10.cpp
  src/s2/areas/ro10_shop_data.cpp
  src/s2/areas/ro10_items.cpp
  src/s2/areas/ro10_dialogue.cpp
  src/s2/areas/ro10_hand.cpp
  src/s2/areas/area_uede.cpp
  src/s2/areas/uede_dialogue.cpp
  src/s2/areas/uede_dice.cpp
  src/s2/areas/uede_results.cpp
  src/s2/areas/area_gabt.cpp
  src/s2/areas/gabt_authored.cpp
  src/s2/areas/gabt_mole.cpp
  src/s2/areas/gabt_battle.cpp
  src/s2/areas/gabt_outcomes.cpp
  src/s2/areas/area_knmn.cpp
  src/s2/areas/knmn_battle.cpp
  src/s2/areas/knmn_practice.cpp
  src/s2/areas/knmn_decision.cpp
  src/s2/areas/knmn_qte.cpp
  src/s2/areas/knmn_feedback.cpp
  src/s2/areas/area_knw0.cpp
  src/s2/areas/knw0_authored.cpp
  src/s2/areas/knw0_sequence.cpp
  src/s2/areas/knw0_decision.cpp
  src/s2/areas/knw0_practice.cpp
  src/s2/areas/knw0_feedback.cpp
  src/s2/areas/area_ycpa.cpp
  src/s2/areas/ycpa_queued.cpp
  src/s2/areas/ycpa_choreography.cpp
  src/s2/areas/ycpa_lane.cpp
  src/s2/areas/ycpa_geometry.cpp
  src/s2/areas/ycpa_discovery.cpp
  src/s2/areas/ycpa_authored.cpp
  src/s2/areas/ycpa_brief.cpp
  src/s2/areas/ycpa_cleanup.cpp
  src/s2/areas/ycpa_advice.cpp
  src/s2/areas/area_ksht.cpp
  src/s2/areas/ksht_authored.cpp
  src/s2/areas/ksht_cloud_author.cpp
  src/s2/areas/ksht_cloud_characters.cpp
  src/s2/areas/ksht_cloud_effects.cpp
  src/s2/areas/ksht_cloud_exit.cpp
  src/s2/areas/ksht_cloud_packages.cpp
  src/s2/areas/ksht_conversations.cpp
  src/s2/areas/ksht_departure_author.cpp
  src/s2/areas/ksht_drawing_author.cpp
  src/s2/areas/ksht_exit.cpp
  src/s2/areas/ksht_free_talk.cpp
  src/s2/areas/ksht_free_talk_actors.cpp
  src/s2/areas/ksht_pose.cpp
  src/s2/areas/ksht_short_conversations.cpp
  src/s2/areas/ksht_small_authors.cpp
  src/s2/areas/ksht_subtitles.cpp
  src/s2/areas/ksht_waiting.cpp
  src/s2/areas/area_maki.cpp
  src/s2/areas/maki_pickup.cpp
  src/s2/areas/maki_state.cpp
  src/s2/areas/maki_dialogue.cpp
  src/s2/areas/maki_lighter.cpp
  src/s2/areas/area_wbbk.cpp
  src/s2/areas/wbbk_authored.cpp
  src/s2/areas/wbbk_carry.cpp
  src/s2/areas/wbbk_choreography.cpp
  src/s2/areas/wbbk_dialogue.cpp
  src/s2/areas/wbbk_drop.cpp
  src/s2/areas/wbbk_idle.cpp
  src/s2/areas/wbbk_introduction.cpp
  src/s2/areas/wbbk_lifecycle.cpp
  src/s2/areas/wbbk_models.cpp
  src/s2/areas/wbbk_motion.cpp
  src/s2/areas/wbbk_pickup.cpp
  src/s2/areas/wbbk_presentation.cpp
  src/s2/areas/wbbk_qte.cpp
  src/s2/areas/wbbk_scenario.cpp
  src/s2/areas/wbbk_shadow.cpp
  src/s2/areas/area_slot.cpp
  src/s2/areas/area_slt1.cpp
  src/s2/areas/area_kesa.cpp
  src/s2/areas/kesa_authored.cpp
  src/s2/areas/kesa_sword.cpp
  src/s2/areas/kesa_energy.cpp
  src/s2/areas/kesa_fire.cpp
  src/s2/areas/kesa_lighting.cpp
  src/s2/areas/area_krct.cpp
  src/s2/areas/krct_authored.cpp
  src/s2/areas/krct_characters.cpp
  src/s2/areas/area_chaw.cpp
  src/s2/areas/chaw_gameplay.cpp
  src/s2/areas/chaw_callbacks.cpp
  src/s2/areas/chaw_authors.cpp
  src/s2/areas/chaw_cleanup.cpp
  src/s2/areas/chaw_lecture.cpp
  src/s2/scripts/cwon/area_cwon.cpp
  src/s2/scripts/cwon/cwon_helpers.cpp
  src/s2/scripts/cwon/cwon_props.cpp
  src/s2/scripts/cwon/cwon_escape.cpp
  src/s2/scripts/cwon/cwon_tables.cpp
  src/s2/scripts/cwon/cwon_battle.cpp
  src/s2/scripts/cwon/cwon_authored.cpp
  src/s2/areas/area_pvsj.cpp
  src/s2/areas/pvsj_tables.cpp
  src/s2/areas/pvsj_boards.cpp
  src/s2/areas/pvsj_dialogue.cpp
  src/s2/areas/pvsj_camera.cpp
  src/s2/areas/pvsj_helpers.cpp
  src/s2/areas/pvsj_lifecycle.cpp
  src/s2/areas/pvsj_gameplay.cpp
  src/s2/areas/area_tejo.cpp
  src/s2/areas/tejo_lifecycle.cpp
  src/s2/areas/tejo_tables.cpp
  src/s2/areas/area_game.cpp
  src/s2/areas/game/arcade.cpp
  src/s2/areas/game/arcade/render.cpp
  src/s2/areas/area_toma.cpp
  src/s2/areas/toma_talk.cpp
  src/s2/areas/toma_helpers.cpp
  src/s2/areas/area_syw0.cpp
  src/s2/areas/syw0_authored.cpp
  src/s2/areas/syw0_training.cpp
  src/s2/areas/syw0_sparring.cpp
  src/s2/areas/syw0_practice.cpp
  src/s2/areas/syw0_feedback.cpp
  src/s2/areas/syw0_dialogue.cpp
  src/s2/areas/syw0_decision.cpp
  src/s2/areas/syw0_gate.cpp
  src/s2/areas/area_ytqb.cpp
  src/s2/areas/ytqb_attacks.cpp
  src/s2/areas/ytqb_choreography.cpp
  src/s2/areas/ytqb_effects.cpp
  src/s2/areas/area_mfw0.cpp
  src/s2/areas/mfw0_authored.cpp
  src/s2/areas/mfw0_choreography.cpp
  src/s2/areas/mfw0_training.cpp
  src/s2/areas/mfw0_feedback.cpp
  src/s2/areas/mfw0_dialogue.cpp
  src/s2/areas/mfw0_decision.cpp
  src/s2/areas/area_wbsr.cpp
  src/s2/areas/area_wkta.cpp
  src/s2/areas/area_wrsq.cpp
  src/s2/areas/area_wssf.cpp
  src/s2/areas/area_wtkq.cpp
  src/s2/areas/area_sfjf.cpp
  src/s2/areas/area_aktm.cpp
  src/s2/areas/area_aozr.cpp
  src/s2/areas/area_apa0.cpp
  src/s2/areas/area_bjmp.cpp
  src/s2/areas/area_bok2.cpp
  src/s2/areas/area_bosy.cpp
  src/s2/areas/area_btel.cpp
  src/s2/areas/area_istb.cpp
  src/s2/areas/area_kick.cpp
  src/s2/areas/area_koro.cpp
  src/s2/areas/area_duck.cpp
  src/s2/areas/area_ablb.cpp
  src/s2/areas/area_b102.cpp
  src/s2/areas/area_bell.cpp
  src/s2/areas/area_bt40.cpp
  src/s2/areas/area_dabo.cpp
  src/s2/areas/area_elevator_event.cpp
  src/s2/areas/area_gate.cpp
  src/s2/areas/area_hpam.cpp
  src/s2/areas/area_loop.cpp
  src/s2/areas/area_0516.cpp
  src/s2/areas/area_entities.cpp
  src/s2/areas/common/et_setgamble.c
  src/s2/areas/area_pigeons.cpp
  src/s2/areas/area_ahqb.cpp
  src/s2/areas/area_cret.cpp
  src/s2/areas/area_crti.cpp
  src/s2/areas/area_find.cpp
  src/s2/areas/area_gais.cpp
  src/s2/areas/area_jiag.cpp
  src/s2/areas/area_jukb.cpp
  src/s2/areas/area_kami.cpp
  src/s2/areas/area_kbrg.cpp
  src/s2/areas/area_kebi.cpp
  src/s2/areas/area_kest.cpp
  src/s2/areas/area_kwwt.cpp
  src/s2/areas/area_lock.cpp
  src/s2/areas/area_okjo.cpp
  src/s2/areas/area_op02.cpp
  src/s2/areas/area_pach.cpp
  src/s2/areas/area_ahqb_combat.cpp
  src/s2/areas/area_qdfo.cpp
  src/s2/areas/area_qehi.cpp
  src/s2/areas/area_qesc.cpp
  src/s2/areas/area_qyss.cpp
  src/s2/areas/area_qyua.cpp
  src/s2/areas/area_rati.cpp
  src/s2/areas/area_scmn.cpp
  src/s2/areas/area_tes2.cpp
  src/s2/areas/area_tgbt.cpp
  src/s2/areas/area_tofb.cpp
  src/s2/areas/area_topa.cpp
  src/s2/areas/area_ttrp.cpp
  src/s2/areas/area_vsyb.cpp
  src/s2/areas/area_wbbr.cpp
  src/s2/areas/area_wonc.cpp
  src/s2/areas/area_wsye.cpp
  src/s2/areas/area_yoru.cpp
  src/s2/areas/area_yuku.cpp
  src/s2/areas/area_4bil.cpp
  src/s2/areas/area_4btl.cpp
  src/s2/areas/area_0007.cpp
  src/s2/areas/area_0008.cpp
  src/s2/areas/area_0012.cpp
  src/s2/areas/area_0013.cpp
  src/s2/areas/area_0016.cpp
  src/s2/areas/area_0028.cpp
  src/s2/areas/area_0030.cpp
  src/s2/areas/area_0031.cpp
  src/s2/areas/area_0032.cpp
  src/s2/areas/area_0033.cpp
  src/s2/areas/area_0034.cpp
  src/s2/areas/area_0035.cpp
  src/s2/areas/area_0036.cpp
  src/s2/areas/area_0037.cpp
  src/s2/areas/area_0038.cpp
  src/s2/areas/area_0039.cpp
  src/s2/areas/area_0045.cpp
  src/s2/areas/area_0047.cpp
  src/s2/areas/area_0048.cpp
  src/s2/areas/area_0049.cpp
  src/s2/areas/area_0050.cpp
  src/s2/areas/area_0055.cpp
  src/s2/areas/area_0056.cpp
  src/s2/areas/area_0057.cpp
  src/s2/areas/area_0058.cpp
  src/s2/areas/area_0059.cpp
  src/s2/areas/area_0062.cpp
  src/s2/areas/area_0063.cpp
  src/s2/areas/area_0064.cpp
  src/s2/areas/area_0066.cpp
  src/s2/areas/area_0067.cpp
  src/s2/areas/area_0068.cpp
  src/s2/areas/area_0069.cpp
  src/s2/areas/area_0076.cpp
  src/s2/areas/area_0077.cpp
  src/s2/areas/area_0080.cpp
  src/s2/areas/area_011e.cpp
  src/s2/areas/area_0138.cpp
  src/s2/areas/area_0139.cpp
  src/s2/areas/area_0140.cpp
  src/s2/areas/area_0141.cpp
  src/s2/areas/area_0142.cpp
  src/s2/areas/area_0143.cpp
  src/s2/areas/area_0146.cpp
  src/s2/areas/area_0151.cpp
  src/s2/areas/area_0173.cpp
  src/s2/areas/area_0175.cpp
  src/s2/areas/area_0184.cpp
  src/s2/areas/area_0186.cpp
  src/s2/areas/area_0200.cpp
  src/s2/areas/area_0201.cpp
  src/s2/areas/area_0202.cpp
  src/s2/areas/area_0204.cpp
  src/s2/areas/area_0207.cpp
  src/s2/areas/area_020e.cpp
  src/s2/areas/area_0210.cpp
  src/s2/areas/area_0213.cpp
  src/s2/areas/area_0214.cpp
  src/s2/areas/area_0215.cpp
  src/s2/areas/area_0216.cpp
  src/s2/areas/area_0217.cpp
  src/s2/areas/area_0218.cpp
  src/s2/areas/area_0221.cpp
  src/s2/areas/area_0222.cpp
  src/s2/areas/area_0225.cpp
  src/s2/areas/area_0228.cpp
  src/s2/areas/area_0232.cpp
  src/s2/areas/area_0235.cpp
  src/s2/areas/area_0236.cpp
  src/s2/areas/area_0237.cpp
  src/s2/areas/area_0240.cpp
  src/s2/areas/area_0247.cpp
  src/s2/areas/area_0248.cpp
  src/s2/areas/area_0249.cpp
  src/s2/areas/area_0250.cpp
  src/s2/areas/area_0252.cpp
  src/s2/areas/area_0256.cpp
  src/s2/areas/area_0257.cpp
  src/s2/areas/area_0258.cpp
  src/s2/areas/area_0259.cpp
  src/s2/areas/area_0260.cpp
  src/s2/areas/area_0262.cpp
  src/s2/areas/area_0264.cpp
  src/s2/areas/area_0265.cpp
  src/s2/areas/numeric_event.cpp
  src/s2/areas/harbor_author_event.cpp
  src/s2/areas/area_0061.cpp
  src/s2/areas/area_0009.cpp
  src/s2/areas/area_0010.cpp
  src/s2/areas/area_0014.cpp
  src/s2/areas/area_0015.cpp
  src/s2/areas/area_0017.cpp
  src/s2/areas/area_0023.cpp
  src/s2/areas/area_0025.cpp
  src/s2/areas/area_0026.cpp
  src/s2/areas/area_0027.cpp
  src/s2/areas/area_0029.cpp
  src/s2/areas/area_WBSS.cpp
  src/s2/areas/area_WKKT.cpp
  src/s2/areas/area_krk6.cpp
  src/s2/areas/area_qd00.cpp
  src/s2/areas/area_4nin.cpp
  src/s2/areas/area_fall_events.cpp
  src/s2/areas/area_dart_arrow.cpp
  src/s2/areas/area_dart_ai.cpp
  src/s2/areas/area_dart_admission.cpp
  src/s2/areas/area_dart_opponents.cpp
  src/s2/areas/area_dart_scoring.cpp
  src/s2/areas/area_dart_display.cpp
  src/s2/areas/area_dart_children.cpp
  src/s2/areas/area_sfkk.cpp
  src/s2/areas/area_qtcv.cpp
  src/s2/areas/area_WB88.cpp
  src/s2/areas/area_0001.cpp
  src/s2/areas/area_ymqb.cpp
  src/s2/areas/area_qpam.cpp
  src/s2/areas/area_qrdk.cpp
  src/s2/areas/area_sysp.cpp
  src/s2/areas/area_sysp_dispatch.cpp
  src/s2/areas/area_dart.cpp
  src/s2/areas/area_sfkm.cpp
  src/s2/areas/area_sfiz.cpp
  src/s2/areas/area_sfbr.cpp
  src/s2/areas/area_hasi.cpp
  src/s2/areas/area_qdh1.cpp
  src/s2/areas/area_qdh2.cpp
  src/s2/areas/area_qdtb.cpp
  src/s2/areas/area_qdtg.cpp
  src/s2/areas/area_kew0.cpp
  src/s2/areas/area_qfeb.cpp
  src/s2/areas/qes_event.cpp
  src/s2/areas/qes2_event.cpp
  src/s2/areas/qes3_event.cpp
  src/s2/areas/area_qes1.cpp
  src/s2/areas/area_qes2.cpp
  src/s2/areas/area_qes3.cpp
  src/s2/areas/qdmr_timer.cpp
  src/s2/areas/qdmr_event.cpp
  src/s2/areas/area_qdmr.cpp
  src/s2/areas/area_urni.cpp
  src/s2/areas/urni_syk.cpp
  src/s2/areas/urni_kln.cpp
  src/s2/areas/urni_mm5.cpp
  src/s2/areas/urni_seh.cpp
  src/s2/areas/urni_sri.cpp
  src/s2/areas/urni_rcd.cpp
  src/s2/areas/area_sflu.cpp
  src/s2/areas/area_sfm2.cpp
  src/s2/areas/area_sfs2.cpp
  src/s2/areas/area_sfsr.cpp
  src/s2/areas/area_sftc.cpp
  src/s2/areas/area_sfts.cpp
  src/s2/areas/area_sfwf.cpp
)
add_library(shenmue2_game STATIC
  src/d3t/content.cpp
  src/d3t/model_load.cpp
  src/d3t/ui_runtime.cpp
  src/d3t/ui_animation.cpp
  src/d3t/ui_input.cpp
  src/d3t/ui_layout.cpp
  src/d3t/ui_lists.cpp
  src/d3t/ui_panels.cpp
  src/d3t/ui_render.cpp
  src/d3t/ui_text.cpp
  src/d3t/ui_views.cpp
  src/d3t/ui_widgets.cpp
  src/d3t/model_paths.cpp
  src/d3t/model_cache.cpp
  src/d3t/object_store.cpp
  src/s2/renderer_scratch.cpp
        src/s2/advice.cpp
        src/s2/area_event.cpp
        src/s2/area_jump.cpp
        src/s2/author.cpp
        src/s2/author_control.cpp
        src/s2/camera.cpp
        src/s2/character.cpp
        src/s2/conversation.cpp
        src/s2/door.cpp
        src/s2/display_configuration.cpp
        src/s2/elevator.cpp
        src/s2/event.cpp
        src/s2/scenario.cpp
        src/s2/scenario_characters.cpp
        src/s2/scenario_free_talk.cpp
        src/s2/scenario_scnt.cpp
        src/s2/scenario_actions.cpp
        src/s2/scenario_window.cpp
        src/s2/scenario_flags.cpp
        src/s2/scenario_event_objects.cpp
        src/s2/scenario_sequence.cpp
        src/s2/event_qte.cpp
        src/s2/event_darts.cpp
        src/s2/event_weather.cpp
        src/s2/event_cloth.cpp
        src/s2/event_character_api.cpp
        src/s2/dynamic_tracks.cpp
        src/s2/butterfly.cpp
        src/s2/event_characters.cpp
        src/s2/event_overlay.cpp
        src/s2/event_queued.cpp
        src/s2/extra_telop.cpp
        src/s2/tglb_event.cpp
        src/s2/tglb_author.cpp
        src/s2/tglb_effects.cpp
        src/s2/step_event.cpp
        src/s2/dai2_event.cpp
        src/s2/geto_event.cpp
        src/s2/anbd_event.cpp
        src/s2/wnpc_event.cpp
        src/s2/krht_event.cpp
        src/s2/hide_event.cpp
        src/s2/nigt_event.cpp
        src/s2/author_viewer_event.cpp
        src/s2/author_viewer_pair_event.cpp
        src/s2/qkqb_event.cpp
        src/s2/flag.cpp
        src/s2/fight.cpp
        src/s2/fight_motion.cpp
        src/s2/fight_reaction.cpp
        src/s2/fight_dialogue.cpp
        src/s2/fight_collision.cpp
        src/s2/fight_action.cpp
        src/s2/fight_object_motion.cpp
        src/s2/fight_main.cpp
        src/s2/fish.cpp
        src/s2/full_map_menu.cpp
        src/s2/legacy_collection.cpp
        src/s2/effect_fft.cpp
        src/s2/model_component.cpp
        src/s2/character_control.cpp
        src/s2/party_companion.cpp
        src/s2/cloth.cpp
        src/s2/hand_model.cpp
        src/s2/hand_pose.cpp
        src/s2/model_runtime.cpp
        src/s2/game_display.cpp
        src/s2/object_xforms.cpp
        src/s2/reflector_board.cpp
        src/s2/character_alpha.cpp
        src/s2/footstep.cpp
        src/s2/tail_lamp.cpp
        src/s2/model_image.cpp
        src/s2/afs.cpp
        src/s2/magic_room.cpp
        src/s2/magic_room_generation.cpp
        src/s2/ui_scene.cpp
        src/s2/ui_grid.cpp
        src/s2/motion_work.cpp
        src/s2/moves_scroll.cpp
        src/s2/item_wf.cpp
        src/s2/particle_effects.cpp
        src/s2/player.cpp
  src/s2/player_collision.cpp
  src/s2/player_movement.cpp
  src/s2/player_input.cpp
  src/s2/player_companions.cpp
  src/s2/player_placement.cpp
  src/s2/player_held.cpp
        src/s2/world_render.cpp
  src/s2/audio_data.cpp
  src/s2/aica_middleware.cpp
  src/s2/sound_program.cpp
  src/s2/d3t_renderer.cpp
  src/s2/date_time.cpp
  src/s2/debug_text.cpp
  src/d3t/debug_font.cpp
  src/s2/debug.cpp
  src/s2/directory_cache.cpp
  src/s2/math.cpp
  src/s2/bridge_bending.cpp
  src/s2/cold_archive.cpp
  src/s2/gauge.cpp
  src/s2/help.cpp
  src/s2/hlib.cpp
  src/s2/font.cpp
  src/s2/face.cpp
  src/s2/hand.cpp
  src/s2/handcuffs.cpp
  src/s2/hiki_model.cpp
  src/s2/hiki.cpp
  src/s2/effect_texture.cpp
  src/s2/texture_source.cpp
  src/s2/field_collision.cpp
  src/s2/fog.cpp
  src/s2/frame_pacing.cpp
  src/s2/weather.cpp
  src/s2/warning.cpp
  src/s2/snapshot.cpp
  src/s2/tree.cpp
  src/s2/input.cpp
  src/s2/input_bindings.cpp
  src/s2/inventory.cpp
  src/s2/load_title.cpp
  src/s2/pawn.cpp
  src/s2/game_runtime.cpp
  src/s2/initialization.cpp
  src/s2/map_control.cpp
  src/s2/map_assets.cpp
  src/s2/maze.cpp
  src/s2/memo.cpp
  src/s2/map_info.cpp
  src/s2/legacy_draw.cpp
  src/s2/legacy_projection.cpp
  src/s2/legacy_sprite.cpp
  src/s2/battle_qte.cpp
  src/s2/battle_qte_sprites.cpp
  src/s2/battle_qte_ui.cpp
  src/s2/fight_stage.cpp
  src/s2/lighting.cpp
  src/s2/sd_light.cpp
  src/s2/scene_lighting.cpp
  src/s2/sd_probe.cpp
  src/s2/model_geometry.cpp
  src/s2/model_draw.cpp
  src/s2/frame_pipeline.cpp
  src/s2/simple_shader.cpp
  src/s2/motion.cpp
  src/s2/motion_pose.cpp
  src/s2/motion_frame.cpp
  src/s2/motion_rig.cpp
  src/s2/mt7.cpp
  src/s2/model_clone.cpp
  src/s2/blur_figure_pose.cpp
  src/s2/navigation.cpp
  src/s2/navi.cpp
  src/s2/npc.cpp
  src/s2/npc_package.cpp
  src/s2/npc_package_streaming.cpp
  src/s2/npc_package_routes.cpp
  src/s2/npc_package_schedule.cpp
  src/s2/npc_package_transition.cpp
  src/s2/npc_package_simulation.cpp
  src/s2/npc_area_route.cpp
  src/s2/pawn_shop.cpp
  src/s2/pause.cpp
  src/s2/pak.cpp
  src/s2/renderer_initialization.cpp
  src/s2/scene_data.cpp
  src/s2/save.cpp
  src/s2/search.cpp
  src/s2/screen_film.cpp
  src/s2/story_state.cpp
  src/s2/system_modules.cpp
  src/s2/title.cpp
  src/s2/ui.cpp
  src/s2/ui_scroll_list.cpp
  src/s2/ui_loadsave.cpp
  src/s2/ui_credits.cpp
  src/s2/ui_options.cpp
  src/s2/ui_collection.cpp
  src/s2/ui_snapshot_debug.cpp
  src/s2/ui_pawn_flyers.cpp
  src/s2/ui_pawn_legacy.cpp
  src/s2/ui_list_input.cpp
  src/s2/ui_frontend.cpp
  src/s2/ui_legacy_icons.cpp
  src/s2/ui_subtitles.cpp
  src/s2/ui_transition.cpp
  src/s2/training_seqp.cpp
  src/s2/minigame_random.cpp
  src/s2/qdsa.cpp
  src/s2/ren_qte_event.cpp
  src/s2/ren_qte_results.cpp
  src/s2/ren_qte_capture.cpp
  src/s2/sleep_event.cpp
  src/s2/street_fight_event.cpp
  src/s2/event_call.cpp
  src/s2/seqp_event.cpp
  src/s2/legacy_hud_text.cpp
  src/s2/duck_race.cpp
  src/s2/search_runtime.cpp
  src/s2/search_prop_ik.cpp
  src/s2/search_commands.cpp
  src/s2/pati_nails.cpp
  src/s2/pati.cpp
  src/s2/darts.cpp
  src/s2/hand_grip.cpp
  src/s2/matrix_stack.cpp
  src/s2/pati_props.cpp
  src/s2/qte_game_entry.cpp
  src/s2/event_gameplay_queued.cpp
  src/s2/street_fight.cpp
  src/s2/street_fight_conversation.cpp
  src/s2/event_voice.cpp
  src/s2/sk_event.cpp
  src/s2/sap_event.cpp
  src/s2/sl_event.cpp
  src/s2/movie.cpp
  src/s2/telephone_cord.cpp
  src/s2/event_movement.cpp
  src/s2/osage.cpp
  src/s2/surface_wave.cpp
  src/s2/warehouse_script.cpp
  src/s2/event_smoke.cpp
  src/s2/qt_nj_event.cpp
  src/s2/qt_nj_obstacles.cpp
  src/s2/carry_event.cpp
  src/s2/darts_conversation.cpp
)
add_library(shenmue2_arcade_core STATIC
  src/s2/areas/game/harrier/control.cpp
  src/s1/modules/harrier_cpu.cpp
  src/s2/areas/game/harrier/render.cpp
  src/s2/areas/game/arcade/diagnostics.cpp
  src/s2/areas/game/arcade/sound.cpp
  src/s2/areas/game/arcade/audio_host.cpp
  src/s2/areas/game/arcade/host_data.cpp
  src/s2/areas/game/outrun/board.cpp
  src/s2/areas/game/outrun/sound.cpp
  src/s2/areas/game/outrun/render.cpp
  src/s2/areas/game/emulator/draw.cpp
  src/s2/areas/game/emulator/overlay.cpp
  src/s2/areas/game/afterburner/board.cpp
  src/s2/areas/game/afterburner/sound.cpp
  src/s2/areas/game/afterburner/render.cpp
  src/s2/areas/game/arcade/math.cpp
)
target_compile_definitions(shenmue2_arcade_core PRIVATE SHENMUE_TARGET_S2_PC=1 SHENMUE_LOAD_HANGON_DLL=1)
target_compile_features(shenmue2_arcade_core PRIVATE cxx_std_20)
target_include_directories(shenmue2_arcade_core PRIVATE src)
target_link_libraries(shenmue2_arcade_core PRIVATE shenmue_common shenmue_d3t ZLIB::ZLIB Threads::Threads ${SHENMUE_JSONCPP_TARGET})
if(MSVC)
  target_compile_options(shenmue2_arcade_core PRIVATE /Gy /Gw)
else()
  target_compile_options(shenmue2_arcade_core PRIVATE -ffunction-sections -fdata-sections)
endif()
if(WIN32)
  target_link_libraries(shenmue2_arcade_core PRIVATE d3d11 d3dcompiler dxgi dwmapi user32 gdi32 xinput9_1_0 xaudio2 ole32 uuid)
endif()

target_compile_definitions(shenmue2_game PUBLIC SHENMUE_TARGET_S2_PC=1)
target_compile_features(shenmue2_game PUBLIC cxx_std_20)
if(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS)
  target_sources(shenmue2_game PRIVATE src/s2/enhancements/map_seams.cpp)
  target_compile_definitions(shenmue2_game PRIVATE SHENMUE_ENHANCEMENT_S2_MAP_SEAMS=1)
endif()
target_include_directories(shenmue2_game PUBLIC src)
target_compile_definitions(shenmue2_events PUBLIC SHENMUE_TARGET_S2_PC=1)
target_compile_features(shenmue2_events PUBLIC cxx_std_20)
target_include_directories(shenmue2_events PUBLIC src)
target_link_libraries(shenmue2_events PUBLIC shenmue_common shenmue_d3t ZLIB::ZLIB)
if(WIN32)
  target_link_libraries(shenmue2_events PUBLIC shenmue_imgui)
endif()
shenmue_set_project_warnings(shenmue2_events)
target_link_libraries(shenmue2_game PUBLIC shenmue_common shenmue_d3t ZLIB::ZLIB PRIVATE shenmue2_arcade_core shenmue2_events)
if(WIN32)
  target_link_libraries(shenmue2_game PUBLIC d3d11 dxgi dwmapi user32 gdi32
                                             xinput9_1_0 xaudio2 ole32 uuid  shenmue_imgui)
endif()
shenmue_set_project_warnings(shenmue2_game)
get_target_property(shenmue2_cxx_options shenmue2_game COMPILE_OPTIONS)
set_property(TARGET shenmue2_game PROPERTY COMPILE_OPTIONS "$<$<COMPILE_LANGUAGE:CXX>:${shenmue2_cxx_options}>")

add_executable(shenmue2 EXCLUDE_FROM_ALL src/s2/main.cpp)
target_compile_features(shenmue2 PRIVATE cxx_std_20)
target_link_libraries(shenmue2 PRIVATE shenmue2_game)
if(MSVC)
  target_link_options(shenmue2 PRIVATE /MAP /OPT:REF)
else()
  target_link_options(shenmue2 PRIVATE -Wl,--gc-sections)
endif()
shenmue_set_project_warnings(shenmue2)
add_custom_command(TARGET shenmue2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_CURRENT_SOURCE_DIR}/reference/filename_database.json"
          "$<TARGET_FILE_DIR:shenmue2>/filename_database.json"
)



if(SHENMUE_STEAM)
  if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Steam ASIs require Windows x64")
  endif()
  enable_language(RC)
  set(steam_names_resource "${CMAKE_CURRENT_BINARY_DIR}/steam_names.rc")
  file(GENERATE OUTPUT "${steam_names_resource}" CONTENT
"#include \"shenmue_internal.h\"
101 RCDATA \"${CMAKE_CURRENT_SOURCE_DIR}/reference/filename_database.json\"
STRINGTABLE
BEGIN
  102 SHENMUE_COMMIT_HASH
END
")
  set_source_files_properties("${steam_names_resource}" PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/reference/filename_database.json")
  foreach(title IN ITEMS 1 2)
    get_target_property(steam_sources shenmue${title}_game SOURCES)
    add_library(shenmue${title}_steam_game STATIC ${steam_sources})
    foreach(property IN ITEMS COMPILE_DEFINITIONS COMPILE_OPTIONS INCLUDE_DIRECTORIES COMPILE_FEATURES LINK_LIBRARIES INTERFACE_LINK_LIBRARIES)
      get_target_property(value shenmue${title}_game ${property})
      if(value)
        if(property MATCHES "LINK_LIBRARIES")
          list(FILTER value EXCLUDE REGEX "GameHangon_import[.]lib")
        endif()
        set_property(TARGET shenmue${title}_steam_game PROPERTY ${property} "${value}")
      endif()
    endforeach()
    target_compile_definitions(shenmue${title}_steam_game PRIVATE SHENMUE_STEAM=1 SHENMUE_LOAD_HANGON_DLL=1)
    add_library(shenmue${title}_asi MODULE src/d3t/steam_entry.cpp src/d3t/steam.cpp src/d3t/steam_storage.cpp
      src/common/archive_data.cpp "${steam_names_resource}")
    target_compile_features(shenmue${title}_asi PRIVATE cxx_std_20)
    target_compile_definitions(shenmue${title}_asi PRIVATE SHENMUE_STEAM=1 SHENMUE_TARGET_S${title}_PC=1)
    target_include_directories(shenmue${title}_asi PRIVATE src)
    target_link_libraries(shenmue${title}_asi PRIVATE shenmue${title}_steam_game)
    if(title EQUAL 1)
      set(steam_name "shenmue1e")
    else()
      set(steam_name "shenmue2e")
    endif()
    set_target_properties(shenmue${title}_asi PROPERTIES PREFIX "" SUFFIX ".asi" OUTPUT_NAME "${steam_name}")
    if(MSVC)
      target_link_options(shenmue${title}_asi PRIVATE /MAP /OPT:REF /INCREMENTAL:NO)
    endif()
    shenmue_set_project_warnings(shenmue${title}_asi)
  endforeach()
endif()
