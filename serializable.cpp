#include "serializable.hpp"
std::string Serializable::depth = {};
std::map<void*, std::vector<std::shared_ptr<sVec>>> Serializable::sVec_pool = {};
Serializable* Serializable::root_obj = { nullptr };
//std::set<Serializable*> Serializable::ptrs;
//std::set<Serializable*> Serializable::shown;
