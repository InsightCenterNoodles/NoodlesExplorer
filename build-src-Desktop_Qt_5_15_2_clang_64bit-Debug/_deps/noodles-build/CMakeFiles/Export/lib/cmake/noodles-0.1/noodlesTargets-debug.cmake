#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "noodles::noodles" for configuration "Debug"
set_property(TARGET noodles::noodles APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(noodles::noodles PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/noodles-0.1/libnoodles.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libnoodles.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS noodles::noodles )
list(APPEND _IMPORT_CHECK_FILES_FOR_noodles::noodles "${_IMPORT_PREFIX}/lib/noodles-0.1/libnoodles.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
