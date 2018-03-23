project(timerfix)

cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(AMXConfig)
include(AddSAMPPlugin)

add_definitions (-DPLUGIN_NAME="${PROJECT_NAME}")

add_samp_plugin(${PROJECT_NAME}
  src/amxplugin.cpp
  src/amxplugin2.cpp
  src/Timer.cpp
  src/Timer.hpp
  src/natives.hpp
  src/natives.cpp
  src/main.hpp
  src/main.cpp
  src/main.def
)

target_include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/amx
)