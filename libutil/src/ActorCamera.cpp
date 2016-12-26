#include "libutil_private.h"
#include "ActorCamera.h"
#include "Transform3D.h"


CActorCamera::CActorCamera(const IActor & actor)
	: m_actor(actor)
{
}

void CActorCamera::Update(float deltaSec)
{
}

glm::mat4 CActorCamera::GetViewTransform() const
{
	CTransform3D transform;
	transform.m_position = m_actor.GetPosition() * -1.f;
	transform.m_orientation = m_actor.GetDirection() * -1.f;
	return transform.ToMat4ForCamera();
}
