#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wyap" for configuration "Debug"
set_property(TARGET wyap APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wyap PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libwyapd.so"
  IMPORTED_SONAME_DEBUG "libwyapd.so"
  )

list(APPEND _cmake_import_check_targets wyap )
list(APPEND _cmake_import_check_files_for_wyap "${_IMPORT_PREFIX}/lib/libwyapd.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
