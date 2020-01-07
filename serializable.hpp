#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

enum class sType {unknown_, object_, obj_ptr_, int_, uint_, float_, double_, str_, vec_, vec_int_, vec_uint_, vec_float_, vec_double_, vec_str_, map_, ptr_};

struct Serializable {
	static std::string depth;
   static Serializable* root_obj;
   static std::map<void*, std::vector<std::shared_ptr<struct sVec>>> sVec_pool;
   static void depth_push() {depth+="   ";}
   static void depth_pop()
   {
      if (depth.size()) {
         for(size_t i=0;3>i;++i) depth.pop_back();
      }
   }
	int marked{ 0 };
	virtual sType type(){return sType::unknown_;}
	virtual bool is(sType t){return true;}
	virtual struct sObj* get_fields(){return nullptr;}
   virtual void custom_write(std::ostream& s) {}
   virtual void custom_read(std::istream& s) {}
   //virtual void free_vectors_memory() {}

   //~Serializable() { this->free_vectors_memory(); }
};

#define filed(FiledType) std::map<std::string, FiledType>
#define DEF_SERIALIZABLE(Name, Type, sType_) \
   struct s ## Name : Serializable { \
		Type n; \
		s ## Name(){} \
		s ## Name(Type i):n(i){} \
		sType type() override {return sType :: sType_;} \
		bool is(sType t) override {return t == sType::sType_;} \
	};
   DEF_SERIALIZABLE(Int, int, int_)
   DEF_SERIALIZABLE(Str, std::string,   str_)
   DEF_SERIALIZABLE(Map, filed(Serializable*), map_)
   DEF_SERIALIZABLE(Obj, filed(Serializable*), object_)
   DEF_SERIALIZABLE(Uint,   unsigned, uint_  )
   DEF_SERIALIZABLE(Float,  float,    float_ )
   DEF_SERIALIZABLE(Double, double,   double_)
   DEF_SERIALIZABLE(VecInt,    std::vector<int>,      vec_int_   )
   DEF_SERIALIZABLE(VecStr, std::vector<std::string>, vec_str_   )
   DEF_SERIALIZABLE(VecUint,   std::vector<unsigned>, vec_uint_  )
   DEF_SERIALIZABLE(VecFloat,  std::vector<float>,    vec_float_ )
   DEF_SERIALIZABLE(VecDouble, std::vector<double>,   vec_double_)

   struct sPtr : Serializable{
   	Serializable* n;
   	sPtr(){}
      sPtr(Serializable* i) :n(i) { if(i != nullptr) ++(i->marked); }
      ~sPtr() { if (n != nullptr) --(n->marked); }
   	sType type() override {return sType :: ptr_;}
   	bool is(sType t) override {return t == sType::ptr_;}
	};
#undef field
#undef DEF_SERIALIZABLE

struct sVec : Serializable {
   std::vector<Serializable*> n;
   sVec() {}
   sVec(std::vector<Serializable*> i):n(i) {for(auto&it:n)++(it->marked);}
   ~sVec() {for(auto&it:n)--(it->marked);}
   void push_back(Serializable*i) {n.push_back(i);if(i!=nullptr)++(i->marked);}
   Serializable* pop_back() {Serializable*i=n.back();n.pop_back();if(i!=nullptr)--(i->marked);return i;}
   sType type() override {return sType::vec_;}
   bool is(sType t) override {return t == sType::vec_;}
};

static std::ostream& operator << (std::ostream &s, sPtr      &i) {if(i.n==nullptr)s<<"[]";else{s<<i.n;i.n->marked=true;}return s;} /*if (!i.shown.count(i.n)) i.ptrs.insert(i.n);*/
static std::ostream& operator << (std::ostream &s, sInt      &i) {s<<i.n; return s;}
static std::ostream& operator << (std::ostream &s, sStr      &i) {s<<i.n; return s;}
static std::ostream& operator << (std::ostream &s, sUint     &i) {s<<i.n; return s;}
static std::ostream& operator << (std::ostream &s, sFloat    &i) {s<<i.n; return s;}
static std::ostream& operator << (std::ostream &s, sDouble   &i) {s<<i.n; return s;}
static std::ostream& operator << (std::ostream &s, sVecStr   &i) {for(auto&it:i.n) s<<'\n'<<i.depth<<it; return s;}
static std::ostream& operator << (std::ostream &s, sVecInt   &i) {bool not_first=false;for(auto&it:i.n){if(not_first)s<<',';else{not_first=true;}s<<i.depth<<it;}return s;}
static std::ostream& operator << (std::ostream &s, sVecUint  &i) {bool not_first=false;for(auto&it:i.n){if(not_first)s<<',';else{not_first=true;}s<<i.depth<<it;}return s;}
static std::ostream& operator << (std::ostream &s, sVecFloat &i) {bool not_first=false;for(auto&it:i.n){if(not_first)s<<',';else{not_first=true;}s<<i.depth<<it;}return s;}
static std::ostream& operator << (std::ostream &s, sVecDouble&i) {bool not_first=false;for(auto&it:i.n){if(not_first)s<<',';else{not_first=true;}s<<i.depth<<it;}return s;}
static std::ostream& operator << (std::ostream &s, sVec      &l);
static std::ostream& operator << (std::ostream &s, sObj      &l);
#define INCASE(Name, sType_, val) case sType::sType_: s << *reinterpret_cast<s ## Name*>(val); s << '\n'; break;
#define CASES(val) \
	switch(val->type()) { \
		INCASE(Int, int_, val) INCASE(Uint, uint_, val) INCASE(Float, float_, val) INCASE(Double, double_, val) INCASE(Str, str_, val) INCASE(Ptr, ptr_, val) \
		INCASE(VecInt, vec_int_, val) INCASE(VecUint, vec_uint_, val) INCASE(VecFloat, vec_float_, val) INCASE(VecDouble, vec_double_, val) INCASE(VecStr, vec_str_, val) \
		INCASE(Vec, vec_, val) INCASE(Obj, object_, val) \
		default: s << '\n'; break; \
	}

/*static
std::ostream& operator << (std::ostream& s, sVec& l)
{
   for(auto& i : l.n) {
      s << l.depth; CASES(i)
   }
   return s;
}*/

static std::ostream& operator << (std::ostream& s, sObj& l)
{
   for(auto& i : l.n) {
      s << Serializable::depth << i.first;
      if (i.second->marked) s << '[' << i.second << ']';
      s << " : ";
      CASES(i.second)
   }
   return s;
}

static std::ostream& operator << (std::ostream& s, sVec& i) {
   i.depth_push(); s << '[' << i.n.size() << '\n';
   bool not_first = false;
   for (auto& it : i.n) {
      if (it->type() == sType::obj_ptr_) {
         if (not_first) s << "---\n"; else not_first = true;
         reinterpret_cast<sObj*>(it)->custom_write(s);
      }
   }
   s << ']'; i.depth_pop(); return s;
}


static std::istream& operator << (std::istream& s, sObj& l)
{
   std::vector<sObj*> obj_stack;
   std::vector<sMap*> map_stack;
   std::vector<sVec*> vec_stack;
   std::vector<sInt*> int_stack;
   std::vector<sUint*> uint_stack;
   std::vector<sFloat*> float_stack;
   std::vector<sDouble*> double_stack;
   std::vector<sPtr*> ptr_stack;
   std::vector<sStr*> str_stack;
   std::vector<sType> type_stack2;
   std::vector<std::string> data_stack;

   std::string bf, key, marker;
   std::map<std::string, Serializable*> marker_to_ptr;
   if (l.root_obj != nullptr) {
      marker_to_ptr[l.depth] = l.root_obj;
      l.root_obj = nullptr;
      l.depth.clear();
   }
   {
      std::vector<sType> type_stack;
      bool is_key = true, is_marker = false, is_next_map = false;

      obj_stack.push_back(&l);
      type_stack.push_back(l.type());

      while (!s.eof()) {
         std::cout << "bf: " << bf << std::endl;
         std::cout << "key: " << key << '\n' << std::endl;
         int c = s.get();

         if (is_key) {
            if (c == ' ' || c == '\t') {
               continue;
            }
            if (c == '[') {
               is_marker = true;
               marker.clear();
               continue;
            }
            if (is_marker) {
               if (c == ']') {
                  is_marker = false;
                  marker_to_ptr[marker] = key.empty() ? obj_stack.back() : obj_stack.back()->n[key];
                  continue;
               }
               marker += c;
               continue;
            }
            if (c == ':') {
               is_key = false;

               if (s.get() == '\n') {
                  is_next_map = true;
               }
               if (type_stack.back() == sType::object_) {
                  sType type = obj_stack.back()->n[key]->type();
                  auto& obj = *obj_stack.back();
                  if (type == sType::double_) {
                     double_stack.push_back(reinterpret_cast<sDouble*>(obj.n[key]));
                  } else if (type == sType::float_) {
                     float_stack.push_back(reinterpret_cast<sFloat*>(obj.n[key]));
                  } else if (type == sType::int_) {
                     int_stack.push_back(reinterpret_cast<sInt*>(obj.n[key]));
                  } else if (type == sType::uint_) {
                     uint_stack.push_back(reinterpret_cast<sUint*>(obj.n[key]));
                  } else if (type == sType::str_) {
                     str_stack.push_back(reinterpret_cast<sStr*>(obj.n[key]));
                  } else if (type == sType::map_) {
                     map_stack.push_back(reinterpret_cast<sMap*>(obj.n[key]));
                  } else if (type == sType::vec_) {
                     vec_stack.push_back(reinterpret_cast<sVec*>(obj.n[key]));
                  } else if (type == sType::ptr_) {
                     ptr_stack.push_back(reinterpret_cast<sPtr*>(obj.n[key]));
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
                  is_next_map = true;
               }
               type_stack2.push_back(type_stack.back());
               type_stack.pop_back();
               data_stack.push_back(bf);
               bf.clear();
               is_key = type_stack.back() == sType::object_;
            } else {
               bf.push_back(c);
            }
         }
      }
   }
   while (!type_stack2.empty()) {
      sType type = type_stack2.back();
      type_stack2.pop_back();
      bf = data_stack.back();
      data_stack.pop_back();

      if (type == sType::double_) {
         double_stack.back()->n = std::stod(bf);
         double_stack.pop_back();
      } else if (type == sType::float_) {
         float_stack.back()->n = std::stof(bf);
         float_stack.pop_back();
      } else if (type == sType::int_) {
         int_stack.back()->n = std::stoul(bf);
         int_stack.pop_back();
      } else if (type == sType::uint_) {
         uint_stack.back()->n = std::stoi(bf);
         uint_stack.pop_back();
      } else if (type == sType::str_) {
         str_stack.back()->n = bf;
         str_stack.pop_back();
      } else if (type == sType::ptr_) {
         if (bf == "[]") {
            ptr_stack.back()->n = nullptr;
         } else {
            (ptr_stack.back()->n = marker_to_ptr[bf])->marked = true;
         }
         ptr_stack.pop_back();
      }
   }
   return s;
}
#undef INCASE


template<typename T> sVec* ___vec_to_sVec(void* ths, std::vector<T>& v)
{
   sVec* sv;
   if (!Serializable::sVec_pool.count(ths)) Serializable::sVec_pool[ths] = {};
   Serializable::sVec_pool[ths].push_back(std::shared_ptr<sVec>(sv = new sVec()));
   for (auto& it : v) sv->push_back(reinterpret_cast<Serializable*>(&it));
   return sv;
}
#define sFields sObj _fields_ = {{
#define sFields_of(Name)                                                           \
   sObj _fields_;                                                                  \
                                                                                   \
   ~ ## Name() { rm_sVecs_from_pool(); }                                           \
                                                                                   \
   void rm_sVecs_from_pool()                                                       \
   {                                                                               \
      if(Serializable::sVec_pool.count(this)) Serializable::sVec_pool.erase(this); \
   }                                                                               \
                                                                                   \
   void update_fields() {                                                          \
      rm_sVecs_from_pool();                                                        \
      _fields_ = {{

#define ___(Name) {#Name, &(this-> Name)},
#define ___vref(Name) {#Name, ___vec_to_sVec(this, this-> Name)},
#define ____sEnd                                               \
	std::ostream& operator >> (std::ostream &s)                 \
   {                                                           \
      if (marked) {                                            \
         s << depth << '[' << this << "] : \n";                \
         depth_push();                                         \
      }                                                        \
      s << this->_fields_;                                     \
      depth.substr(0,depth.size()-3);                          \
      if(marked)depth_pop();                                   \
      return s;                                                \
   }                                                           \
                                                               \
	std::istream& operator << (std::istream &s)                 \
   {                                                           \
      {                                                        \
         std::string bf;                                       \
         std::getline(s,bf);                                   \
         size_t opn=bf.find('[');                              \
         size_t cls=bf.find(']');                              \
         size_t coln=bf.find(':');                             \
         if(coln > opn && coln > cls && cls > opn) {           \
            root_obj = this;                                   \
         }                                                     \
      }                                                        \
      s << this->_fields_;                                     \
      return s;                                                \
   }                                                           \
                                                               \
	sObj* get_fields() override {return &this->_fields_;}       \
	sType type() {return sType::obj_ptr_;}                      \
	bool is(sType t) {return t == sType::obj_ptr_;}             \
   void custom_write(std::ostream& s) override {(*this) >> s;}
#define sEnd }}; ____sEnd
#define sEnd_ }}; } ____sEnd

//#define sEnd_ }};}

#endif // SERIALIZABLE_HPP
