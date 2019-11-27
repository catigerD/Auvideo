//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLSURFACERENDER_H
#define AUVIDEO_GLSURFACERENDER_H

#include <array>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "GLTools.h"
#include "AndroidLog.h"
#include "RenderConfig.h"

/*
 * 1.使用自定义 FrameBuffer 绘制输入纹理到目标纹理。
 * 2.使用默认 FrameBuffer 绘制输入纹理到 Surface上。
 */
class GLSurfaceRender {
public:
    GLSurfaceRender(int viewWidth, int viewHeight)
            : viewWidth(viewHeight),
              viewHeight(viewHeight) {}

    ~GLSurfaceRender() = default;

    //需要在 OpenGL线程中初始化
    void init();

    void resetRenderSize(int left, int top, int width, int height);

    void renderToView(GLuint texId);

    void renderToView(GLuint texId, int renderWidth, int renderHeight);

    void renderToViewWithAutoFit(GLuint texId, int renderWidth, int renderHeight, int texWidth,
                                 int texHeight);

    void renderToViewWithAutoFill(GLuint texId, int renderWidth, int renderHeight, int texWidth,
                                  int texHeight);

    void renderToTexture(GLuint inputTexId, GLuint outputTexId);

    void renderToVFlipTexture(GLuint inputTexId, GLuint outputTexId);

    void renderToAutoFitTexture(GLuint inputTexId, int texWidth, int texHeight,
                                GLuint outputTexId);

    void renderToAutoFillTexture(GLuint inputTexId, int texWidth, int texHeight,
                                 GLuint outputTexId);

    void renderWaterToTexture(GLuint inputTexId, int left, int top, int waterWidth, int waterHeight,
                              GLuint outputTexId);

    void destroy() {
        glDeleteSamplers(1, &SMO);
        glDeleteBuffers(2, VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(programId);
    }

private:
    GLint left{};
    GLint top{};
    GLint viewWidth;
    GLint viewHeight;

    GLuint programId{};
private:

    GLuint VBO[2]{};
    GLuint VAO{};
    GLuint SMO{};
};


#endif //AUVIDEO_GLSURFACERENDER_H
