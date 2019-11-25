//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_FBOTEXTUREFRAME_H
#define AUVIDEO_FBOTEXTUREFRAME_H

#include <vector>

#include "TextureFrame.h"
#include "AndroidLog.h"
#include "GLTools.h"

class FBOTextureFrame : public TextureFrame {

public:

    FBOTextureFrame(int width, int height, int degress = 0)
            : width(degress == 90 || degress == 270 ? height : width),
              height(degress == 90 || degress == 270 ? width : height),
              degress(degress) {

    }

    ~FBOTextureFrame() override = default;

    void initTexture() override;

    void updateTexImage() override {};

    void bindTexture(GLuint *samplerLoc) override;

    void destroy() override {
        if (texId) {
            glDeleteTextures(1, &texId);
        }
        if (fbo) {
            glDeleteFramebuffers(1, &fbo);
        }
    }

    GLuint getTexId() {
        return texId;
    }

    GLuint getFbo() {
        return fbo;
    }

    int getWidth() const {
        return width;
    };

    int getHeight() const {
        return height;
    }

private:
    GLuint texId{};
    GLuint fbo;
    int width{};
    int height{};
    int degress{};
};


#endif //AUVIDEO_FBOTEXTUREFRAME_H
