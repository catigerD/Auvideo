//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewRender.h"

#define LOG_TAG "RecordingPreviewRender"

RecordingPreviewRender::RecordingPreviewRender(int viewWidth, int viewHeight, int texWidth,
                                               int texHeight, int degress)
        : cameraTextureFrame(make_shared<GPUTextureFrame>()),
          formatTextureFrame(make_shared<FBOTextureFrame>(texWidth, texHeight, degress)),
          copier(make_shared<GPUTextureFrameCopier>(degress, texWidth, texHeight)),
          render(make_shared<GLSurfaceRender>(viewWidth, viewHeight)),
          viewWidth(viewWidth),
          viewHeight(viewHeight),
          texWidth(texWidth),
          texHeight(texHeight),
          degress(degress) {
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
            texCoords = const_cast<GLfloat *>(CAMERA_TEXTURE_ROTATED_90);
            break;
        case 180:
            texCoords = const_cast<GLfloat *>(CAMERA_TEXTURE_ROTATED_180);
            break;
        case 270:
            texCoords = const_cast<GLfloat *>(CAMERA_TEXTURE_ROTATED_270);
            break;
        default:
            texCoords = const_cast<GLfloat *>(CAMERA_TEXTURE_ROTATED_0);
            break;
    }
}

float RecordingPreviewRender::flip(float i) {
    return 1.0f - i;
}
