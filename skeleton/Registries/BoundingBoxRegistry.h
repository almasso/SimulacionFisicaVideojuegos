#pragma once
#include <unordered_map>
#include <string>
#include "../Utils/BoundingBox.h"
class BoundingBoxRegistry : public std::unordered_map<std::string, BoundingBox*> {
public:
	inline void addRegistry(std::string name, BoundingBox* bb) { this->insert({ name, bb }); }
	inline void deleteRegister(std::string name) { this->erase(name); }
};

