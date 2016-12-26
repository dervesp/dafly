#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class IActor
{
public:
	virtual ~IActor() = default;
	
	virtual glm::quat GetDirection() const = 0;
	virtual glm::vec3 GetPosition() const = 0;
	virtual void Update(float deltaSec) = 0;
	virtual bool IsAlive() const = 0;
};
