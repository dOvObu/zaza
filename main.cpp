#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <exception>
#include <iostream>
#include "win.hpp"

struct BinTree : Serializable {
   sPtr l, r;
   BinTree(Serializable *l_, Serializable *r_) :l(l_), r(r_) {}
   sFields
   __(l)
   __(r)
   sEnd
};


int main()
{
   try {
      std::vector<BinTree> n;
      n.push_back({nullptr, nullptr});
      n.push_back({nullptr, nullptr});
      n.push_back({&n[n.size() - 1], &n[n.size() - 2]});
      std::ofstream file;
      n.back() >> file;

      //Win wnd = {"window_settings.txt"};
      //wnd.run();

   } catch (std::exception const &err) {
      std::cout << err.what() << std::endl;
      system("pause");
   }

   return 0;
}
