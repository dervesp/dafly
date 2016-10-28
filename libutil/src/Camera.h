#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <SDL2/SDL_events.h>
#include <boost/noncopyable.hpp>
#include <set>

class CCamera : private boost::noncopyable
{
public:
    explicit CCamera();

    void Update(float deltaSec);
    bool OnKeyDown(const SDL_KeyboardEvent &event);
    bool OnKeyUp(const SDL_KeyboardEvent &event);

    glm::mat4 GetViewTransform() const;

private:
	glm::vec3 m_position;
	glm::quat m_direction;
	float m_speed;
    std::set<unsigned> m_keysPressed;
};
