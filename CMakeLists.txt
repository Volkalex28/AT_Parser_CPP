cmake_minimum_required(VERSION 3.0)

project(Parser)

enable_language(C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCE 
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/Parser.cpp
  ${SOURCE_DIR}/esp8266.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>: -DDEBUG>
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${SOURCE_DIR}
)
