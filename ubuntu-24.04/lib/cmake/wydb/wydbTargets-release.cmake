#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wydb" for configuration "Release"
set_property(TARGET wydb APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(wydb PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libwydb.so"
  IMPORTED_SONAME_RELEASE "libwydb.so"
  )

list(APPEND _cmake_import_check_targets wydb )
list(APPEND _cmake_import_check_files_for_wydb "${_IMPORT_PREFIX}/lib/libwydb.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
