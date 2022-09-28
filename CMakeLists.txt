cmake_minimum_required(VERSION 3.23)
project(sires VERSION 1.0.0 LANGUAGES C CXX)

# Harmless but useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(sires STATIC)
target_compile_features(sires PUBLIC cxx_std_20)
target_compile_options(sires PUBLIC
  $<$<PLATFORM_ID:Darwin>:-fobjc-arc>
  $<$<PLATFORM_ID:iOS>:-fobjc-arc>)
target_link_libraries(sires PUBLIC
  "$<$<PLATFORM_ID:Darwin>:-framework Foundation>"
  "$<$<PLATFORM_ID:iOS>:-framework Foundation>")
target_link_options(sires PUBLIC
  $<$<PLATFORM_ID:iOS>:-ObjC>)
target_sources(sires PRIVATE
  $<$<PLATFORM_ID:Darwin>:apple.mm>
  $<$<PLATFORM_ID:iOS>:apple.mm>
  $<$<PLATFORM_ID:Windows>:windows.cpp>
  $<$<STREQUAL:${CMAKE_SYSTEM_NAME},WASI>:wasm.cpp>)
target_sources(sires INTERFACE FILE_SET HEADERS FILES sires.hpp)

add_executable(sires-poc MACOSX_BUNDLE poc.txt)
target_link_libraries(sires-poc PUBLIC sires)
target_sources(sires-poc PRIVATE poc.cpp)
set_target_properties(sires-poc PROPERTIES RESOURCE poc.txt)
