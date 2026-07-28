#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wydb" for configuration "Release"
set_property(TARGET wydb APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(wydb PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/wydb.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/wydb.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS wydb )
list(APPEND _IMPORT_CHECK_FILES_FOR_wydb "${_IMPORT_PREFIX}/lib/wydb.lib" "${_IMPORT_PREFIX}/bin/wydb.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
