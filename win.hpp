#ifndef WIN_HPP
#define WIN_HPP

#include <memory>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string>

#include "serializable.hpp"
#include "irenderer.hpp"


struct WinSettings : Serializable {
   sStr  title    = {"Default Title"};
   sStr  renderer = {"OpenGL"};
   sUint width    = 640;
   sUint height   = 420;
   sInt fullscreen= 0;

   sFields
   __(title)
   __(renderer)
   __(width)
   __(height)
   __(fullscreen)
   sEnd
};


struct Win {
   Win();
   Win(char const * path_to_settings);
   Win(WinSettings const &win_settings);
   ~Win();
   void run();
private:
   void init(WinSettings const &win_settings);
   int width, height;
   std::unique_ptr<IRenderer> renderer;
};

#endif // WIN_HPP
