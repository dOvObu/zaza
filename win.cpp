#include "win.hpp"
#include "opengl_renderer.hpp"

Win::Win()
{
   WinSettings ws = {};
   init(ws);
}

Win::Win(char const *path_to_win_settings)
{
   throw std::runtime_error("Error! Deserialization steel is not implemented");
}

Win::Win(WinSettings const &ws)
{
   init(ws);
}

Win::~Win()
{
   renderer->shutdown();
}

void Win::run()
{
   while (!renderer->is_closed()) {
      renderer->clear({255, 255, 255});
      renderer->show();
   }
}

void Win::init(WinSettings const &ws)
{
   if (ws.renderer.n == "OpenGL") {
      renderer = std::unique_ptr<IRenderer>(new OpenglRenderer());
   }
   renderer->setup({ws.width.n, ws.height.n}, width, height, ws.title.n.c_str(), ws.fullscreen.n);
}


