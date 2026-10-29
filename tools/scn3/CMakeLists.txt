if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.22)
  project(scn3cc LANGUAGES C CXX)
  include("${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CompilerWarnings.cmake")
else()
  enable_language(C)
endif()

find_package(Clang CONFIG REQUIRED)
if(MSVC AND TARGET LLVMDebugInfoPDB)
  set(SCN3_VS_ROOT "${CMAKE_GENERATOR_INSTANCE}")
  if(NOT SCN3_VS_ROOT)
    file(TO_CMAKE_PATH "$ENV{VSINSTALLDIR}" SCN3_VS_ROOT)
  endif()
  get_target_property(SCN3_PDB_LIBRARIES LLVMDebugInfoPDB INTERFACE_LINK_LIBRARIES)
  list(TRANSFORM SCN3_PDB_LIBRARIES REPLACE "^.*/DIA SDK/" "${SCN3_VS_ROOT}/DIA SDK/")
  set_target_properties(LLVMDebugInfoPDB PROPERTIES INTERFACE_LINK_LIBRARIES "${SCN3_PDB_LIBRARIES}")
endif()

add_library(scenario_lib STATIC
  backend/addressing.cpp
  backend/calls.cpp
  backend/c_expressions.cpp
  backend/expressions.cpp
  backend/linker.cpp
  backend/lowering.cpp
  backend/statements.cpp
  backend/stack.cpp
  backend/types.cpp

  bytecode/assembly.cpp
  bytecode/decode.cpp
  bytecode/module.cpp

  common/util.cpp

  decompiler/analysis.cpp
  decompiler/dataflow.cpp
  decompiler/cfg.cpp
  decompiler/assembly.cpp
  decompiler/ast_helpers.cpp
  decompiler/data_export.cpp
  decompiler/data_print.cpp
  decompiler/data_recovery.cpp
  decompiler/discovery.cpp
  decompiler/export.cpp
  decompiler/expression_recovery.cpp
  decompiler/lift.cpp
  decompiler/pretty.cpp
  decompiler/recovery.cpp
  decompiler/print.cpp
  decompiler/storage_recovery.cpp
  decompiler/structure.cpp
  decompiler/type_recovery.cpp

  frontend/c_clang.cpp
  frontend/c_frontend.cpp
  frontend/c_sdk.cpp
  frontend/c_types.cpp
  frontend/c_variadic.cpp
  frontend/data.cpp
  frontend/lexer.cpp
  frontend/parse_assembly.cpp
  frontend/parse_data.cpp
  frontend/parse_expressions.cpp
  frontend/parse_statements.cpp
  frontend/parser.cpp
  frontend/slot_names.cpp

  native/prototypes.cpp
  native/data.cpp
  native/lookup.cpp
  native/motion.cpp
  native/symbols.cpp

  sh4/emitter.cpp
  sh4/lift.cpp
  sh4/translate.cpp
)
target_compile_features(scenario_lib PUBLIC cxx_std_20)
find_package(Threads REQUIRED)
target_link_libraries(scenario_lib PUBLIC Threads::Threads)
target_compile_definitions(scenario_lib PRIVATE SHENMUE_TARGET_S1_PC=1)
if(WIN32)
  target_compile_definitions(scenario_lib PRIVATE NOMINMAX)
endif()
target_include_directories(scenario_lib SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../libs/ShenmueDK/include/shendk/files/model/gltf")
target_include_directories(scenario_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
shenmue_set_project_warnings(scenario_lib)
if(MSVC)
  target_compile_options(scenario_lib PRIVATE /bigobj)
endif()

target_include_directories(scenario_lib SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
if(TARGET clang-cpp)
  target_link_libraries(scenario_lib PRIVATE clang-cpp LLVM)
else()
  target_link_libraries(scenario_lib PRIVATE clangTooling)
endif()
if(NOT LLVM_ENABLE_RTTI)
  set_source_files_properties(frontend/c_clang.cpp PROPERTIES COMPILE_OPTIONS "$<IF:$<CXX_COMPILER_ID:MSVC>,/GR-,-fno-rtti>")
endif()
set(SCN3_CLANG_RESOURCE_DIR "${LLVM_LIBRARY_DIR}/clang/${LLVM_VERSION_MAJOR}")

add_executable(scn3cc cli/main.cpp cli/driver.cpp cli/diagnostics.cpp cli/disassembly.cpp)
target_compile_definitions(scn3cc PRIVATE SHENMUE_TARGET_S1_PC=1)
if(WIN32)
  target_compile_definitions(scn3cc PRIVATE NOMINMAX)
endif()
target_link_libraries(scn3cc PRIVATE scenario_lib)
shenmue_set_project_warnings(scn3cc)

set_source_files_properties(native/prototypes.cpp native/data.cpp PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/nuklear")

get_filename_component(SCN3_PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(SCN3_C_SDK "${CMAKE_CURRENT_BINARY_DIR}/include/scenario.h")
add_custom_command(TARGET scn3cc POST_BUILD
  BYPRODUCTS "${SCN3_C_SDK}"
  COMMAND $<TARGET_FILE:scn3cc> --write-c-header "${SCN3_C_SDK}"
  VERBATIM)
target_compile_definitions(scenario_lib PRIVATE
  SCN3_CLANG_RESOURCE_DIR="${SCN3_CLANG_RESOURCE_DIR}" SCN3_C_SDK_PATH="${CMAKE_CURRENT_BINARY_DIR}/include")
