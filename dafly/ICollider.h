#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class ICollider
{
public:
	virtual bool CheckCollisions(const glm::vec3 & position) const = 0;
};
