cmake_minimum_required(VERSION 3.18.4)
project(renderer)

set(
  SOURCES
  src/main.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")  # force .exe to build/bin

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC lib/include)
target_include_directories(${PROJECT_NAME} PUBLIC src)

file(COPY res/ DESTINATION bin/res/)  # copy resources to bin
