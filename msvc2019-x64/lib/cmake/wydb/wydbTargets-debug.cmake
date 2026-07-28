#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wydb" for configuration "Debug"
set_property(TARGET wydb APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wydb PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/wydbd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/wydbd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS wydb )
list(APPEND _IMPORT_CHECK_FILES_FOR_wydb "${_IMPORT_PREFIX}/lib/wydbd.lib" "${_IMPORT_PREFIX}/bin/wydbd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
