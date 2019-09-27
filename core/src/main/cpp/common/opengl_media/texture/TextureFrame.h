//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_TEXTUREFRAME_H
#define AUVIDEO_TEXTUREFRAME_H

#include "GLTools.h"

class TextureFrame {
public:
    TextureFrame() = default;

    virtual ~TextureFrame() {};

    virtual void initTexture() = 0;

    virtual void updateTexImage() = 0;

    virtual void bindTexture(GLuint *samplerLoc) = 0;

protected:
};


#endif //AUVIDEO_TEXTUREFRAME_H
