#include "serializable.hpp"
std::string Serializable::depth{ "" };
Serializable* Serializable::root_obj{ nullptr };
//std::set<Serializable*> Serializable::ptrs;
//std::set<Serializable*> Serializable::shown;
