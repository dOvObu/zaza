#include "shaders.hpp"
#include "win.hpp"

void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); }
}
void resize_cb(GLFWwindow* window, int w, int h) { glViewport(0, 0, (GLsizei)w, (GLsizei)h); }

int main()
{
   auto w = gl::for_window().config().version(3, 3).depth_bits(24).w
       .mk(400,200,"Simple example")
      .key_cb(key_cb).resize_cb(resize_cb);

   unsigned vb, cb;

   float v[] = {
      -1, -1,
       1, -1,
      -1,  1,
       1,  1,
   };
   float c[] = {
       0.5,    0, 0.7,
       0.7,  0.5,   0,
         1, 0.25,   0,
      0.75,  0.5,   0,
   };
   unsigned el[] = {
      0, 1, 2,
      1, 3, 2,
   };
   unsigned ebo;
   unsigned prog, mat;
   
   gl::
      for_program().remember_material(mat)
         .attach_vert("./sh.vert").attach_frag("./sh.frag")
            .def_input() .vec2<0>(4, "v_pos", v)            .as(vb)
            .def_input() .vec3<1>(4, "v_col", c)            .as(cb)
            .def_order().set(6, el)                         .as(ebo)
         .link_it() .use()                                  .as(prog);
   
   while (w.is_active())
   {
      w.clear();
      gl::use(prog, mat);
      w.draw_by_order(GL_TRIANGLES, 6);
      w.show();
   }
   w.close();
}