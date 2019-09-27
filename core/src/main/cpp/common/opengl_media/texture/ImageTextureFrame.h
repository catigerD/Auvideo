//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_IMAGETEXTUREFRAME_H
#define AUVIDEO_IMAGETEXTUREFRAME_H

#include "TextureFrame.h"
#include "stb_image.h"

class ImageTextureFrame : public TextureFrame {
public:

    ImageTextureFrame(const char *imagePath, int &width, int &height, int &channel);

    ~ImageTextureFrame();

    void initTexture();

    void updateTexImage();

    void bindTexture(GLuint *samplerLoc);

    GLuint getTexId() {
        return texId;
    }

private:
    GLuint texId{};
    const char *imagePath{};
    int &width;
    int &height;
    int &channel;
    unsigned char *data{};
};


#endif //AUVIDEO_IMAGETEXTUREFRAME_H
