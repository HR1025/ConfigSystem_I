cmake_minimum_required(VERSION 3.5)

project(ConfigSystem_I LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core LinguistTools REQUIRED)

set(TS_FILES ConfigSystem_I_zh_CN.ts)

add_executable(ConfigSystem_I
  main.cpp
  ${TS_FILES}
)
target_link_libraries(ConfigSystem_I Qt5::Core)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
