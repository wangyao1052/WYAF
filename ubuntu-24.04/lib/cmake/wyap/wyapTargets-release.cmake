#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wyap" for configuration "Release"
set_property(TARGET wyap APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(wyap PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libwyap.so"
  IMPORTED_SONAME_RELEASE "libwyap.so"
  )

list(APPEND _cmake_import_check_targets wyap )
list(APPEND _cmake_import_check_files_for_wyap "${_IMPORT_PREFIX}/lib/libwyap.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
