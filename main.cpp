#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <exception>
#include <iostream>
#include "win.hpp"

struct BinNode : Serializable {
   sPtr l, r;
   BinNode(Serializable *l_, Serializable *r_) :l(l_), r(r_) {}

   sFields
   ___(l)
   ___(r)
   sEnd
};
struct BinTree : Serializable {
   std::vector<BinNode> nodes;

   sFields_of(BinTree)
   ___vref(nodes)
   sEnd_
   
};


int main()
{
   try {
      BinTree tree;
      std::vector<BinNode>& nodes = tree.nodes;
      nodes.push_back({ nullptr, nullptr });
      nodes.push_back({ nullptr, nullptr });
      nodes.push_back({ &nodes[nodes.size() - 1], &nodes[nodes.size() - 2] });
      tree.update_fields();

      {
         std::ofstream file("window_settings.txt");
         tree >> file;
         file.close();
      }
      {
         std::ifstream file("window_settings.txt");
         std::string str;
      }
      //Win wnd = {"window_settings.txt"};
      //wnd.run();

   } catch (std::exception const &err) {
      std::cout << err.what() << std::endl;
      system("pause");
   }

   return 0;
}
