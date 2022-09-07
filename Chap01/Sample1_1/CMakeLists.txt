cmake_minimum_required(VERSION 3.5)

project(Sample1_1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# 把.cpp和.c文件都塞到SOURCE_FILES里
file(GLOB SOURCE_FILES "*.cpp" "*.c" )
# 把.cpp和.c文件都塞到HEADER_FILES里
file(GLOB HEADER_FILES "*.h" "*.hpp" )
# 把.ui文件都塞到HEADER_FILES里
file(GLOB UI_FILES "*.ui" )

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(Sample1_1 SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
  )
else()
  add_executable(Sample1_1
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
  )
endif()

target_link_libraries(Sample1_1 PRIVATE Qt5::Widgets)
