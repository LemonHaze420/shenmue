set(_shenmue_dc_executable shenmue2_dc)
set(_shenmue_dc_core shenmue2_dc_core)
set(_shenmue_dc_linker "${PROJECT_SOURCE_DIR}/src/platform/dc/shenmue2.ld")

add_library(shenmue2_dc_core STATIC
  src/s2/hlib.cpp src/s2/pak_dc.cpp src/s2/character_table_dc.cpp src/s2/character_dc.cpp src/s2/map_characters_dc.cpp src/s2/audio_partition_dc.cpp src/s2/event_overlay.cpp src/s2/input_dc.cpp src/s2/pad_control_dc.cpp src/platform/dc/input.cpp src/s2/frame_dc.cpp src/s2/screen_film.cpp src/s2/system_modules_dc.cpp
  src/s2/scene_path.cpp src/s2/scene_text_heap.cpp src/platform/dc/disc.cpp src/platform/dc/simm.cpp src/s2/directory_cache.cpp src/s2/file_loader_dc.cpp
  src/s2/full_map_menu.cpp
  src/s2/legacy_collection.cpp
  src/s2/face_mouth_dc.cpp
  src/s2/face_mouth_shapes_dc.cpp
  src/s2/hand_staging_dc.cpp
  src/s2/hand_matrix_dc.S
  src/s2/hand_skin_dc.cpp
  src/s2/hand_skin_normals_dc.S
  src/s2/handcuffs.cpp src/s2/handcuffs_render_dc.cpp src/s2/telephone_cord.cpp
  src/s2/character_transparency_dc.cpp
  src/platform/dc/mt7_vertex_colours.S
 src/s2/button_mediation_dc.cpp
 src/s2/help_icon_dc.cpp
 src/s2/flag_sequence_dc.cpp src/s2/flag.cpp
  src/s2/world_render.cpp src/s2/character.cpp src/s2/object_draw_dc.cpp
  src/s2/field_depth_dc.cpp src/s2/prop_draw_dc.cpp src/s2/map_draw_dc.cpp src/s2/map_command_cells_dc.cpp src/s2/map_command_records_dc.cpp
  src/s2/map_load_dc.cpp
 src/s2/modifier_volume_dc.cpp src/s2/modifier_vertices_dc.cpp
 src/s2/modifier_triangles_dc.cpp src/common/matrix_inverse_dc.cpp
  src/s2/scenario_scnt.cpp
  src/s2/search_state_dc.cpp
 src/s2/search_prop_ik.cpp
  src/s2/player.cpp src/s2/party_companion.cpp
  src/s2/player_control_state_dc.cpp
  src/s2/player_game_flags_dc.cpp
  src/s2/player_control_enabled_dc.cpp
  src/s2/player_movement_dc.cpp
  src/s2/player_held_air_dc.cpp src/s2/door_state_dc.cpp
  src/s2/scenario_control_dc.cpp
  src/s2/character_definition_dc.cpp
  src/s2/character_control.cpp
  src/s2/character_control_constructor_dc.cpp
  src/s2/character_control_update_dc.cpp
  src/s2/character_control_ground_dc.cpp
  src/s2/player_ground_state_dc.cpp
  src/s2/field_sphere_dc.cpp
  src/s2/field_sphere_grid_dc.cpp
 src/s2/field_sphere_volume_grid_dc.cpp
  src/s2/minigame_random.cpp
  src/s2/scenario_character_query_dc.cpp
  src/s2/character_collision_sort_dc.cpp src/s2/character_collision_pair_dc.cpp
  src/s2/hand_state_dc.cpp
  src/s2/hand.cpp
  src/s2/reflector_board.cpp src/s2/reflector_board_draw_dc.cpp src/s2/reflector_board_light_dc.cpp
  src/s2/hand_model.cpp
  src/s2/hand_pose.cpp
  src/s2/face_controller_dc.cpp
  src/s2/face_morph_dc.cpp
  src/s2/face_eyelids_dc.cpp
  src/s2/face_eyes_dc.cpp
  src/s2/footstep.cpp
  src/s2/game_display.cpp
  src/s2/game_system_dc.cpp
 src/s2/object_light_dc.cpp src/s2/area_jump_geometry_dc.cpp src/s2/area_jump_state_dc.cpp
 src/s2/easy_option_state_dc.cpp
 src/s2/time_hud_gate_dc.cpp src/s2/time_compass_dc.cpp src/s2/time_hud_draw_dc.cpp
  src/s2/author_controller_dc.cpp
 src/s2/conversation_index_dc.cpp src/s2/conversation_stream_dc.cpp src/s2/conversation_callbacks_dc.cpp
 src/s2/character_state_flags_dc.cpp
  src/s2/door_control_dc.cpp
  src/s2/player_flags_dc.cpp
  src/s2/lcdm_dc.cpp src/s2/lcdm.cpp
  src/s2/title.cpp
  src/s2/title_control_dc.cpp
  src/s2/save_load_dc.cpp
  src/s2/title_sequence_dc.cpp
  src/s2/title_sprite_dc.cpp
  src/s2/title_warning_dc.cpp
  src/s2/backup_metadata_dc.cpp
  src/s2/sound_table_dc.cpp
  src/s2/sound_program.cpp
  src/s2/particle_effects.cpp src/s2/effect_fft.cpp src/s2/hiki.cpp
  src/platform/dc/particle_parameters.cpp src/platform/dc/fft_parameters.cpp
  src/s2/event_abi_dc.cpp
  src/s2/event_character_transform_dc.cpp
  src/s2/event_character_state_dc.cpp
  src/s2/event_character_model_dc.cpp
  src/s2/event_package_dc.cpp
  src/s2/event_character_control_dc.cpp
  src/s2/event_matrix_dc.cpp
  src/s2/event_vector_dc.cpp
  src/s2/character_pose_dc.cpp
  src/s2/model_transform_dc.cpp
  src/s2/string_utility.cpp
  src/s2/weather_wind_dc.cpp
  src/s2/weather_noise_dc.cpp
  src/s2/lflt_dc.cpp
  src/s2/lrgb_dc.cpp
  src/s2/weather_execution_dc.cpp src/s2/weather.cpp
  src/platform/dc/fog.cpp
  src/s2/fog.cpp
  src/s2/bicycle.cpp
  src/s2/navigation_map.cpp
  src/s2/help_sprite.cpp
  src/s2/timed_motion.cpp
  src/s2/debug_tree.cpp
  src/s2/orientation_arc.cpp
  src/s2/title_save_import.cpp
  src/s2/fight_placement.cpp
  src/s2/collision_primitives.cpp
  src/platform/dc/trigonometry.cpp
  src/s2/lens_flare_dc.cpp
  src/s2/lighting_publication_dc.cpp
  src/s2/lighting_geometry_dc.cpp
  src/platform/dc/mt7_geometry.cpp
  src/s2/lighting_task_dc.cpp
  src/s2/lighting_definitions_dc.cpp
  src/s2/osage.cpp
  src/s2/cloth.cpp
  src/s2/cloth_geometry_dc.cpp
  src/s2/blur_figure_pose.cpp
  src/s2/blur_figure_pose_core.cpp
 src/s2/fight.cpp
  src/s2/gauge.cpp
  src/s2/debug_screen_dc.cpp
  src/s2/legacy_hud_text.cpp
  src/s2/magic_room_generation.cpp
  src/s2/air_state_dc.cpp
  src/s2/weather_air_dc.cpp
  src/s2/weather_light_dc.cpp
  src/s2/weather_timeline_dc.cpp
  src/s2/data_list_dc.cpp
  src/s2/weather_fixc_dc.cpp
  src/s2/weather_clip_dc.cpp
  src/s2/weather_group_dc.cpp
  src/s2/weather_rain_dc.cpp
  src/s2/debug.cpp
  src/s2/texture_cache_dc.cpp
  src/s2/weather_table_dc.cpp
  src/s2/model_render_dc.cpp
  src/platform/dc/mt7_draw.cpp
  src/platform/dc/mt7_projection.cpp
  src/platform/dc/mt7_materials.cpp
  src/platform/dc/mt7_prepare.cpp
  src/platform/dc/mt7_matrix.S
  src/platform/dc/mt7_packets.S
  src/platform/dc/mt7_float_lights.S
  src/platform/dc/mt7_packed_lights.S
  src/platform/dc/am2_vector.S
  src/s2/model_texture_coordinates_dc.cpp
  src/s2/palette_dc.cpp
  src/s2/model_vertex_links_dc.cpp
  src/s2/character_transform_dc.cpp
  src/s2/camera_state_dc.cpp
  src/s2/camera_sequence_dc.cpp
  src/s2/camera_conversation_dc.cpp
  src/s2/camera_fov.cpp
  src/s2/camera_parameters.cpp
  src/s2/math.cpp
  src/s2/object_xforms.cpp
  src/s2/character_draw_dc.cpp
  src/s2/map_event_character_dc.cpp
 src/s2/player_readiness_dc.cpp
  src/s2/qron_transition_dc.cpp
  src/s2/motion_work_dc.cpp src/s2/motion_bone_query_dc.cpp
  src/s2/motion_region_dc.cpp
  src/s2/motion_stream_dc.cpp
  src/s2/motion_channel_dc.cpp
  src/s2/motion_node_dc.cpp
  src/s2/model_image_component_dc.cpp
  src/s2/model_clone.cpp
  src/s2/model_image_animation_dc.cpp
 src/s2/graphics_dc.cpp src/s2/map_clip_dc.cpp src/s2/face_data_dc.cpp src/s2/shadow_dc.cpp src/s2/shadow_cast_dc.c src/s2/shadow_range.cpp src/s2/flag_wind_dc.cpp src/s2/cold_archive.cpp src/s2/map_render_profile.cpp src/s2/player_state_dc.cpp src/s2/player_config_dc.cpp src/s2/ui_state_dc.cpp src/s2/ui_legacy_icons.cpp src/s2/story_state.cpp src/s2/scenario_flags.cpp src/s2/arcade_rankings_dc.cpp src/s2/calendar_dc.cpp
 src/s2/colour_bar_dc.cpp src/s2/scene_assets_dc.cpp src/platform/dc/weather_texture.cpp src/s2/field_collision.cpp src/s2/texture_motion.cpp src/s2/texture_dc.cpp
  src/s2/motion_registry_dc.cpp src/s1/motion_file_dc.cpp src/s2/map_chunk.cpp src/s2/npc_data_dc.cpp src/s2/lighting.cpp src/s2/aica_dma_dc.cpp src/s2/legacy_sprite.cpp src/s2/load_title_dc.cpp src/s2/load_title_draw_dc.cpp src/s2/legacy_sprite_slot.cpp src/s2/conversation.cpp src/platform/dc/audio.cpp src/platform/dc/aica.cpp src/platform/dc/aica_stream.cpp src/platform/dc/runtime_arithmetic.cpp
  src/s2/load_title.cpp
  src/platform/dc/sdk.cpp src/platform/dc/cache.cpp src/platform/dc/matrix.S src/platform/dc/package.cpp src/platform/dc/screen.cpp src/common/matrix_stack.cpp src/common/math.cpp)
if(SHENMUE_DC_TARGET MATCHES "^S2_")
  target_sources(shenmue2_dc_core PRIVATE
    src/s2/fight_reaction.cpp
    src/s2/fight_action.cpp
    src/s2/ui_credits.cpp
    src/s2/pause.cpp src/s2/save.cpp src/s2/warning.cpp src/s2/fight_stage.cpp src/s2/dynamic_tracks.cpp src/s2/elevator.cpp src/s2/advice.cpp
    src/s2/tree.cpp
    src/s2/npc_package_streaming.cpp
    src/s2/npc_package_routes.cpp
    src/s2/npc_area_route.cpp
    src/s2/motion.cpp src/s2/motion_frame.cpp src/s2/motion_rig.cpp)
  if(NOT SHENMUE_DC_GCC2952)
    set_property(SOURCE src/s2/world_render.cpp src/s2/lens_flare_dc.cpp src/s2/math.cpp src/s2/motion_rig.cpp APPEND PROPERTY COMPILE_OPTIONS -fno-math-errno)
  endif()
endif()
target_sources(shenmue2_dc_core PRIVATE
  src/platform/dc/particles.cpp src/platform/dc/particle_simulation.cpp src/s2/navi.cpp src/s2/fight_dialogue.cpp
  src/s2/battle_qte_sprites.cpp src/s2/battle_qte.cpp src/s2/extra_telop.cpp src/s2/fight_object_motion.cpp src/s2/fight_main.cpp src/s2/fight_motion.cpp src/s2/fight_collision.cpp src/s2/help.cpp src/s2/player_input.cpp src/s2/maze.cpp
  src/s2/npc_package.cpp src/s2/npc_package_schedule.cpp src/s2/npc_package_simulation.cpp src/s2/npc_package_transition.cpp
  src/s2/player_companions.cpp src/s2/player_movement.cpp src/s2/player_held.cpp src/s2/player_placement.cpp src/s2/player_collision.cpp
  src/s2/title_save_draw_dc.cpp src/s2/date_time.cpp src/s2/memo.cpp src/s2/navigation.cpp src/s2/pawn.cpp src/s2/pawn_shop.cpp
  src/s2/moves_scroll.cpp src/s2/item_wf.cpp src/s1/task/effe_draw_packets_dc.cpp
  src/platform/dc/particle_fft.cpp src/platform/dc/particle_effects.cpp
  src/platform/dc/particle_renderer.cpp src/platform/dc/particle_packets.cpp)

target_include_directories(shenmue2_dc_core PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_include_directories(shenmue2_dc_core PRIVATE "${_dc_zlib_dir}")
target_compile_definitions(shenmue2_dc_core PUBLIC ${_shenmue_dc_definition})
if(NOT SHENMUE_DC_GCC2952)
  target_compile_features(shenmue2_dc_core PUBLIC cxx_std_20)
endif()
target_compile_options(shenmue2_dc_core PRIVATE -ffunction-sections -fdata-sections -fno-math-errno)

add_executable(shenmue2_dc EXCLUDE_FROM_ALL
  src/platform/dc/start.S src/platform/dc/runtime.cpp src/s2/startup_dc.cpp src/s2/event_function_table_dc.cpp)
set(_s2_debug_font "${CMAKE_CURRENT_BINARY_DIR}/resources/debug_screen_font.bin")
set(_s2_debug_font_object "${CMAKE_CURRENT_BINARY_DIR}/resources/debug_screen_font.obj")
set(SHENMUE_DC_DEBUG_FONT_SOURCE "" CACHE FILEPATH "External Dreamcast debug font binary")
if(SHENMUE_DC_DEBUG_FONT_SOURCE)
  if(NOT EXISTS "${SHENMUE_DC_DEBUG_FONT_SOURCE}")
    message(FATAL_ERROR "Missing debug font: ${SHENMUE_DC_DEBUG_FONT_SOURCE}")
  endif()
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources")
  configure_file("${SHENMUE_DC_DEBUG_FONT_SOURCE}" "${_s2_debug_font}" COPYONLY)
elseif(NOT EXISTS "${_s2_debug_font}")
  message(FATAL_ERROR "Set SHENMUE_DC_DEBUG_FONT_SOURCE to the external Dreamcast debug font binary")
endif()
add_custom_command(OUTPUT "${_s2_debug_font_object}"
  COMMAND "${CMAKE_OBJCOPY}" -I binary -O elf32-shl -B sh
    --rename-section .data=.rodata,alloc,load,readonly,data,contents --set-section-alignment .data=4
    --redefine-sym _binary_debug_screen_font_bin_start=_shenmue_s2_debug_screen_font
    debug_screen_font.bin "${_s2_debug_font_object}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources"
  DEPENDS "${_s2_debug_font}"
  VERBATIM)
set_source_files_properties("${_s2_debug_font_object}" PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE)
target_sources(shenmue2_dc PRIVATE "${_s2_debug_font_object}")
target_compile_options(shenmue2_dc PRIVATE -ffunction-sections -fdata-sections -fno-math-errno)
target_link_options(shenmue2_dc PRIVATE -nostartfiles -nostdlib
  "-Wl,-EL,--defsym,__dc_segment=${_shenmue_dc_segment},-T,${_shenmue_dc_linker},--gc-sections,-Map,${CMAKE_CURRENT_BINARY_DIR}/shenmue2_dc.map")
target_link_libraries(shenmue2_dc PRIVATE shenmue2_dc_core)
set_target_properties(shenmue2_dc PROPERTIES SUFFIX ".elf" LINK_DEPENDS "${_shenmue_dc_linker}")
add_custom_command(TARGET shenmue2_dc POST_BUILD
  COMMAND "${CMAKE_OBJCOPY}" -O binary --gap-fill 0 --pad-to ${_s2_image_end}
    "$<TARGET_FILE:shenmue2_dc>" "${CMAKE_CURRENT_BINARY_DIR}/1ST_READ.BIN"
  BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/1ST_READ.BIN"
  VERBATIM)

include("${CMAKE_CURRENT_LIST_DIR}/EventOverlays.cmake")
