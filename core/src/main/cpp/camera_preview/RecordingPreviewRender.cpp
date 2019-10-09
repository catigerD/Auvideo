//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewRender.h"

#define LOG_TAG "RecordingPreviewRender"

RecordingPreviewRender::RecordingPreviewRender(int screenWidth, int screenHeight, int texWidth,
                                               int texHeight)
        : cameraTextureFrame(make_shared<GPUTextureFrame>()),
          formatTextureFrame(make_shared<FBOTextureFrame>(texWidth, texHeight)),
          copier(make_shared<GPUTextureFrameCopier>()),
          render(make_shared<GLSurfaceRender>(screenWidth, screenHeight)),
          screenWidth(screenWidth),
          screenHeight(screenHeight),
          texWidth(texWidth),
          texHeight(texHeight) {

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
    glViewport(0, 0, screenWidth, screenHeight);
    LOGI("processFrame , screenWidth : %d, screenHeight : %d, texWidth : %d, texHeight : %d",
         screenWidth, screenHeight, texWidth, texHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    copier->renderWithCoords(cameraTextureFrame, formatTextureFrame->getTexId(),
                             CAMERA_TRIANGLE_VERTICES,
                             CAMERA_TEXTURE_ROTATED_0);

}

void RecordingPreviewRender::drawToView() {
    render->renderToView(formatTextureFrame->getTexId());
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
