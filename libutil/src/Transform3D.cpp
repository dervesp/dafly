#include "libutil_private.h"
#include "Transform3D.h"

CTransform3D::CTransform3D()
	: m_sizeScale(glm::vec3(1))
	, m_orientation(glm::angleAxis(0.f, glm::vec3(0.f, 1.f, 0.f)))
{
}

glm::mat4 CTransform3D::ToMat4() const
{
	const glm::mat4 scaleMatrix = glm::scale(glm::mat4(), m_sizeScale);
	const glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	const glm::mat4 translateMatrix = glm::translate(glm::mat4(), m_position);

	return  translateMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 CTransform3D::ToMat4ForCamera() const
{
	const glm::mat4 scaleMatrix = glm::scale(glm::mat4(), m_sizeScale);
	const glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	const glm::mat4 translateMatrix = glm::translate(glm::mat4(), m_position);

	return scaleMatrix * rotationMatrix * translateMatrix;
}
