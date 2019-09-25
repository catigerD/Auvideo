cmake_minimum_required(VERSION 3.4.1)

# set path
set(PATH_WORKSPACE ${CMAKE_CURRENT_SOURCE_DIR})

set(PATH_COMMON ${PATH_WORKSPACE}/common)
set(PATH_COMMON_EGL_CORE ${PATH_COMMON}/egl_core)

set(PATH_JNI ${PATH_WORKSPACE}/jni)

#include header
include_directories(${PATH_COMMON})
include_directories(${PATH_COMMON_EGL_CORE})

include_directories(${PATH_JNI})

# set file path
file(GLOB FILES_LIB_COMMON ${PATH_COMMON}/*.cpp)
file(GLOB FILES_LIB_COMMON_EGL_CORE ${PATH_COMMON_EGL_CORE}/*.cpp)

file(GLOB FILES_LIB_JNI ${PATH_JNI}/*.cpp)

# add lib
add_library(
        core
        SHARED
        ${FILES_LIB_COMMON}
        ${FILES_LIB_COMMON_EGL_CORE}
        ${FILES_LIB_JNI}
)

# link target
target_link_libraries(
        core
        log
        android
        GLESv2
        EGL
        z
        OpenSLES
)

