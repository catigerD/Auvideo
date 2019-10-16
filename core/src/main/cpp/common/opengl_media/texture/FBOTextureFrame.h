//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_FBOTEXTUREFRAME_H
#define AUVIDEO_FBOTEXTUREFRAME_H

#include <TextureFrame.h>
#include <CommonTools.h>
#include <GLTools.h>
#include <vector>

using namespace std;

class FBOTextureFrame : public TextureFrame {

public:

    FBOTextureFrame(int width, int height, int degress = 0) : width(width), height(height),
                                                              degress(degress) {

    }

    ~FBOTextureFrame() {
        if (texId) {
            glDeleteTextures(1, &texId);
        }
    }

    void initTexture();

    void updateTexImage() {};

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
    GLuint fbo;
    int width{};
    int height{};
    int degress{};
};


#endif //AUVIDEO_FBOTEXTUREFRAME_H
