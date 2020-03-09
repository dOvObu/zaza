#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2/SOIL2.h>

namespace gl {
   namespace With {
      void alpha_support(bool enabled = true) { if (enabled) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); } else { glDisable(GL_BLEND); } }
   }

   struct _TextureWrapVariants {
      struct _TextureOperators& t;
      inline struct _TextureOperators& repeat() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); return t; }
      inline struct _TextureOperators& mirrored() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); return t; }
      inline struct _TextureOperators& stretched() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); return t; }
      inline struct _TextureOperators& centralized() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); return t; }
      _TextureWrapVariants(struct _TextureOperators& _t) : t(_t) {}
   };

   struct _FilterVariants {
      struct _TextureOperators& t;
      _FilterVariants(struct _TextureOperators& _t) :t(_t) {}
      inline _TextureOperators& linear() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); return t; }
      inline _FilterVariants& linear_min() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); return *this; }
      inline _FilterVariants& linear_mag() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); return *this; }

      inline _TextureOperators& pixel() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); return t; }
      inline _FilterVariants& pixel_min() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); return *this; }
      inline _FilterVariants& pixel_mag() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); return *this; }
   };

   struct _MipMapFilterVariants {
      struct _TextureOperators& _t;
      void*& _data;

      _MipMapFilterVariants(struct _TextureOperators& t, void* data) :_t(t), _data(data) {}
      inline _MipMapFilterVariants& linlin() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); return *this; }
      inline _MipMapFilterVariants& pixlin() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); return *this; }
      inline _MipMapFilterVariants& linpix() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); return *this; }
      inline _MipMapFilterVariants& pixpix() { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); return *this; }
      inline _TextureOperators& mk() { glGenerateMipmap(GL_TEXTURE_2D); /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, _width, _height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, _data);*/ return _t; }
   };

   struct _TextureOperators {
      GLuint const t{ 0 };
      void* data{ nullptr };

      _TextureOperators(GLuint const _t) : t(_t) {}
      _TextureOperators(GLuint const _t, void* _data) : t(_t), data(_data) {}
      ~_TextureOperators() { if (data != nullptr) SOIL_free_image_data(reinterpret_cast<unsigned char*>(data)); }

      template<bool deb_mod> inline void deb();
      template<> inline void deb<true>() { if (data == nullptr) { std::cerr << "Can't create mipmaps without loaded image" << std::endl; abort(); } }
      template<> inline void deb<false>() {}

      inline _TextureWrapVariants wrap() { return _TextureWrapVariants(*this); }
      inline _TextureOperators& border_color(float r, float g, float b, float a = 1.f) { float const color[] = { 1.f, 0.f, 0.f, 1.f }; glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color); return *this; }
      inline _TextureOperators& border_color(float const* const& color) { glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color); return *this; }
      inline _FilterVariants filter() { return _FilterVariants(*this); }
      inline _MipMapFilterVariants mipmap() { deb<debug_mode>();  return _MipMapFilterVariants(*this, data); }
      inline void as(GLuint& _t) { _t = t; }
   };

   namespace For {
      static inline _TextureOperators texture(GLuint texId) { glBindTexture(GL_TEXTURE_2D, texId); return _TextureOperators(texId); }
      static inline _TextureOperators texture() { GLuint texId; glGenTextures(1, &texId); glBindTexture(GL_TEXTURE_2D, texId); return _TextureOperators(texId); }

      template<int gl_alpha, int soil_alpha> static inline _TextureOperators __texture(char const* name) {
         GLint width, height;
         GLuint texId;
         glGenTextures(1, &texId);
         glBindTexture(GL_TEXTURE_2D, texId);
         unsigned char* data = SOIL_load_image(name, &width, &height, 0, soil_alpha);
         if (debug_mode) std::cerr << "Loading RGBA image " << name << " ...\n" << SOIL_last_result() << '\n' << std::endl;
         glTexImage2D(GL_TEXTURE_2D, 0, gl_alpha, width, height, 0, gl_alpha, GL_UNSIGNED_BYTE, data);
         return _TextureOperators(texId, data);
      }
      static inline _TextureOperators texture(char const* name) { return __texture<GL_RGBA, SOIL_LOAD_RGBA>(name); }
      static inline _TextureOperators texture_without_alpha(char const* name) { return __texture<GL_RGB, SOIL_LOAD_RGB>(name); }
   }
}

#endif // TEXTURE_HPP
