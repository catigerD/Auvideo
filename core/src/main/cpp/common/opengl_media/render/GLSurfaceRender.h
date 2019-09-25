//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_GLSURFACERENDER_H
#define AUVIDEO_GLSURFACERENDER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLTools.h"
#include "CommonTools.h"

static const char *OUTPUT_VIEW_VERTEX_SOURCE = R"(
    attribute vec2 vertexcoord;
    attribute vec2 texcoord;
    varying vec2 v_texcoord;
    void main(){
        gl_Position = vec4(vertexcoord, 0, 1);
        v_texcoord = texcoord;
    }
)";

static const char *OUTPUT_VIEW_FRAGMENT_SOURCE = R"(
    varying highp vec2 v_texcoord;
    uniform sampler2D texture;
    void main(){
        gl_FragColor = texture2D(texture, v_texcoord);
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

/*
 * 1.使用自定义 FrameBuffer 绘制输入纹理到目标纹理。
 * 2.使用默认 FrameBuffer 绘制输入纹理到 Surface上。
 */
class GLSurfaceRender {
public:
    GLSurfaceRender(int width, int height);

    ~GLSurfaceRender();

    //需要在 OpenGL线程中初始化
    void initProgram();

    void resetRenderSize(int left, int top, int width, int height);

    void renderToTexture(GLuint inputTexId, GLuint outputTexId);

    void renderToView(GLuint texId);

private:
    GLint left{};
    GLint top{};
    GLint width{};
    GLint height{};

    const char *vertexSource{OUTPUT_VIEW_VERTEX_SOURCE};
    const char *fragmentSource{OUTPUT_VIEW_FRAGMENT_SOURCE};

    bool isInitProgram{};
    GLuint programId{};
    GLuint vertexcoordAttrLoc{};
    GLuint texcoordAttrLoc{};
    GLuint textureUniformLoc{};
};


#endif //AUVIDEO_GLSURFACERENDER_H
