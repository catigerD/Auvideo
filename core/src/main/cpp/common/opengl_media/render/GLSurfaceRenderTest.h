//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLSURFACERENDERTEST_H
#define AUVIDEO_GLSURFACERENDERTEST_H

#include "EglTest.h"
#include "GLSurfaceRender.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "ImageTextureFrame.h"

class GLSurfaceRenderTest : public EglTest {
public:
    GLSurfaceRenderTest(const shared_ptr<ANativeWindow> &window, int width, int height,
                        const char *imagePath);

    ~GLSurfaceRenderTest();

private:
    void loadImage();

    void drawRect();

private:
    const char *imagePath;
    bool isLoadedImage{};
    unsigned char *data{};
    int imageWidth{};
    int imageHeight{};
    int imageChannel{};

    GLuint FBO;
    GLuint outputTexId;
    shared_ptr<ImageTextureFrame> inputTextureFrame;


    shared_ptr<GLSurfaceRender> render;
};

#endif //AUVIDEO_GLSURFACERENDERTEST_H
