//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_TEXTUREFRAMECOPIER_H
#define AUVIDEO_TEXTUREFRAMECOPIER_H

#include <memory>
#include "TextureFrame.h"
#include "AndroidLog.h"
#include "matrix.h"

using namespace std;

static const char *NO_FILTER_VERTEX_SOURCE = R"(
#version 300 es

layout(location=0) in vec4 verCoords;
layout(location=1) in vec4 texCoords;
out vec4 v_texCoords;

void main(){
    gl_Position = verCoords;
    v_texCoords = texCoords;
}
)";

static const char *NO_FILTER_FRAGMENT_SOURCE = R"(
#version 300 es
precision mediump float;

in vec4 v_texCoords;
layout(location=0) out vec4 gl_FragColor;
uniform sampler2D texSampler;

void main(){
    gl_FragColor = texture(texSampler, v_texCoords.xy);
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
    bool isInit{};
};


#endif //AUVIDEO_TEXTUREFRAMECOPIER_H
