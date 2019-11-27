//
// Created by dengchong on 2019-09-30.
//

#include "GPUTextureFrameCopier.h"

#define LOG_TAG "GPUTextureFrameCopier"

const std::string GPUTextureFrameCopier::GPU_FRAGMENT_SOURCE = R"(
#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;
in vec4 v_texcoord;
layout(location=0) out vec4 gl_FragColor;
uniform samplerExternalOES gpuSampler;
void main(){
    gl_FragColor = texture(gpuSampler, v_texcoord.xy);
}
)";

GPUTextureFrameCopier::GPUTextureFrameCopier(int viewWidth, int viewHeight, int degress)
        : viewWidth(degress == 90 || degress == 270 ? viewHeight : viewWidth),
          viewHeight(degress == 90 || degress == 270 ? viewWidth : viewHeight),
          TextureFrameCopier(RenderConfig::VERTEX_SOURCE, GPU_FRAGMENT_SOURCE) {

}

void GPUTextureFrameCopier::init() {
    programId = loadProgram(vertexSource.c_str(), fragmentSource.c_str());
    if (!programId) {
        LOGE("GPUTextureFrameCopier::init() failed ...");
        return;
    }
    gpuSampler = static_cast<GLuint>(glGetUniformLocation(programId, "gpuSampler"));
    glUniform1i(gpuSampler, 0);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::VERTEX_COORD.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(RenderConfig::VERTEX_POS_INDEX);
    glVertexAttribPointer(RenderConfig::VERTEX_POS_INDEX, RenderConfig::VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::TEXTURE_COORD.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(RenderConfig::VERTEX_TEX_INDEX);
    glVertexAttribPointer(RenderConfig::VERTEX_TEX_INDEX, RenderConfig::VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GPUTextureFrameCopier::renderWithCoords(std::shared_ptr<TextureFrame> textureFrame, GLuint outputTexId,
                                             const std::array<GLfloat, RenderConfig::VERTEX_SIZE> &verCoord,
                                             const std::array<GLfloat, RenderConfig::VERTEX_SIZE> &texCoord) {
    if (!programId) {
        LOGE("GPUTextureFrameCopier::init() not success");
        return;
    }

    glViewport(0, 0, viewWidth, viewHeight);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        LOGE("glCheckFramebufferStatus() not complete， status : %d ， textureid : %d", status,
             outputTexId);
        return;
    }
    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, verCoord.data());
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, texCoord.data());
    glBindTexture(GL_TEXTURE_2D, textureFrame->getTexId());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
