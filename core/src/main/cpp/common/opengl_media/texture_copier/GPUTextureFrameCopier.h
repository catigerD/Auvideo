//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_GPUTEXTUREFRAMECOPIER_H
#define AUVIDEO_GPUTEXTUREFRAMECOPIER_H

#include "TextureFrameCopier.h"

static const char *GPU_FRAGMENT_SOURCE = R"(
    #extension GL_OES_EGL_image_external : require
    precision mediump float;
    varying vec4 v_texCoords;
    uniform samplerExternalOES gpuSampler;
    void main(){
        gl_FragColor = texture2D(gpuSampler, v_texCoords.xy);
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

    GLuint gpuSamplerUniformLoc{};
};


#endif //AUVIDEO_GPUTEXTUREFRAMECOPIER_H
