//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_IMAGETEXTUREFRAME_H
#define AUVIDEO_IMAGETEXTUREFRAME_H

#include <string>

#include "TextureFrame.h"
#include "stb_image.h"

class ImageTextureFrame : public TextureFrame {
public:

    explicit ImageTextureFrame(std::string imagePath);

    ~ImageTextureFrame() override;

    void initTexture() override;

    void updateTexImage() override;

    void destroy() override {
        glDeleteTextures(1, &texId);
    }

public:

    int width{};
    int height{};
    int channel{};

private:
    bool validPath{};
    std::string imagePath;
    unsigned char *data{};
};


#endif //AUVIDEO_IMAGETEXTUREFRAME_H
