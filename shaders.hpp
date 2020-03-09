#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

#include "deb_mode.hpp"
#include "sys.hpp"
#include "texture.hpp"



namespace gl {
   struct _ShaderOperations {
      GLuint sh{ 0 };
      GLint const& type{ GL_FRAGMENT_SHADER };
      char const* name{ nullptr };
      _ShaderOperations(unsigned const& _sh) : sh(_sh) {}
      _ShaderOperations(unsigned const& _sh, int const& _type) : sh(_sh), type(_type) {}

      template<bool deb_mod> inline void deb();
      template<> inline void deb<true>() { GLint compiled; glGetShaderiv(sh, GL_COMPILE_STATUS, &compiled); std::cerr << "Compiling shader " << name << " ..." << std::endl; char inf_log[512]; glGetShaderInfoLog(sh, 512, NULL, inf_log); std::cerr << inf_log << '\n' << std::endl; if (not compiled) { std::cerr << "ERROR::Can't compile shader " << name << std::endl; abort(); } }
      template<> inline void deb<false>() {}

      inline _ShaderOperations& from_code(char const* code) { glShaderSource(sh, 1, &code, nullptr); return *this; }
      inline _ShaderOperations& compile() { glCompileShader(sh); deb<debug_mode>(); return *this; }
      inline _ShaderOperations& from_file(char const* path, char const*& code) { name = path; FILE* file{ nullptr }; fopen_s(&file, path, "rb"); code = sys::for_file(file).read(); from_code(code).compile(); return *this; }
      inline _ShaderOperations& from_file(char const* path) { char const* code; from_file(path, code); /*delete[] code;*/ return *this; }
   };
   static GLuint _id_by_shader_program_operators(struct _ShaderProgramOperators&);
   static GLuint _draw_type_by_shader_program_operators(_ShaderProgramOperators&);

   namespace For {
      static inline _ShaderOperations shader(GLuint const& sh, GLint const& type) { return struct _ShaderOperations(sh, type); }
      static inline _ShaderOperations shader(GLint const& type) { unsigned const& sh = glCreateShader(type); if (debug_mode && not sh) { std::cerr << "Can't create a shader" << std::endl; char inf_log[512]; glGetShaderInfoLog(sh, 512, NULL, inf_log); std::cerr << inf_log << std::endl; abort(); } return struct _ShaderOperations(sh, type); }
   }


   struct _ShaderParametersOperators {
      struct _ShaderProgramOperators& end;
      GLuint bf{ 0 };

      _ShaderParametersOperators(struct _ShaderProgramOperators& _p, int const& n = 1) : end(_p) { glGenBuffers(n, &bf); }
      inline void bind() { glBindBuffer(GL_ARRAY_BUFFER, bf); }
      inline _ShaderParametersOperators& as(GLuint& bf) { bf = this->bf; return *this; }

      template<GLuint layout_loc> inline _ShaderParametersOperators& vec4(size_t num_vert, char const* name, void const* data) { bind(); glBufferData(GL_ARRAY_BUFFER, num_vert * 4 * sizeof(float), data, _draw_type_by_shader_program_operators(end)); glEnableVertexAttribArray(layout_loc); glVertexAttribPointer(layout_loc, 4, GL_FLOAT, GL_FALSE, 0, 0); bind(); glBindAttribLocation(_id_by_shader_program_operators(end), bf, name); return *this; }
      template<GLuint layout_loc> inline _ShaderParametersOperators& vec3(size_t num_vert, char const* name, void const* data) { bind(); glBufferData(GL_ARRAY_BUFFER, num_vert * 3 * sizeof(float), data, _draw_type_by_shader_program_operators(end)); glEnableVertexAttribArray(layout_loc); glVertexAttribPointer(layout_loc, 3, GL_FLOAT, GL_FALSE, 0, 0); bind(); glBindAttribLocation(_id_by_shader_program_operators(end), bf, name); return *this; }
      template<GLuint layout_loc> inline _ShaderParametersOperators& vec2(size_t num_vert, char const* name, void const* data) { bind(); glBufferData(GL_ARRAY_BUFFER, num_vert * 2 * sizeof(float), data, _draw_type_by_shader_program_operators(end)); glEnableVertexAttribArray(layout_loc); glVertexAttribPointer(layout_loc, 2, GL_FLOAT, GL_FALSE, 0, 0); bind(); glBindAttribLocation(_id_by_shader_program_operators(end), bf, name); return *this; }
   };

   
   struct _ShaderUniformsOperators {
      struct _ShaderProgramOperators& p;
      _ShaderUniformsOperators(struct _ShaderProgramOperators& _p) : p(_p) {}

#define SAMPLER2D_DEFINITION(_TEX_NUM) inline struct _ShaderProgramOperators& sampler2D_##_TEX_NUM(char const* name, GLuint const& t) { glActiveTexture(GL_TEXTURE##_TEX_NUM); glBindTexture(GL_TEXTURE_2D, t); glUniform1i(glGetUniformLocation(_id_by_shader_program_operators(p), name), _TEX_NUM); return p; }
      SAMPLER2D_DEFINITION(0) SAMPLER2D_DEFINITION(1) SAMPLER2D_DEFINITION(2) SAMPLER2D_DEFINITION(3) SAMPLER2D_DEFINITION(4) SAMPLER2D_DEFINITION(5) SAMPLER2D_DEFINITION(6) SAMPLER2D_DEFINITION(7) SAMPLER2D_DEFINITION(8) SAMPLER2D_DEFINITION(9) SAMPLER2D_DEFINITION(10) SAMPLER2D_DEFINITION(11)
      SAMPLER2D_DEFINITION(12) SAMPLER2D_DEFINITION(13) SAMPLER2D_DEFINITION(14) SAMPLER2D_DEFINITION(15) SAMPLER2D_DEFINITION(16) SAMPLER2D_DEFINITION(17) SAMPLER2D_DEFINITION(18) SAMPLER2D_DEFINITION(19) SAMPLER2D_DEFINITION(20) SAMPLER2D_DEFINITION(21) SAMPLER2D_DEFINITION(22)
      SAMPLER2D_DEFINITION(23) SAMPLER2D_DEFINITION(24) SAMPLER2D_DEFINITION(25) SAMPLER2D_DEFINITION(26) SAMPLER2D_DEFINITION(27) SAMPLER2D_DEFINITION(28) SAMPLER2D_DEFINITION(29) SAMPLER2D_DEFINITION(30) SAMPLER2D_DEFINITION(31)
#undef SAMPLER2D_DEFINITION

      inline _ShaderProgramOperators& f32(char const* name, GLfloat f) {
         glUniform1f(glGetUniformLocation(_id_by_shader_program_operators(p), name), f);
         return p;
      }
   };


   struct _ShaderParametersOrderOperators {
      struct _ShaderProgramOperators& end;
      GLuint bf{ 0 };

      _ShaderParametersOrderOperators(struct _ShaderProgramOperators& _p, int const& n = 1) : end(_p) { glGenBuffers(n, &bf); }
      inline void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bf); }
      inline _ShaderParametersOrderOperators& as(GLuint& bf) { bf = this->bf; return *this; }

      template<typename T, std::size_t L> inline _ShaderParametersOrderOperators& set(T(&data)[L]) { bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, _draw_type_by_shader_program_operators(end)); bind(); return *this; }
      inline _ShaderParametersOrderOperators& set(size_t L, GLuint* data) { bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, L * sizeof(GLuint), data, _draw_type_by_shader_program_operators(end)); bind(); return *this; }
   };


   struct _ShaderProgramOperators {
      GLuint const p{ 0 };
      GLuint v{ 0 }, f{ 0 };
      GLuint draw_type{ GL_STATIC_DRAW };
      char const* v_code{ nullptr };
      char const* f_code{ nullptr };
      inline _ShaderParametersOperators def_input() { return _ShaderParametersOperators(*this); }
      inline _ShaderUniformsOperators uni_input() { return _ShaderUniformsOperators(*this); }
      inline _ShaderParametersOrderOperators def_order() { return _ShaderParametersOrderOperators(*this); }
      inline _ShaderProgramOperators remember_material(GLuint& vao) { glGenVertexArrays(1, &vao); glBindVertexArray(vao); return *this; }

      _ShaderProgramOperators(GLuint const _p) : p(_p) {}
      ~_ShaderProgramOperators() = default;
      
      template<bool deb_mod> inline void deb();
      template<> inline void deb<true>() { GLint linked; glGetProgramiv(p, GL_LINK_STATUS, &linked); std::cerr << "Linking a program ..." << std::endl; char inf_log[512]; glGetProgramInfoLog(p, 512, NULL, inf_log); std::cerr << inf_log << '\n' << std::endl; if (not linked) { std::cerr << "ERROR::Can't link a program!" << std::endl; abort(); } }
      template<> inline void deb<false>() {}

      inline _ShaderProgramOperators& draw_static() { draw_type = GL_STATIC_DRAW; return *this; }
      inline _ShaderProgramOperators& draw_dynamic() { draw_type = GL_DYNAMIC_DRAW; return *this; }
      inline _ShaderProgramOperators& draw_stream() { draw_type = GL_STREAM_DRAW; return *this; }

      inline _ShaderProgramOperators& attach_vert(char const* path) { glAttachShader(p, v = gl::For::shader(GL_VERTEX_SHADER).from_file(path, v_code).sh); delete[] v_code;  return *this; }
      inline _ShaderProgramOperators& attach_frag(char const* path) { glAttachShader(p, f = gl::For::shader(GL_FRAGMENT_SHADER).from_file(path, f_code).sh); delete[] f_code;  return *this; }
      inline _ShaderProgramOperators& link_it() { glLinkProgram(p); deb<debug_mode>(); if (v) glDeleteShader(v), v = NULL; if (f) glDeleteShader(f), f = NULL; return *this; }
      inline _ShaderProgramOperators& use() { glUseProgram(p);  return *this; }
      inline _ShaderProgramOperators& as(GLuint& p) { p = this->p; return *this; }
   };
   static GLuint _draw_type_by_shader_program_operators(_ShaderProgramOperators& p) { return p.draw_type; }
   static GLuint _id_by_shader_program_operators(_ShaderProgramOperators& p) { return p.p; }
   namespace For {
      static inline _ShaderProgramOperators program() { GLuint p = glCreateProgram(); return _ShaderProgramOperators(p); }
      static inline _ShaderProgramOperators program(GLuint p) { return _ShaderProgramOperators(p); }
   }

   namespace drop {
      void vao(GLuint const& ui) { glDeleteVertexArrays(1, &ui); }
      void input(GLuint const& ui) { glDeleteBuffers(1, &ui); }
      void program(GLuint const& ui) { glDeleteProgram(ui); }
      void texture(GLuint const& ui) { glDeleteTextures(1, &ui); }
   }

   static void use(GLuint& prog, GLuint& vao) { glBindVertexArray(vao); glUseProgram(prog); }
}


#endif // SHADERS_HPP 
