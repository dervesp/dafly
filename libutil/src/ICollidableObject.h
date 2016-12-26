#pragma once
#include <glm/fwd.hpp>
#include <cstdlib>

class ICollidableObject
{
public:
	virtual ~ICollidableObject() = default;

	virtual bool CheckCollision(const glm::vec3 & position) const = 0;
};
