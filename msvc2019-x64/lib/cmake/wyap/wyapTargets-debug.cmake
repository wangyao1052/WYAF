#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wyap" for configuration "Debug"
set_property(TARGET wyap APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wyap PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/wyapd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/wyapd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS wyap )
list(APPEND _IMPORT_CHECK_FILES_FOR_wyap "${_IMPORT_PREFIX}/lib/wyapd.lib" "${_IMPORT_PREFIX}/bin/wyapd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
