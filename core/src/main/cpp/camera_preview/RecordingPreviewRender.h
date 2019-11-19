//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWRENDER_H
#define AUVIDEO_RECORDINGPREVIEWRENDER_H

#include <memory>

#include "GPUTextureFrame.h"
#include "GPUTextureFrameCopier.h"
#include "GLSurfaceRender.h"
#include "FBOTextureFrame.h"
#include "ImageTextureFrame.h"

static const GLfloat CAMERA_TRIANGLE_VERTICES[8] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

//1. 先顺时针旋转 x 度。
//2. 由于计算机图形坐标与 OpenGL坐标 y 轴相反，做 VFlip 操作
static const GLfloat CAMERA_TEXTURE_ROTATED_0[8] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

static const GLfloat CAMERA_TEXTURE_ROTATED_90[8] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
};

static const GLfloat CAMERA_TEXTURE_ROTATED_180[8] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
};

static const GLfloat CAMERA_TEXTURE_ROTATED_270[8] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
};

class RecordingPreviewRender {
public:

    RecordingPreviewRender(int viewWidth, int viewHeight, int texWidth, int texHeight,
                           int degress, bool isVFlip);

    ~RecordingPreviewRender();

    void init();

    void setDegress(int degress, bool isVFlip);

    void processFrame();

    void drawToView();

    void destroy();

    int getCameraTexId() {
        return cameraTextureFrame->getTexId();
    }

    int getRenderTexId() {
        return formatTextureFrame->getTexId();
    }

private:
    GLuint FBO{};
    //传给相机的纹理 Id 封装
    shared_ptr<GPUTextureFrame> cameraTextureFrame;
    shared_ptr<FBOTextureFrame> formatTextureFrame;
    shared_ptr<GPUTextureFrameCopier> copier;
    shared_ptr<GLSurfaceRender> render;

private:
    int viewWidth{};
    int viewHeight{};
    int texWidth{};
    int texHeight{};
    int degress{};
    bool isVFlip{};

    GLfloat texCoords[8];

    void fillTexCoords();

    float flip(float i);
};


#endif //AUVIDEO_RECORDINGPREVIEWRENDER_H
