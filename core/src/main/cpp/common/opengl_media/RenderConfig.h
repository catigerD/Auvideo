//
// Created by dengchong on 2019-11-29.
//

#ifndef CPP_RENDERCONFIG_H
#define CPP_RENDERCONFIG_H

#include <array>
#include <string>

#include <GLES3/gl3.h>


class RenderConfig {
public:
    static RenderConfig &getInstance() {
        static RenderConfig instance;
        return instance;
    }

public:

    static const std::string VERTEX_SOURCE;

    static const std::string FRAGMENT_SOURCE;

    static const int VERTEX_POS_SIZE{2};
    static const int VERTEX_TEX_SIZE{2};
    static const GLuint VERTEX_POS_INDEX{0};
    static const GLuint VERTEX_TEX_INDEX{1};
    static const int VERTEX_NUM{4};
    static const int VERTEX_SIZE{VERTEX_NUM * VERTEX_POS_SIZE};
    static const int VERTEX_PTR_SIZE{sizeof(GLfloat) * VERTEX_SIZE};

    static const std::array<GLfloat, VERTEX_SIZE> VERTEX_COORD;

    static const std::array<GLfloat, VERTEX_SIZE> TEXTURE_COORD;

    static const std::array<GLfloat, VERTEX_SIZE> V_FLIP_TEXTURE_COORD;

    static void getFitTexCoord(int renderWidth, int renderHeight, int texWidth, int texHeight,
                               std::array<GLfloat, VERTEX_SIZE> &texCoord);

    static void getFillTexCoord(int renderWidth, int renderHeight, int texWidth, int texHeight,
                                std::array<GLfloat, VERTEX_SIZE> &texCoord);

    //1. 先顺时针旋转 x 度。
    //2. 由于计算机图形坐标与 OpenGL坐标 y 轴相反，做 VFlip 操作
    static const std::array<GLfloat, VERTEX_SIZE> CAMERA_TEXTURE_ROTATED_0;

    static const std::array<GLfloat, VERTEX_SIZE> CAMERA_TEXTURE_ROTATED_90;

    static const std::array<GLfloat, VERTEX_SIZE> CAMERA_TEXTURE_ROTATED_180;

    static const std::array<GLfloat, VERTEX_SIZE> CAMERA_TEXTURE_ROTATED_270;

private:
    RenderConfig() = default;

    RenderConfig(const RenderConfig &) = default;
};


#endif //CPP_RENDERCONFIG_H
