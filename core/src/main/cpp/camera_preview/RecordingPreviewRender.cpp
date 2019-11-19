//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewRender.h"

#define LOG_TAG "RecordingPreviewRender"

using namespace std;

RecordingPreviewRender::RecordingPreviewRender(int viewWidth, int viewHeight, int texWidth,
                                               int texHeight, int degress, bool isVFlip)
        : cameraTextureFrame(make_shared<GPUTextureFrame>()),
          formatTextureFrame(make_shared<FBOTextureFrame>(texWidth, texHeight, degress)),
          copier(make_shared<GPUTextureFrameCopier>(degress, texWidth, texHeight)),
          render(make_shared<GLSurfaceRender>(viewWidth, viewHeight)),
          viewWidth(viewWidth),
          viewHeight(viewHeight),
          texWidth(texWidth),
          texHeight(texHeight),
          degress(degress),
          isVFlip(isVFlip) {
    fillTexCoords();
}

RecordingPreviewRender::~RecordingPreviewRender() = default;

void RecordingPreviewRender::init() {
    cameraTextureFrame->initTexture();
    formatTextureFrame->initTexture();
    copier->init();
    render->init();
    glGenFramebuffers(1, &FBO);
}

void RecordingPreviewRender::setDegress(int degress, bool isVFlip) {
    this->degress = degress;
    this->isVFlip = isVFlip;
    fillTexCoords();
}

void RecordingPreviewRender::processFrame() {
    //这里会影响后面的 renderToView
    //glViewport(0, 0, texHeight, texWidth);
    LOGI("processFrame , screenWidth : %d, screenHeight : %d, texWidth : %d, texHeight : %d, degress : %d",
         viewWidth, viewHeight, texWidth, texHeight, degress);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    copier->renderWithCoords(cameraTextureFrame, formatTextureFrame->getTexId(),
                             CAMERA_TRIANGLE_VERTICES,
                             texCoords);

}

void RecordingPreviewRender::drawToView() {
    render->renderToViewWithAutoFill(formatTextureFrame->getTexId(), viewWidth, viewHeight,
                                     formatTextureFrame->getWidth(), formatTextureFrame->getHeight());
//    render->renderToView(formatTextureFrame->getTexId());
}

void RecordingPreviewRender::destroy() {
    if (FBO) {
        glDeleteFramebuffers(1, &FBO);
    }
    render->destroy();
    copier->destroy();
    formatTextureFrame->destroy();
    cameraTextureFrame->destroy();
}

void RecordingPreviewRender::fillTexCoords() {
    switch (degress) {
        case 90:
            memcpy(texCoords, CAMERA_TEXTURE_ROTATED_90, 8 * sizeof(GLfloat));
            break;
        case 180:
            memcpy(texCoords, CAMERA_TEXTURE_ROTATED_180, 8 * sizeof(GLfloat));
            break;
        case 270:
            memcpy(texCoords, CAMERA_TEXTURE_ROTATED_270, 8 * sizeof(GLfloat));
            break;
        default:
            memcpy(texCoords, CAMERA_TEXTURE_ROTATED_0, 8 * sizeof(GLfloat));
            break;
    }
    if (isVFlip) {
        texCoords[1] = flip(texCoords[1]);
        texCoords[3] = flip(texCoords[3]);
        texCoords[5] = flip(texCoords[5]);
        texCoords[7] = flip(texCoords[7]);
    }
}

float RecordingPreviewRender::flip(float i) {
    return 1.0f - i;
}
