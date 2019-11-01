#pragma once

#include <iosfwd>
#include "../Graphics/Renderer.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Graphics/TransFlags.h"

namespace Falltergeist
{
    namespace Graphics
    {
        class Animation
        {
            public:
                Animation(const std::string& filename);
                ~Animation();
                void render(int x, int y, unsigned int direction, unsigned int frame, bool transparency = false, bool light = false, int outline = 0,
                            unsigned int lightValue=0);
                bool opaque(unsigned int x, unsigned int y);
                void trans(Graphics::TransFlags::Trans _trans);

            private:
                GLuint _vao;
                GLuint _coordsVBO;
                GLuint _texCoordsVBO;
                GLuint _ebo;
                Texture* _texture;
                int _stride;
                Graphics::TransFlags::Trans _trans = Graphics::TransFlags::Trans::NONE;

                std::vector<glm::vec2> _vertices;
                std::vector<glm::vec2> _texCoords;

                GLint _uniformTex;
                GLint _uniformTexSize;
                GLint _uniformFade;
                GLint _uniformMVP;
                GLint _uniformCnt;
                GLint _uniformLight;
                GLint _uniformTrans;
                GLint _uniformOffset;
                GLint _uniformOutline;
                GLint _uniformTexStart;
                GLint _uniformTexHeight;

                GLint _attribPos;
                GLint _attribTex;
                Graphics::Shader*_shader;
        };
    }
}
