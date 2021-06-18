#CMake最低版本号要求
cmake_minimum_required(VERSION 3.5)

#指定项目名称
project(ConfigSystem_I LANGUAGES CXX)

set(TS_FILES ConfigSystem_I_zh_CN.ts)

#QT5自带设置
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#添加包
find_package(Qt5 COMPONENTS Core LinguistTools REQUIRED)

#查找src目录下的所有源文件，并将名称保存到SRCS目录
aux_source_directory(./src SRCS)

#指定头文件目录,PROJECT_SOURCE_DIR为工程的根目录
include_directories(${PROJECT_SOURCE_DIR}/include)

#指定库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(ConfigSystem_I
  ${SRCS}
  ${TS_FILES}
)

#链接共享库
target_link_libraries(ConfigSystem_I ${PROJECT_SOURCE_DIR}/lib/yaml/libyaml-cpp.a)
target_link_libraries(ConfigSystem_I Qt5::Core)
target_link_libraries(ConfigSystem_I -lpthread)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
