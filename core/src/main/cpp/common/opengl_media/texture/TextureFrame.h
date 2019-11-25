//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_TEXTUREFRAME_H
#define AUVIDEO_TEXTUREFRAME_H

#include "GLTools.h"

class TextureFrame {
public:
    TextureFrame() = default;

    virtual ~TextureFrame() = default;;

    virtual void initTexture() = 0;

    virtual void updateTexImage() = 0;

    virtual void bindTexture(GLuint *samplerLoc) = 0;

    virtual void destroy() = 0;

protected:
};


#endif //AUVIDEO_TEXTUREFRAME_H
