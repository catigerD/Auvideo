//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWRENDER_H
#define AUVIDEO_RECORDINGPREVIEWRENDER_H

#include <memory>
#include <array>

#include "GPUTextureFrame.h"
#include "TextureFrameCopier.h"
#include "GLSurfaceRender.h"
#include "FBOTextureFrame.h"
#include "ImageTextureFrame.h"

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
    std::shared_ptr<GPUTextureFrame> cameraTextureFrame;
    std::shared_ptr<FBOTextureFrame> formatTextureFrame;
    std::shared_ptr<TextureFrameCopier> gpuCopier;
    std::shared_ptr<GLSurfaceRender> render;

private:
    int viewWidth{};
    int viewHeight{};
    int texWidth{};
    int texHeight{};
    int degress{};
    bool isVFlip{};

    std::array<GLfloat, RenderConfig::VERTEX_SIZE> texCoords{};

    void fillTexCoords();

    float flip(float i);
};


#endif //AUVIDEO_RECORDINGPREVIEWRENDER_H
