foreach(_language C CXX ASM)
  set(CMAKE_DEPFILE_FLAGS_${_language} "")
  set(CMAKE_${_language}_DEPENDS_USE_COMPILER FALSE)
endforeach()
