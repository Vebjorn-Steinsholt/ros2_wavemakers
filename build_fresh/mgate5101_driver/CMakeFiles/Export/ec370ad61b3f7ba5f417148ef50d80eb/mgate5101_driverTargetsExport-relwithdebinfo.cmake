#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mgate5101_driver::mgate5101" for configuration "RelWithDebInfo"
set_property(TARGET mgate5101_driver::mgate5101 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mgate5101_driver::mgate5101 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libmgate5101.a"
  )

list(APPEND _cmake_import_check_targets mgate5101_driver::mgate5101 )
list(APPEND _cmake_import_check_files_for_mgate5101_driver::mgate5101 "${_IMPORT_PREFIX}/lib/libmgate5101.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
