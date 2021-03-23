cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

ExternalProject_Add(
  unit_tests
  PREFIX            ${CMAKE_SOURCE_DIR}/tests
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/tests
  BINARY_DIR        ${CMAKE_SOURCE_DIR}/tests/build
  STAMP_DIR         ${CMAKE_SOURCE_DIR}/tests/build
  TMP_DIR           ${CMAKE_SOURCE_DIR}/tests/build
  DOWNLOAD_COMMAND  ""
  PATCH_COMMAND     ""
  INSTALL_COMMAND   ""
  UPDATE_COMMAND    ""
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S ${CMAKE_SOURCE_DIR}/tests -G ${CMAKE_GENERATOR}
  BUILD_ALWAYS      TRUE
  ALWAYS            TRUE
)
