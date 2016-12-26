#pragma once
#include "libutil.h"

class CBullet : public IActor
{
public:
	CBullet();
	CBullet(const glm::vec3 & position, const glm::quat & direction);

	void Draw(IRenderer3D &renderer)const;
	void Update(float deltaSec) override;
	bool IsAlive() const override;
	glm::quat GetDirection() const override;
	glm::vec3 GetPosition() const override;

private:
	float m_lifetime = 0;
	glm::vec3 m_position;
	glm::quat m_direction;

	CMeshP3NT2 m_mesh;
};

