//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_GPUTEXTUREFRAME_H
#define AUVIDEO_GPUTEXTUREFRAME_H

#include "TextureFrame.h"

class GPUTextureFrame : public TextureFrame {
public:

    GPUTextureFrame() = default;

    ~GPUTextureFrame() override = default;;

    void initTexture() override;

    void updateTexImage() override;

    void destroy() override {
        if (texId) {
            glDeleteTextures(1, &texId);
        }
    }
};


#endif //AUVIDEO_GPUTEXTUREFRAME_H
