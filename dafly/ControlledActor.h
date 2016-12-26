#pragma once
#include "libutil.h"
#include "ICollider.h"

class CControlledActor : public IActor
{
public:
	CControlledActor(ICollider & collider, const glm::vec3 & position, const glm::vec3 & eulerAngles);
	bool IsAlive() const override;

	void Update(float deltaSec) override;
	bool OnKeyDown(const SDL_KeyboardEvent &event);
	bool OnKeyUp(const SDL_KeyboardEvent &event);

	glm::quat GetDirection() const override;
	glm::vec3 GetPosition() const override;

private:
	glm::vec3 m_position;
	glm::quat m_direction;
	float m_speed;
	float m_roll;
	ICollider & m_collider;

	std::set<unsigned> m_keysPressed;
};

