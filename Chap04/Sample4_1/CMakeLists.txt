cmake_minimum_required(VERSION 3.5)

project(Sample4_1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 把.cpp和.c文件都塞到SOURCE_FILES里
file(GLOB SOURCE_FILES "*.cpp" "*.c" )
# 把.cpp和.c文件都塞到HEADER_FILES里
file(GLOB HEADER_FILES "*.h" "*.hpp" )
# 把.ui文件都塞到HEADER_FILES里
file(GLOB UI_FILES "*.ui" )


find_package(Qt5 COMPONENTS Widgets REQUIRED)


# Qt Cmake添加*.qrc资源文件
file(GLOB QRC_SOURCE_FILES "*.qrc" )
set(QRC_SOURCE_FILES res.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
SOURCE_GROUP("Resource Files" FILES ${QRC_SOURCE_FILES})


if(ANDROID)
  add_library(Sample4_1 SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    ${QRC_FILES}
  )
else()
  add_executable(Sample4_1
    ${QRC_FILES}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
  )
endif()

target_link_libraries(Sample4_1 PRIVATE Qt5::Widgets)
