//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLSURFACERENDERTEST_H
#define AUVIDEO_GLSURFACERENDERTEST_H

#include <string>

#include <GLES3/gl3.h>

#include "EglTest.h"
#include "GLSurfaceRender.h"
#include "ImageTextureFrame.h"

class GLSurfaceRenderTest : public EglTest {
public:
    GLSurfaceRenderTest(const shared_ptr<ANativeWindow> &window, int width, int height, std::string imagePath);

    ~GLSurfaceRenderTest() override;

private:
    void loadImage();

    void drawRect() override;

private:
    std::string imagePath;
    bool isLoadedImage{};

    GLuint FBO;
    GLuint outputTexId;
    shared_ptr<ImageTextureFrame> inputTextureFrame;


    shared_ptr<GLSurfaceRender> render;
};

#endif //AUVIDEO_GLSURFACERENDERTEST_H
