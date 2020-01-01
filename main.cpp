#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <exception>
#include <iostream>


#include "win.hpp"

int main()
{
   try {
      Win wnd = {};
      wnd.run();

   } catch (std::exception const &err) {
      std::cout << err.what() << std::endl;
      system("pause");
   }

   return 0;
}
