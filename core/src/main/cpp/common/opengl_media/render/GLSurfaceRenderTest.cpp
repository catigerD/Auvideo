//
// Created by dengchong on 2019-09-25.
//
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLSurfaceRenderTest.h"

#define LOG_TAG "GLSurfaceRenderTest"

GLSurfaceRenderTest::GLSurfaceRenderTest(const shared_ptr<ANativeWindow> &window, int width,
                                         int height, const char *imagePath)
        : EglTest(window, width, height),
          imagePath(imagePath),
          render(make_shared<GLSurfaceRender>(width, height)) {
}

GLSurfaceRenderTest::~GLSurfaceRenderTest() {
    stbi_image_free(data);
    LOGI("GLSurfaceRenderTest::~GLSurfaceRenderTest()");
}

void GLSurfaceRenderTest::loadImage() {
    render->initProgram();
    data = stbi_load(imagePath, &imageWidth, &imageHeight, &imageChannel, 0);
    glGenTextures(1, &imageTex);
    glBindTexture(GL_TEXTURE_2D, imageTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    glBindTexture(GL_TEXTURE_2D, 0);
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
    render->renderToAutoFillTexture(imageTex, imageWidth, imageHeight, outputTexId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    render->renderToViewWithAutoFit(outputTexId, width, height, width, height);
}
