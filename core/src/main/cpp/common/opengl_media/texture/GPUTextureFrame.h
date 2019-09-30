//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_GPUTEXTUREFRAME_H
#define AUVIDEO_GPUTEXTUREFRAME_H

#include "TextureFrame.h"

class GPUTextureFrame : public TextureFrame {
public:

    GPUTextureFrame() = default;

    ~GPUTextureFrame() {

    };

    void initTexture();

    void updateTexImage();

    void bindTexture(GLuint *samplerLoc);

    void destroy() {
        if (texId) {
            glDeleteTextures(1, &texId);
        }
    }

    GLuint getTexId() {
        return texId;
    }

private:
    GLuint texId{};
};


#endif //AUVIDEO_GPUTEXTUREFRAME_H
