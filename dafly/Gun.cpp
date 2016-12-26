#include "stdafx.h"
#include "Gun.h"
#include "Bullet.h"
#include "libutil.h"

namespace
{
	bool ShouldTrackKeyPressed(const SDL_Keysym &key)
	{
		switch (key.sym)
		{
		case SDLK_f:
			return true;
		}
		return false;
	}

	bool GetBulletFired(std::set<unsigned> & keysPressed)
	{
		if (keysPressed.count(SDLK_f))
		{
			return true;
		}
		return false;
	}
}

CGun::CGun(const IActor & actor) : m_actor(actor)
{
}

bool CGun::OnKeyDown(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.insert(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

bool CGun::OnKeyUp(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.erase(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

void CGun::Update(float deltaSec)
{
	if (GetBulletFired(m_keysPressed))
	{
		m_bullets.push_back(std::make_unique<CBullet>(m_actor.GetPosition(), m_actor.GetDirection()));
		std::cout << "attack!" << std::endl;
	}

	for (auto & bullet : m_bullets)
	{
		//if (bullet->IsAlive())
		//{
			bullet->Update(deltaSec);
		//}
	}
}

void CGun::Draw(CProgramContext &context, IRenderer3D &renderer) const
{
	for (auto & bullet: m_bullets)
	{
		//if (bullet->IsAlive())
		//{
			CTransform3D transform;
			transform.m_position = bullet->GetPosition();
			context.SetModel(transform.ToMat4());
			bullet->Draw(renderer);
			context.SetModel(glm::mat4());
		//}
	}
}