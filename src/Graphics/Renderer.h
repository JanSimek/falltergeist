#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include "../Graphics/Point.h"
#include "../Graphics/Size.h"

namespace Falltergeist
{
    namespace Graphics
    {
        class Texture;

        #define GL_CHECK(x) do { \
            x; \
            int _err = glGetError(); \
            if (_err) { \
                printf("GL Error %d at %d, %s in %s", _err, __LINE__, __func__, __FILE__); \
                exit(-1); \
            } \
        } while (0)

        class Renderer
        {
            public:
                enum class RenderPath
                {
                    OGL21 = 0,
                    OGL32,
                    GLES1,
                    GLES2
                };

                Renderer(unsigned int width, unsigned int height);
                Renderer(const Size& size);
                ~Renderer();

                void init();

                void beginFrame();
                void endFrame();
                void think();

                int width();
                int height();
                const Size& size() const;

                float scaleX();
                float scaleY();

                bool fadeDone();
                bool fading();

                void fadeIn(uint8_t r, uint8_t g, uint8_t b, unsigned int time, bool inmovie = false);
                void fadeOut(uint8_t r, uint8_t g, uint8_t b, unsigned int time, bool inmovie = false);

                void setCaption(const std::string& caption);
                SDL_Window* sdlWindow();

                GLuint getVAO();
                GLuint getVVBO();
                GLuint getTVBO();
                GLuint getEBO();
                glm::mat4 getMVP();

                void drawRect(int x, int y, int w, int h, SDL_Color color);
                void drawRect(const Point &pos, const Size &size, SDL_Color color);

                glm::vec4 fadeColor();

                void screenshot();

                int32_t maxTextureSize();

                Texture* egg();

                RenderPath renderPath();

            protected:
                Size _size;
                RenderPath _renderpath = RenderPath::OGL21;

                short _fadeStep = 0;
                unsigned int _fadeTimer = 0;
                unsigned int _fadeDelay = 0;
                unsigned int _fadeAlpha = 0;
                bool _fadeDone = true;
                SDL_Color _fadeColor = {0, 0, 0, 0};

                bool _inmovie = false;

                float _scaleX = 1.0;
                float _scaleY = 1.0;

                SDL_Window* _sdlWindow;
                SDL_GLContext _glcontext;
                GLuint _vao;
                GLuint _ebo;
                GLuint _texcoord_vbo;
                GLuint _coord_vbo;
                glm::mat4 _MVP;
                GLint _major;
                GLint _minor;
                int32_t _maxTexSize;

                Texture* _egg;
        };
    }
}
