//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_GPUTEXTUREFRAMECOPIER_H
#define AUVIDEO_GPUTEXTUREFRAMECOPIER_H

#include "TextureFrameCopier.h"

static const char *GPU_FRAGMENT_SOURCE = R"(
#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;
in vec4 v_texCoords;
layout(location=0) out vec4 gl_FragColor;
uniform samplerExternalOES gpuSampler;
void main(){
    gl_FragColor = texture(gpuSampler, v_texCoords.xy);
}
)";

class GPUTextureFrameCopier : public TextureFrameCopier {
public:
    GPUTextureFrameCopier(int degress, int viewWidth, int viewHeight);

    ~GPUTextureFrameCopier() override;

    bool init() override;

    void renderWithCoords(shared_ptr<TextureFrame> textureFrame,
                          GLuint texId, const GLfloat *vertexCoords, const GLfloat *texCoords) override;

    void destroy() override;

private:
    int viewWidth;
    int viewHeight;

    GLuint VBO;
    GLuint VAO;

    GLuint gpuSamplerUniformLoc{};
};


#endif //AUVIDEO_GPUTEXTUREFRAMECOPIER_H
