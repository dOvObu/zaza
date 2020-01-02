#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

enum class sType {unknown_, object_, int_, uint_, float_, double_, str_, vec_, map_, ptr_};

struct Serializable {
	static std::string depth;
	bool marked = false;
	virtual sType type(){return sType::unknown_;}
	virtual bool is(sType t){return true;}
	virtual struct sMap* get_fields(){return nullptr;}
};

#define DEF_SERIALIZABLE(Name, Type, sType_) \
	struct s ## Name : Serializable { \
		Type n; \
		s ## Name(){} \
		s ## Name(Type i):n(i){} \
		sType type() override {return sType :: sType_;} \
		bool is(sType t) override {return t == sType::sType_;} \
	};
#define filed std::map<std::string, Serializable*>
   DEF_SERIALIZABLE(Int, int, int_)
   DEF_SERIALIZABLE(Uint, unsigned, uint_)
   DEF_SERIALIZABLE(Float, float, float_)
   DEF_SERIALIZABLE(Double, double, double_)
   DEF_SERIALIZABLE(Str, std::string, str_)
   DEF_SERIALIZABLE(Vec, std::vector<Serializable*>, vec_)
   DEF_SERIALIZABLE(Ptr, Serializable*, ptr_)
   DEF_SERIALIZABLE(Map, filed, map_)
#undef field
#undef DEF_SERIALIZABLE
//std::vector<std::unique_ptr<Serializable>> serializable_pool;


static std::ostream& operator << (std::ostream& s, sInt& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sUint& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sFloat& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sDouble& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sStr& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sPtr& i) {/*s << i.n; if (!i.shown.count(i.n)) i.ptrs.insert(i.n);*/ return s;}

static std::ostream& operator << (std::ostream& s, sVec& l);
static std::ostream& operator << (std::ostream& s, sMap& l);
#define INCASE(Name, sType_, val) case sType::sType_: s << *reinterpret_cast<s ## Name*>(val); s << '\n'; break;
#define CASES(val) \
	switch(val->type()) { \
		INCASE(Int, int_, val) INCASE(Uint, uint_, val) INCASE(Float, float_, val) \
		INCASE(Str, str_, val) INCASE(Vec, vec_, val) INCASE(Map, map_, val) \
		INCASE(Double, double_, val) \
		case sType::object_: l.depth += ' '; s << '\n' << *val->get_fields(); l.depth.pop_back(); break; \
		default: s << '\n'; break; \
	}

static std::ostream& operator << (std::ostream& s, sVec& l) {
   s << "[\n";
	for(auto& i : l.n) {
			s << l.depth;
			CASES(i)
	}
	s << l.depth << "]\n";
	return s;
}
static std::ostream& operator << (std::ostream& s, sMap& l) {
  for(auto& i : l.n) {
		s << l.depth << i.first << " : ";
    CASES(i.second)
  }
	return s;
}
static std::istream& operator >> (std::istream& s, sMap& l) {

   std::vector<sMap*> map_stack;
   std::vector<sVec*> vec_stack;
   std::vector<sInt*> int_stack;
   std::vector<sUint*> uint_stack;
   std::vector<sFloat*> float_stack;
   std::vector<sDouble*> double_stack;
   std::vector<sPtr*> ptr_stack;
   std::vector<sStr*> str_stack;
   std::vector<sType> type_stack;

   std::string bf, key;
   size_t depth = 0, prew_depth = 0;
   bool is_key = true, is_next_map = false;

   map_stack.push_back(&l);
   type_stack.push_back(sType::map_);

   while (!s.eof()) {
      std::cout << "bf: " << bf << std::endl;
      std::cout << "key: " << key << '\n' << std::endl;
      int c = s.get();

      if (is_key) {
         if (c == ' ' || c == '\t') {
            if (key.size() != 0) ++depth;
            continue;
         }
         if (c == ':') {
            is_key = false;
            if (s.get() == '\n') {
               prew_depth = depth;
               is_next_map = true;
            }
            if (type_stack.back() == sType::map_) {
               sType type = map_stack.back()->n[key]->type();
               if (type == sType::double_) {
                  double_stack.push_back(reinterpret_cast<sDouble*>(map_stack.back()->n[key]));
               } else if (type == sType::float_) {
                  float_stack.push_back(reinterpret_cast<sFloat*>(map_stack.back()->n[key]));
               } else if (type == sType::int_) {
                  int_stack.push_back(reinterpret_cast<sInt*>(map_stack.back()->n[key]));
               } else if (type == sType::uint_) {
                  uint_stack.push_back(reinterpret_cast<sUint*>(map_stack.back()->n[key]));
               } else if (type == sType::str_) {
                  str_stack.push_back(reinterpret_cast<sStr*>(map_stack.back()->n[key]));
               } else if (type == sType::map_) {
                  map_stack.push_back(reinterpret_cast<sMap*>(map_stack.back()->n[key]));
               } else if (type == sType::vec_) {
                  vec_stack.push_back(reinterpret_cast<sVec*>(map_stack.back()->n[key]));
               } else if (type == sType::vec_) {
                  ptr_stack.push_back(reinterpret_cast<sPtr*>(map_stack.back()->n[key]));
               }
               key.clear();
               type_stack.push_back(type);
            }
            continue;
         }
         key.push_back(c);
      } else {
         if (c == '\n') {
            if (bf.size() == 0) {
               prew_depth = depth;
               is_next_map = true;
            } else {
               sType type = type_stack.back();
               type_stack.pop_back();

               if (type == sType::double_) {
                  double_stack.back()->n = std::stod(bf);
                  bf.clear();
               } else if (type == sType::float_) {
                  float_stack.back()->n = std::stof(bf);
                  bf.clear();
               } else if (type == sType::int_) {
                  int_stack.back()->n = std::stoul(bf);
                  bf.clear();               } else if (type == sType::uint_) {
                  uint_stack.back()->n = std::stoi(bf);
                  bf.clear();
               } else if (type == sType::str_) {


                  std::cout << "[bf1]" << bf << std::endl;
                  str_stack.back()->n = bf;
                  std::cout << "[bf2]" << bf << std::endl;



                  bf.clear();
               } else {
                  type_stack.push_back(type);
               }
            }
            is_key = type_stack.back() == sType::map_;
            if (is_key) depth = 0;
         } else {
            bf.push_back(c);
         }
      }
   }
}

#undef INCASE

#define sFields sMap _fields_ = {{
	#define __(Name) {#Name, &(this-> Name)},
#define sEnd }}; \
\
	std::ostream& operator << (std::ostream& s) {s << this->_fields_; return s;} \
	sMap* get_fields() override {return &this->_fields_;} \
	sType type() {return sType::object_;} \
	bool is(sType t) {return t == sType::object_;}



#endif // SERIALIZABLE_HPP
