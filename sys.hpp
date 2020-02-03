#ifndef SYS_HPP
#define SYS_HPP
#include <iostream>
#include <cstdio>

namespace sys {

   struct _FileOperations {
      FILE* _f{ nullptr };
      _FileOperations(FILE* f) :_f(f) {}

      inline _FileOperations& from_start_step(long n = 0) { fseek(_f, n, SEEK_SET); return *this; }
      inline _FileOperations& from_current_step(long n = 0) { fseek(_f, n, SEEK_CUR); return *this; }
      inline _FileOperations& from_end_step(long n = 0) { fseek(_f, n, SEEK_END); return *this; }
      inline _FileOperations& restart() { rewind(_f);             return *this; }

      inline long get_current_pos() { return ftell(_f); }
      inline long len() { auto const& l = from_end_step().get_current_pos(); restart(); return l; }
      inline char const* read() { long const& l = len(); auto const* s = new char[l + 1]; fread(const_cast<char*>(s), sizeof(char), l, _f); const_cast<char*>(s)[l] = '\0'; return s; }
   };
   inline _FileOperations for_file(FILE* f) { if (f == nullptr) (std::cerr << "Can't open file" << std::endl), abort(); return _FileOperations(f); }
}



#endif // SYS_HPP
