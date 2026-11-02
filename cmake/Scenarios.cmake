option(SHENMUE_BUILD_SCENARIOS
  "Build and compile Shenmue scenario scripts" OFF)

if(NOT SHENMUE_BUILD_SCENARIOS)
  return()
endif()

if(CMAKE_CROSSCOMPILING)
  return()
endif()

set(SHENMUE_SCENARIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/s1/scenario" CACHE PATH "SCN3 scenario sources")
file(GLOB_RECURSE scenario_inputs CONFIGURE_DEPENDS "${SHENMUE_SCENARIO_DIR}/*.scn" "${SHENMUE_SCENARIO_DIR}/*.c")
set(scenario_game_root "")
foreach(variable IN ITEMS SHEN_STEAM_ROOT SHEN_ROOT)
  if(NOT "$ENV{${variable}}" STREQUAL "")
    file(TO_CMAKE_PATH "$ENV{${variable}}/sm1" scenario_game_root)
    if(NOT IS_DIRECTORY "${scenario_game_root}/archives/dx11")
      message(FATAL_ERROR "${variable} does not contain sm1/archives/dx11")
    endif()
    break()
  endif()
endforeach()
if(NOT scenario_inputs AND NOT scenario_game_root)
  return()
endif()

if(NOT TARGET scn3cc)
  add_subdirectory(tools/scn3)
endif()
add_subdirectory(tools/archive)
add_subdirectory(tools/motion)
get_filename_component(scenario_source_root "${SHENMUE_SCENARIO_DIR}" DIRECTORY)
set(scenario_script "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildScenarios.cmake")
set(scenario_work "${CMAKE_CURRENT_BINARY_DIR}/scenarios")
set(scenario_arguments
  "-DSOURCE_DIR=${SHENMUE_SCENARIO_DIR}"
  "-DSOURCE_ROOT=${scenario_source_root}"
  "-DWORK_DIR=${scenario_work}"
  "-DCOMPILER=$<TARGET_FILE:scn3cc>")

add_custom_target(shenmue_scenarios
  COMMAND "${CMAKE_COMMAND}" ${scenario_arguments} -DMODE=prepare
    "-DGAME_ROOT=${scenario_game_root}"
    "-DCATALOGUE=${CMAKE_CURRENT_SOURCE_DIR}/reference/filename_database.json"
    "-DEXTRACTOR=$<TARGET_FILE:tacextract>" "-DMOTION=$<TARGET_FILE:motextract>"
    -P "${scenario_script}"
  DEPENDS scn3cc tacextract motextract
  VERBATIM)

foreach(target IN ITEMS shenmue1 shenmue1_asi)
  if(TARGET ${target})
    add_custom_target(${target}_scenarios
      COMMAND "${CMAKE_COMMAND}" ${scenario_arguments} -DMODE=compile
        "-DOUTPUT_DIR=$<TARGET_FILE_DIR:${target}>/scenario" -P "${scenario_script}"
      DEPENDS shenmue_scenarios
      VERBATIM)
    add_dependencies(${target} ${target}_scenarios)
  endif()
endforeach()
