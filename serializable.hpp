#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP
#include <map>
#include <set>
#include <string>
#include <vector>
#include <fstream>

enum class sType {unknown_, object_, int_, uint_, float_, double_, str_, vec_, map_, ptr_};

struct Serializable {
	static std::string depth;
	static std::set<Serializable*> shown;
	static std::set<Serializable*> ptrs;
	virtual sType type(){return sType::unknown_;}
	virtual bool is(sType t){return true;}
	virtual struct sMap* getFields(){
		return nullptr;
	}
};

#define DEF_SERIALIZABLE(Name, Type, sType_) \
	struct s ## Name : Serializable { \
		Type n; \
		s ## Name(){} \
		s ## Name(Type i):n(i){} \
		sType type() override {return sType :: sType_;} \
		bool is(sType t) override {return t == sType::sType_;} \
	}
DEF_SERIALIZABLE(Int, int, int_);
DEF_SERIALIZABLE(Uint, unsigned, uint_);
DEF_SERIALIZABLE(Float, float, float_);
DEF_SERIALIZABLE(Double, double, double_);
DEF_SERIALIZABLE(Str, std::string, str_);
DEF_SERIALIZABLE(Ptr, Serializable*, ptr_);
DEF_SERIALIZABLE(Vec, std::vector<Serializable*>, vec_);
#define filed std::map<std::string, Serializable*>
DEF_SERIALIZABLE(Map, filed, map_);
#undef field
#undef DEF_SERIALIZABLE

static std::ostream& operator << (std::ostream& s, sInt& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sUint& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sFloat& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sDouble& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sStr& i) {s << i.n; return s;}
static std::ostream& operator << (std::ostream& s, sPtr& i) {s << i.n; if (!i.shown.count(i.n)) i.ptrs.insert(i.n); return s;}

static std::ostream& operator << (std::ostream& s, sVec& l);
static std::ostream& operator << (std::ostream& s, sMap& l);
#define INCASE(Name, sType_, val) case sType::sType_: s << *reinterpret_cast<s ## Name*>(val); s << '\n'; break;
#define CASES(val) \
	switch(val->type()) { \
		INCASE(Int, int_, val) INCASE(Uint, uint_, val) INCASE(Float, float_, val) \
		INCASE(Str, str_, val) INCASE(Vec, vec_, val) INCASE(Map, map_, val) \
		INCASE(Double, double_, val) \
		case sType::object_: l.depth += ' '; s << '\n' << *val->getFields(); l.depth.pop_back(); break; \
		default: s << '\n'; break; \
	}

static std::ostream& operator << (std::ostream& s, sVec& l) {
	for(auto& i : l.n) {
			s << l.depth;
			CASES(i)
	}
	return s;
}
static std::ostream& operator << (std::ostream& s, sMap& l) {
  for(auto& i : l.n) {
		s << l.depth << i.first << " : ";
    CASES(i.second)
  }
	return s;
}
#undef INCASE

#define sFields sMap _fields_ = {{
	#define __(Name) {#Name, &this-> Name},
#define sEnd }}; \
\
	std::ostream& operator << (std::ostream& s) {s << this->_fields_; return s;} \
	sMap* getFields() override {return &this->_fields_;} \
	sType type() {return sType::object_;} \
	bool is(sType t) {return t == sType::object_;}



#endif // SERIALIZABLE_HPP
