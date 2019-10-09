//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWRENDER_H
#define AUVIDEO_RECORDINGPREVIEWRENDER_H

#include <memory>
#include <GPUTextureFrame.h>
#include <GPUTextureFrameCopier.h>
#include "GLSurfaceRender.h"
#include "FBOTextureFrame.h"

using namespace std;

static const GLfloat CAMERA_TRIANGLE_VERTICES[8] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

static const GLfloat CAMERA_TEXTURE_ROTATED_0[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
};

class RecordingPreviewRender {
public:

    RecordingPreviewRender(int screenWidth, int screenHeight, int texWidth, int texHeight);

    ~RecordingPreviewRender();

    void init();

    void processFrame();

    void drawToView();

    void destroy();

    int getCameraTexId() {
        return cameraTextureFrame->getTexId();
    }

private:
    GLuint FBO{};
    //传给相机的纹理 Id 封装
    shared_ptr<GPUTextureFrame> cameraTextureFrame;
    shared_ptr<FBOTextureFrame> formatTextureFrame;
    shared_ptr<GPUTextureFrameCopier> copier;
    shared_ptr<GLSurfaceRender> render;

private:
    int screenWidth{};
    int screenHeight{};
    int texWidth{};
    int texHeight{};
};


#endif //AUVIDEO_RECORDINGPREVIEWRENDER_H
