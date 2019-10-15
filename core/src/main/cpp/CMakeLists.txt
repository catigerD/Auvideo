cmake_minimum_required(VERSION 3.4.1)

# set path
set(PATH_WORKSPACE ${CMAKE_CURRENT_SOURCE_DIR})

set(PATH_COMMON ${PATH_WORKSPACE}/common)
set(PATH_COMMON_EGL_CORE ${PATH_COMMON}/egl_core)
set(PATH_COMMON_OPENGL_MEDIA ${PATH_COMMON}/opengl_media)
set(PATH_COMMON_OPENGL_MEDIA_RENDER ${PATH_COMMON_OPENGL_MEDIA}/render)
set(PATH_COMMON_OPENGL_MEDIA_TEXTURE ${PATH_COMMON_OPENGL_MEDIA}/texture)
set(PATH_COMMON_OPENGL_MEDIA_TEXTURE_COPIER ${PATH_COMMON_OPENGL_MEDIA}/texture_copier)
set(PATH_COMMON_LOOPER ${PATH_COMMON}/looper)
set(PATH_CAMERA_PREVIEW ${PATH_WORKSPACE}/camera_preview)
set(PATH_VIDEO_ENCODER ${PATH_WORKSPACE}/video_encoder)
set(PATH_SOFT_ENCODER ${PATH_VIDEO_ENCODER}/soft_encoder)

set(PATH_JNI ${PATH_WORKSPACE}/jni)

set(PATH_THIRD_PARTY ${PATH_WORKSPACE}/thirdparty)
set(PATH_TO_PRE_BUILT ${PATH_THIRD_PARTY}/prebuilt/${ANDROID_ABI})
set(PATH_FFMEPG_INCLUDE ${PATH_THIRD_PARTY}/ffmpeg/include)

#include header
include_directories(${PATH_COMMON})
include_directories(${PATH_COMMON_EGL_CORE})
include_directories(${PATH_COMMON_OPENGL_MEDIA})
include_directories(${PATH_COMMON_OPENGL_MEDIA_RENDER})
include_directories(${PATH_COMMON_OPENGL_MEDIA_TEXTURE})
include_directories(${PATH_COMMON_OPENGL_MEDIA_TEXTURE_COPIER})
include_directories(${PATH_COMMON_LOOPER})
include_directories(${PATH_CAMERA_PREVIEW})
include_directories(${PATH_VIDEO_ENCODER})
include_directories(${PATH_SOFT_ENCODER})

include_directories(${PATH_JNI})

include_directories(${PATH_FFMEPG_INCLUDE})

# set file path
file(GLOB FILES_LIB_COMMON ${PATH_COMMON}/*.cpp)
file(GLOB FILES_LIB_COMMON_EGL_CORE ${PATH_COMMON_EGL_CORE}/*.cpp)
file(GLOB FILES_LIB_COMMON_OPENGL_MEDIA ${PATH_COMMON_OPENGL_MEDIA}/*.cpp)
file(GLOB FILES_LIB_COMMON_OPENGL_MEDIA_RENDER ${PATH_COMMON_OPENGL_MEDIA_RENDER}/*.cpp)
file(GLOB FILES_LIB_COMMON_OPENGL_MEDIA_TEXTURE ${PATH_COMMON_OPENGL_MEDIA_TEXTURE}/*.cpp)
file(GLOB FILES_LIB_COMMON_OPENGL_MEDIA_TEXTURE_COPIER ${PATH_COMMON_OPENGL_MEDIA_TEXTURE_COPIER}/*.cpp)
file(GLOB FILES_LIB_COMMON_LOOPER ${PATH_COMMON_LOOPER}/*.cpp)
file(GLOB FILES_LIB_CAMERA_PREVIEW ${PATH_CAMERA_PREVIEW}/*.cpp)
file(GLOB FILES_LIB_VIDEO_ENCODER ${PATH_VIDEO_ENCODER}/*.cpp)
file(GLOB FILES_LIB_SOFT_ENCODER ${PATH_SOFT_ENCODER}/*.cpp)

file(GLOB FILES_LIB_JNI ${PATH_JNI}/*.cpp)

# add lib
add_library(
        core
        SHARED
        ${FILES_LIB_COMMON}
        ${FILES_LIB_COMMON_EGL_CORE}
        ${FILES_LIB_COMMON_OPENGL_MEDIA}
        ${FILES_LIB_COMMON_OPENGL_MEDIA_RENDER}
        ${FILES_LIB_COMMON_OPENGL_MEDIA_TEXTURE}
        ${FILES_LIB_COMMON_OPENGL_MEDIA_TEXTURE_COPIER}
        ${FILES_LIB_COMMON_LOOPER}
        ${FILES_LIB_CAMERA_PREVIEW}
        ${FILES_LIB_VIDEO_ENCODER}
        ${FILES_LIB_SOFT_ENCODER}

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
        #ffmpeg相关库
        ${PATH_TO_PRE_BUILT}/libavfilter.a
        ${PATH_TO_PRE_BUILT}/libavformat.a
        ${PATH_TO_PRE_BUILT}/libavcodec.a
        ${PATH_TO_PRE_BUILT}/libpostproc.a
        ${PATH_TO_PRE_BUILT}/libswresample.a
        ${PATH_TO_PRE_BUILT}/libswscale.a
        ${PATH_TO_PRE_BUILT}/libavutil.a
        ${PATH_TO_PRE_BUILT}/libfdk-aac.a
        ${PATH_TO_PRE_BUILT}/libx264.a
)

