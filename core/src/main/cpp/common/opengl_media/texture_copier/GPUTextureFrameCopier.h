//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_GPUTEXTUREFRAMECOPIER_H
#define AUVIDEO_GPUTEXTUREFRAMECOPIER_H

#include "TextureFrameCopier.h"

class GPUTextureFrameCopier : public TextureFrameCopier {
public:
    static const std::string GPU_FRAGMENT_SOURCE;

public:
    GPUTextureFrameCopier(int viewWidth, int viewHeight, int degress);

    void init() override;

    void renderWithCoords(std::shared_ptr<TextureFrame> textureFrame, GLuint outputTexId,
                          const std::array<GLfloat, RenderConfig::VERTEX_SIZE> &verCoord,
                          const std::array<GLfloat, RenderConfig::VERTEX_SIZE> &texCoord) override;

    void destroy() override {
        glDeleteBuffers(2, VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(programId);
    }

private:
    int viewWidth;
    int viewHeight;

    GLuint VBO[2];
    GLuint VAO{};

    GLuint gpuSampler{};
};


#endif //AUVIDEO_GPUTEXTUREFRAMECOPIER_H
