#ifndef OPENGL_RENDERER_HPP
#define OPENGL_RENDERER_HPP

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <stdexcept>
#include "irenderer.hpp"

struct OpenglRenderer : IRenderer {
   GLFWwindow* hwnd{ nullptr };

   void setup(glm::uvec2 window_size, int &width, int &height, char const *title, bool fullscreen) override;
   void clear(glm::ivec3 color) override;
   void draw_line(glm::vec2 from, glm::vec2 to, glm::ivec4 color) override;
   void show(void) override;
   bool is_closed(void) override;
   void shutdown(void) override;
};

#endif // OPENGL_RENDERER_HPP
