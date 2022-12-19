#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "KimeraRPGO" for configuration "Release"
set_property(TARGET KimeraRPGO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(KimeraRPGO PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libKimeraRPGO.so"
  IMPORTED_SONAME_RELEASE "libKimeraRPGO.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS KimeraRPGO )
list(APPEND _IMPORT_CHECK_FILES_FOR_KimeraRPGO "${_IMPORT_PREFIX}/lib/libKimeraRPGO.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
