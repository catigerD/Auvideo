//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewRender.h"

RecordingPreviewRender::RecordingPreviewRender(int screenWidth, int screenHeight)
        : cameraTextureFrame(make_shared<GPUTextureFrame>()),
          formatTextureFrame(make_shared<DefaultTextureFrame>()),
          copier(make_shared<GPUTextureFrameCopier>()),
          render(make_shared<GLSurfaceRender>(screenWidth, screenHeight)),
          screenWidth(screenWidth),
          screenHeight(screenHeight) {

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
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, screenWidth, screenHeight);
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
