#include "opengl_renderer.hpp"

/*  Window The window that received the event.
 *  Key The [keyboard key](@ref keys) that was pressed or released.
 *  Scancode The system-specific scancode of the key.
 *  Action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
 *  Mods Bit field describing which [modifier keys](@ref mods) were held down.
 */
void cb_key(GLFWwindow *hwnd, int glfw_key, int specific_scancode_of_the_key, int glfw_action, int glfw_mod_keys)
{
}


/*  Window The window that received the event.
 *  Button The [mouse button](@ref buttons) that was pressed or released.
 *  Action One of `GLFW_PRESS` or `GLFW_RELEASE`.
 *  Mods Bit field describing which [modifier keys](@ref mods) were held down.
 */
void cb_mouse(GLFWwindow *hwnd, int glfw_btn, int glfw_action, int glfw_mod_keys)
{
}


/*  Window The window that received the event.
 *  The scroll offset along the x-axis.
 *  The scroll offset along the y-axis.
 */
void cb_scroll(GLFWwindow *hwnd, double xoffset, double yoffset)
{
}


/*  Window The window that received the event.
 *  The mouse position along the x-axis.
 *  The mouse position along the y-axis.
 */
void cb_pos(GLFWwindow* window, double xpos, double ypos)
{
}

void OpenglRenderer::setup(glm::uvec2 sz, int &width, int &height, char const *title, bool fullscreen)
{
   if (!glfwInit()) throw std::runtime_error("Error! Can't init glfw");
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_RESIZABLE, !fullscreen ? GL_TRUE : GL_FALSE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   hwnd = glfwCreateWindow(sz.x, sz.y, title, nullptr, nullptr);
   if (!hwnd) {
      glfwTerminate();
      throw std::runtime_error("Error! Can't create a window with glfw");
   }

   glfwMakeContextCurrent(hwnd);
   glfwGetFramebufferSize(hwnd, &width, &height);

   glEnable(GLFW_STICKY_KEYS);

   glfwSetKeyCallback(hwnd, cb_key);
   glfwSetMouseButtonCallback(hwnd, cb_mouse);
   glfwSetScrollCallback(hwnd, cb_scroll);
   glfwSetCursorPosCallback(hwnd, cb_pos);

   glfwSetInputMode(hwnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

   //glewExperimental = GL_TRUE;
   if (glewInit() != GLEW_OK) {
      glfwTerminate();
      throw std::runtime_error("Error! Can't init glew");
   }
   glViewport(0, 0, width, height);
   glEnable( GL_DEPTH_TEST );
   // enable alpha support
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


void OpenglRenderer::clear(glm::ivec3 color)
{
   glClearColor((float)color.x/255, (float)color.y/255, (float)color.z/255, 1.f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OpenglRenderer::draw_line(glm::vec2 from, glm::vec2 to, glm::ivec4 color)
{
   glBegin(GL_LINE);
      glColor4f(color.x,color.y,color.z,color.w);
      glVertex2f(from.x, from.y);
      glVertex2f(to.x, to.y);
   glEnd();
}


void OpenglRenderer::show()
{
   glfwSwapBuffers(hwnd);
   glfwPollEvents( );
}


bool OpenglRenderer::is_closed()
{
   return glfwWindowShouldClose(hwnd);
}


void OpenglRenderer::shutdown()
{
   glfwTerminate();
}
