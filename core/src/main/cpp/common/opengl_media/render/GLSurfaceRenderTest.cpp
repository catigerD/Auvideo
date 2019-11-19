//
// Created by dengchong on 2019-09-25.
//
#define STB_IMAGE_IMPLEMENTATION

#include "GLSurfaceRenderTest.h"

#define LOG_TAG "GLSurfaceRenderTest"

using namespace std;

GLSurfaceRenderTest::GLSurfaceRenderTest(const shared_ptr<ANativeWindow> &window, int width,
                                         int height, string imagePath)
        : EglTest(window, width, height),
          imagePath(std::move(imagePath)),
          render(make_shared<GLSurfaceRender>(width, height)),
          inputTextureFrame(make_shared<ImageTextureFrame>(imagePath)) {
}

GLSurfaceRenderTest::~GLSurfaceRenderTest() {
//    LOGI("GLSurfaceRenderTest::~GLSurfaceRenderTest()");
}

void GLSurfaceRenderTest::loadImage() {
    render->init();
    inputTextureFrame->initTexture();
    inputTextureFrame->updateTexImage();
    isLoadedImage = true;

    glGenTextures(1, &outputTexId);
    glBindTexture(GL_TEXTURE_2D, outputTexId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &FBO);
}

void GLSurfaceRenderTest::drawRect() {
    if (looperTest) {
        looperTest->handler->sendMessage(LooperTest::MSG_TEST);
    }
    if (!isLoadedImage) {
        loadImage();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    render->renderToAutoFillTexture(inputTextureFrame->getTexId(), inputTextureFrame->width, inputTextureFrame->height,
                                    outputTexId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    render->renderToViewWithAutoFit(outputTexId, width, height, width, height);
}
