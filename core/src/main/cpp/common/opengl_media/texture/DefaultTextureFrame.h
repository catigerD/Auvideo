//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_DEFAULTTEXTUREFRAME_H
#define AUVIDEO_DEFAULTTEXTUREFRAME_H

#include <TextureFrame.h>

class DefaultTextureFrame : public TextureFrame {

public:

    DefaultTextureFrame() = default;

    ~DefaultTextureFrame() {
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
    GLuint texId;
};


#endif //AUVIDEO_DEFAULTTEXTUREFRAME_H
