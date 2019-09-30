//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_TEXTUREFRAMECOPIER_H
#define AUVIDEO_TEXTUREFRAMECOPIER_H

#include <memory>
#include "TextureFrame.h"
#include "CommonTools.h"
#include "matrix.h"

using namespace std;

static const char *NO_FILTER_VERTEX_SOURCE = R"(
    attribute vec4 verCoords;
    attribute vec4 texCoords;
    varying vec4 v_texCoords;
    uniform highp mat4 verMatrix;
    uniform highp mat4 texMatrix;
    void main(){
        gl_Postion = verMatrix * verCoords;
        v_texCoords = texMatrix * texCoords;
    }
)";

static const char *NO_FILTER_FRAGMENT_SOURCE = R"(
    precision mediump float;
    varying vec4 v_texCoords;
    uniform sampler2D texSampler;
    void main(){
        gl_FragColor = texture2D(texSampler, v_texCoords);
    }
)";

class TextureFrameCopier {
public:
    TextureFrameCopier() = default;

    TextureFrameCopier(const char *verSource, const char *fragSource)
            : vertexSource(verSource), fragmentSource(fragSource) {}


    virtual ~TextureFrameCopier() = default;

    virtual bool init() = 0;

    virtual void renderWithCoords(shared_ptr<TextureFrame> textureFrame,
                                  GLuint texId, const GLfloat *vertexCoords,
                                  const GLfloat *texCoords) = 0;

    virtual void destroy() = 0;

protected:
    const char *vertexSource{NO_FILTER_VERTEX_SOURCE};
    const char *fragmentSource{NO_FILTER_FRAGMENT_SOURCE};
    GLuint programId{};
    GLuint verCoordsAttrLoc{};
    GLuint texCoordsAttrLoc{};
    GLuint verMatrixUniformLoc{};
    GLuint texMatrixUniformLoc{};
    bool isInit{};
};


#endif //AUVIDEO_TEXTUREFRAMECOPIER_H
