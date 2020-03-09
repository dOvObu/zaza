#include "shaders.hpp"
#include "win.hpp"


void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
   }
}
void resize_cb(GLFWwindow* window, int w, int h) { glViewport(0, 0, (GLsizei)w, (GLsizei)h); }


int main()
{
   auto& window = gl::For::window().config(3, 3).depth_bits(24).bk().mk("simple zaza-api").key_cb(key_cb).resize_cb(resize_cb);
   GLuint material, shape_texture;
   float shape[] = {
      -1, 1,
      -1,-1,
       1,-1,
       1, 1,
   };
   int order[] = { 0,1,2,  2,3,0 };

   gl::For::program().attach_vert("sh.vert").attach_frag("sh.frag").draw_dynamic().remember_material(material)
      .def_input().vec2<0>(4, "shape_texture", (void*)shape).end
      .def_order().set(order).end
   .link_it().use()
   ;

   while (window.is_active()) {
      window.clear();
      window.draw_by_order(GL_TRIANGLES, 6);
      window.show();
   }
   window.close();
}
