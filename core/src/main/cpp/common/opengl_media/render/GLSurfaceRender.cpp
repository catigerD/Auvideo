//
// Created by dengchong on 2019-09-25.
//

#include "GLSurfaceRender.h"

#define LOG_TAG "GLSurfaceRender"

GLSurfaceRender::GLSurfaceRender(int width, int height)
        : width(width),
          height(height) {
}

void GLSurfaceRender::initProgram() {
    programId = loadProgram(vertexSource, fragmentSource);
    if (!programId) {
        LOGE("Could not create Program");
        return;
    }
    vertexcoordAttrLoc = static_cast<GLuint>(glGetAttribLocation(programId, "vertexcoord"));
    checkGLError("glGetAttribLocation vertexcoord");
    texcoordAttrLoc = static_cast<GLuint>(glGetAttribLocation(programId, "texcoord"));
    checkGLError("glGetAttribLocation texcoord");
    textureUniformLoc = static_cast<GLuint>(glGetAttribLocation(programId, "texture"));
    checkGLError("glGetAttribLocation texture");
    isInitProgram = true;
}

GLSurfaceRender::~GLSurfaceRender() {
    if (programId) {
        glDeleteProgram(programId);
    }
}

void GLSurfaceRender::resetRenderSize(int left, int top, int width, int height) {
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;
}

void GLSurfaceRender::renderToTexture(GLuint inputTexId, GLuint outputTexId) {
    glViewport(left, top, width, height);
    if (!isInitProgram) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGLError("renderToTexture , glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
    }

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_TEXTURE_COORD);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToView(GLuint texId) {
    glViewport(left, top, width, height);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_TEXTURE_COORD);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
}
