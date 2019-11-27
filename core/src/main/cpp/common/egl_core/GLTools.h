//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLTOOLS_H
#define AUVIDEO_GLTOOLS_H

#include <GLES3/gl3.h>
#include <android/log.h>

#define GL_LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "GL_TOOLS", __VA_ARGS__)
#define GL_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "GL_TOOLS", __VA_ARGS__)

static GLuint loadShader(GLenum shaderType, const char *shaderSource) {
    GLuint shaderId = glCreateShader(shaderType);
    if (shaderId) {
        glShaderSource(shaderId, 1, &shaderSource, nullptr);
        glCompileShader(shaderId);
        GLint status;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        if (!status) {
            GLint logLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
            if (!logLength) {
                logLength = 512;
            }
            char infoLog[logLength];
            glGetShaderInfoLog(shaderId, logLength, nullptr, infoLog);
            GL_LOGE("Could not compile shader %d:\n%s\n", shaderType, infoLog);
            glDeleteShader(shaderId);
            shaderId = 0;
        }
    }
    return shaderId;
}

static GLuint loadProgram(const char *vertexSource, const char *fragmentSource) {
    GLuint programId = 0;
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return programId;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        return programId;
    }
    programId = glCreateProgram();
    if (programId) {
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);
        GLint status;
        glGetProgramiv(programId, GL_LINK_STATUS, &status);
        if (!status) {
            GLint logLength;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
            if (!logLength) {
                logLength = 512;
            }
            char infoLog[logLength];
            glGetProgramInfoLog(programId, logLength, nullptr, infoLog);
            GL_LOGE("Could not link program \n%s\n", infoLog);
            glDeleteProgram(programId);
            programId = 0;
        }
    }
    return programId;
}

static void checkGLError(const char *op) {
    GL_LOGE("after %s() glError (0x%x)\n", op, glGetError());
}

#endif //AUVIDEO_GLTOOLS_H
