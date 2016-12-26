#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <boost/noncopyable.hpp>
#include "IActor.h"

class CActorCamera : private boost::noncopyable
{
public:
    explicit CActorCamera(IActor const & actor);

	void Update(float deltaSec);
    glm::mat4 GetViewTransform() const;

private:
	IActor const & m_actor;
};
