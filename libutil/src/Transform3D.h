#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

class CTransform3D
{
public:
	CTransform3D();

	glm::mat4 ToMat4() const;
	glm::mat4 ToMat4ForCamera() const;

	glm::vec3 m_sizeScale;
	glm::quat m_orientation;
	glm::vec3 m_position;
};

