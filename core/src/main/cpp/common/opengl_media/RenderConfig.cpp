//
// Created by dengchong on 2019-11-29.
//

#include "RenderConfig.h"

const std::string RenderConfig::VERTEX_SOURCE = R"(
#version 300 es

layout(location = 0) in vec4 vertexcoord;
layout(location = 1) in vec4 texcoord;
out vec4 v_texcoord;
void main(){
    gl_Position = vertexcoord;
    v_texcoord = texcoord;
}
)";

const std::string RenderConfig::FRAGMENT_SOURCE = R"(
#version 300 es

precision mediump float;

in highp vec4 v_texcoord;
layout(location = 0) out vec4 gl_FragColor;
uniform sampler2D texture;
void main(){
    gl_FragColor = texture(texture, v_texcoord.xy);
}
)";

const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::VERTEX_COORD = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::TEXTURE_COORD = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
};

const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::V_FLIP_TEXTURE_COORD = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

//1. 先顺时针旋转 x 度。
//2. 由于计算机图形坐标与 OpenGL坐标 y 轴相反，做 VFlip 操作
const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::CAMERA_TEXTURE_ROTATED_0 = V_FLIP_TEXTURE_COORD;

const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::CAMERA_TEXTURE_ROTATED_90 = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
};

const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::CAMERA_TEXTURE_ROTATED_180 = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
};

const std::array<GLfloat, RenderConfig::VERTEX_SIZE> RenderConfig::CAMERA_TEXTURE_ROTATED_270 = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
};


void RenderConfig::getFitTexCoord(int renderWidth, int renderHeight, int texWidth, int texHeight,
                                  std::array<GLfloat, VERTEX_SIZE> &texCoord) {
    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float renderAspectRatio = static_cast<float>(renderHeight) / renderWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if (texAspectRatio > renderAspectRatio) {
        int fitWidth = texWidth * renderHeight / texHeight;
        xOffset = static_cast<float >(fitWidth - renderWidth) / (2 * fitWidth);
    } else {
        int fitHeight = texHeight * renderWidth / texWidth;
        yOffset = static_cast<float >(fitHeight - renderHeight) / (2 * fitHeight);
    }
    texCoord = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };
}

void
RenderConfig::getFillTexCoord(int renderWidth, int renderHeight, int texWidth, int texHeight,
                              std::array<GLfloat, VERTEX_SIZE> &texCoord) {
    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float renderAspectRatio = static_cast<float>(renderHeight) / renderWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if (texAspectRatio > renderAspectRatio) {
        int fillHeight = texWidth * renderHeight / renderWidth;
        yOffset = static_cast<float>(texHeight - fillHeight) / (2 * texHeight);
    } else {
        int fillWidth = texHeight * renderWidth / renderHeight;
        xOffset = static_cast<float>(texWidth - fillWidth) / (2 * texWidth);
    }
    texCoord = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };
}