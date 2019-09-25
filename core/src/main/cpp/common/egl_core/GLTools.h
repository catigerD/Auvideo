//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLTOOLS_H
#define AUVIDEO_GLTOOLS_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

#define GL_LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "GL_TOOLS", __VA_ARGS__)
#define GL_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "GL_TOOLS", __VA_ARGS__)

static GLuint loadShader(GLenum shaderType, const char *shaderSource);

static GLuint loadProgram(const char *vertexSource, const char *fragmentSource);

#endif //AUVIDEO_GLTOOLS_H
