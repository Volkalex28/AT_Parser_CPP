project (Tests)

cmake_minimum_required(VERSION 3.0)

enable_language(C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# include project source
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/cpputest/include)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_executable(${PROJECT_NAME} 
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/src/functionTests.cpp
  ${PROJECT_SOURCE_DIR}/src/parserTests.cpp
)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/cpputest/cpputest_build/src/CppUTest/libCppUTest.a)

include(ExternalProject)

ExternalProject_Add(
  cpputest
  PREFIX            ${CMAKE_SOURCE_DIR}/cpputest
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/cpputest
  BINARY_DIR        ${CMAKE_SOURCE_DIR}/cpputest/cpputest_build
  STAMP_DIR         ${CMAKE_SOURCE_DIR}/cpputest/cpputest_build
  TMP_DIR           ${CMAKE_SOURCE_DIR}/cpputest/cpputest_build
  DOWNLOAD_COMMAND  ""
  PATCH_COMMAND     ""
  INSTALL_COMMAND   ""
  UPDATE_COMMAND    ""
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S ${CMAKE_SOURCE_DIR}/cpputest -G ${CMAKE_GENERATOR} -DC++11=ON -DTESTS=OFF -DTESTS_BUILD_DISCOVER=OFF
  BUILD_ALWAYS      TRUE
  ALWAYS            TRUE
  DEPENDS           ${PROJECT_NAME}
)
