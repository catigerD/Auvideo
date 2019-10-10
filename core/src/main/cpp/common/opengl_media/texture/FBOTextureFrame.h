//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_FBOTEXTUREFRAME_H
#define AUVIDEO_FBOTEXTUREFRAME_H

#include <TextureFrame.h>

class FBOTextureFrame : public TextureFrame {

public:

    FBOTextureFrame(int width, int height, int degress) : width(width), height(height),
                                                          degress(degress) {

    }

    ~FBOTextureFrame() {
        if (texId) {
            glDeleteTextures(1, &texId);
            texId = 0;
        }
    }

    void initTexture();

    void updateTexImage() {};

    void bindTexture(GLuint *samplerLoc);

    void destroy() {
        if (texId) {
            glDeleteTextures(1, &texId);
            texId = 0;
        }
    }

    GLuint getTexId() {
        return texId;
    }

private:
    GLuint texId{};
    int width{};
    int height{};
    int degress{};
};


#endif //AUVIDEO_FBOTEXTUREFRAME_H
