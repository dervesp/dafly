#include "libutil_private.h"
#include "Camera.h"
#include "Transform3D.h"
#include <glm/gtx/rotate_vector.hpp>


namespace
{
const float ROTATION_SPEED_RADIANS = 0.5f;
const float LINEAR_MOVE_SPEED = 10.f;
const float MIN_DISTANCE = 1.5f;
const float MAX_DISTANCE = 100.f;
const float MIN_SPEED = 0.f;
const float MAX_SPEED = 30.f;

const glm::vec3 FORWARD = { 1.f, 0.f, 0.f };
const glm::vec3 BACKWARD = { -1.f, 0.f, 0.f };
const glm::vec3 LEFT = { 0.f, 0.f, 1.f };
const glm::vec3 RIGHT = { 0.f, 0.f, -1.f };
const glm::vec3 UP = { 0.f, 1.f, 0.f };
const glm::vec3 DOWN = { 0.f, -1.f, 0.f };

bool ShouldTrackKeyPressed(const SDL_Keysym &key)
{
    switch (key.sym)
    {
	case SDLK_UP:
	case SDLK_DOWN:
	case SDLK_w:
	case SDLK_a:
    case SDLK_s:
	case SDLK_d:
	case SDLK_q:
	case SDLK_e:
        return true;
    }
    return false;
}

float GetThrottleChangeSpeed(std::set<unsigned> & keysPressed)
{
	if (keysPressed.count(SDLK_UP))
	{
		return +LINEAR_MOVE_SPEED;
	}
	if (keysPressed.count(SDLK_DOWN))
	{
		return -LINEAR_MOVE_SPEED;
	}
	return 0;
}

float GetYawRotationSpeedRadians(std::set<unsigned> & keysPressed)
{
	if (keysPressed.count(SDLK_d))
	{
		return +ROTATION_SPEED_RADIANS;
	}
	if (keysPressed.count(SDLK_a))
	{
		return -ROTATION_SPEED_RADIANS;
	}
	return 0;
}

float GetPitchRotationSpeedRadians(std::set<unsigned> & keysPressed)
{
	if (keysPressed.count(SDLK_w))
	{
		return +ROTATION_SPEED_RADIANS;
	}
	if (keysPressed.count(SDLK_s))
	{
		return -ROTATION_SPEED_RADIANS;
	}
	return 0;
}

float GetRollRotationSpeedRadians(std::set<unsigned> & keysPressed)
{
	if (keysPressed.count(SDLK_e))
	{
		return +ROTATION_SPEED_RADIANS;
	}
	if (keysPressed.count(SDLK_q))
	{
		return -ROTATION_SPEED_RADIANS;
	}
	return 0;
}
}


CCamera::CCamera()
	: m_position({0.f, 10.f, 0.f})
	, m_direction(glm::quat_cast(glm::yawPitchRoll(0.f, 0.f, float(M_PI))))
{
}

void CCamera::Update(float deltaSec)
{
	const float yaw = deltaSec * GetYawRotationSpeedRadians(m_keysPressed);
	const float pitch = deltaSec * GetPitchRotationSpeedRadians(m_keysPressed);
	const float roll = deltaSec * GetRollRotationSpeedRadians(m_keysPressed);
	const glm::quat rotation = glm::quat_cast(glm::yawPitchRoll(yaw, pitch, roll));
	m_direction = rotation * m_direction;

	m_speed += deltaSec * GetThrottleChangeSpeed(m_keysPressed);
	m_speed = (m_speed > MAX_SPEED) ? MAX_SPEED : m_speed;
	m_speed = (m_speed < MIN_SPEED) ? MIN_SPEED : m_speed;
	
    const float distance = deltaSec * m_speed;
	glm::vec3 translate = glm::vec3({ 0.f, 0.f, distance }) * m_direction;
	m_position += translate;
}

bool CCamera::OnKeyDown(const SDL_KeyboardEvent &event)
{
    if (ShouldTrackKeyPressed(event.keysym))
    {
        m_keysPressed.insert(unsigned(event.keysym.sym));
        return true;
    }
    return false;
}

bool CCamera::OnKeyUp(const SDL_KeyboardEvent &event)
{
    if (ShouldTrackKeyPressed(event.keysym))
    {
        m_keysPressed.erase(unsigned(event.keysym.sym));
        return true;
    }
    return false;
}

glm::mat4 CCamera::GetViewTransform() const
{
	CTransform3D transform;
	transform.m_position = m_position;
	transform.m_orientation = m_direction;
    return transform.ToMat4ForCamera();
}
