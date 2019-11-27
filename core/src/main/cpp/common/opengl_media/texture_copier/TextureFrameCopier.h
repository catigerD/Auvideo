//
// Created by dengchong on 2019-09-27.
//

#ifndef AUVIDEO_TEXTUREFRAMECOPIER_H
#define AUVIDEO_TEXTUREFRAMECOPIER_H

#include <memory>

#include "TextureFrame.h"
#include "AndroidLog.h"
#include "matrix.h"
#include "RenderConfig.h"

class TextureFrameCopier {
public:
    TextureFrameCopier(std::string verSource = RenderConfig::VERTEX_SOURCE,
                       std::string fragSource = RenderConfig::FRAGMENT_SOURCE)
            : vertexSource(std::move(verSource)),
              fragmentSource(std::move(fragSource)) {}


    virtual ~TextureFrameCopier() = default;

    virtual void init() = 0;

    virtual void renderWithCoords(std::shared_ptr<TextureFrame> textureFrame, GLuint outputTexId,
                                  const std::array<GLfloat, RenderConfig::VERTEX_SIZE> &verCoord,
                                  const std::array<GLfloat, RenderConfig::VERTEX_SIZE> &texCoord) = 0;

    virtual void destroy() = 0;

protected:
    const std::string vertexSource;
    const std::string fragmentSource;
    GLuint programId{};
};


#endif //AUVIDEO_TEXTUREFRAMECOPIER_H
