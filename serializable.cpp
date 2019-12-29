#include "serializable.hpp"
std::string Serializable::depth{ "" };
std::set<Serializable*> Serializable::ptrs;
std::set<Serializable*> Serializable::shown;
