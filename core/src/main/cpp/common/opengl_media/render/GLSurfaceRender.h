//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLSURFACERENDER_H
#define AUVIDEO_GLSURFACERENDER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GLTools.h"
#include "AndroidLog.h"

static const char *OUTPUT_VIEW_VERTEX_SOURCE = R"(
#version 300 es

layout(location = 0) in vec4 vertexcoord;
layout(location = 1) in vec4 texcoord;
out vec4 v_texcoord;
void main(){
    gl_Position = vertexcoord;
    v_texcoord = texcoord;
}
)";

static const char *OUTPUT_VIEW_FRAGMENT_SOURCE = R"(
#version 300 es

precision mediump float;

in highp vec4 v_texcoord;
layout(location = 0) out vec4 gl_FragColor;
uniform sampler2D texture;
void main(){
    gl_FragColor = texture(texture, v_texcoord.xy);
}
)";

static const GLfloat OUTPUT_VIEW_VERTEX_COORD[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

static const GLfloat OUTPUT_VIEW_TEXTURE_COORD[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
};

static const GLfloat OUTPUT_VIEW_VFLIP_TEXTURE_COORD[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

/*
 * 1.使用自定义 FrameBuffer 绘制输入纹理到目标纹理。
 * 2.使用默认 FrameBuffer 绘制输入纹理到 Surface上。
 */
class GLSurfaceRender {
public:
    GLSurfaceRender(int viewWidth, int viewHeight, const char *vertexSource = OUTPUT_VIEW_VERTEX_SOURCE,
                    const char *fragmentSource = OUTPUT_VIEW_FRAGMENT_SOURCE);

    ~GLSurfaceRender();

    //需要在 OpenGL线程中初始化
    void init();

    void resetRenderSize(int left, int top, int width, int height);

    void renderToView(GLuint texId);

    void renderToView(GLuint texId, int viewWidth, int viewHeight);

    void renderToViewWithAutoFit(GLuint texId, int viewWidth, int viewHeight, int texWidth,
                                 int texHeight);

    void renderToViewWithAutoFill(GLuint texId, int viewWidth, int viewHeight, int texWidth,
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
        if (programId) {
            glDeleteProgram(programId);
        }
    }

private:
    const static int VERTEX_POSITION_LOCATION = 0;
    const static int VERTEX_TEXTURE_LOCATION = 1;

private:
    GLint left{};
    GLint top{};
    GLint viewWidth{};
    GLint viewHeight{};

    const char *vertexSource;
    const char *fragmentSource;

    bool isInitProgram{};
    GLuint programId{};
    GLuint textureUniformLoc{};
};


#endif //AUVIDEO_GLSURFACERENDER_H
