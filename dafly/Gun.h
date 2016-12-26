#pragma once
#include <vector>
#include "libutil.h"
#include "Bullet.h"
#include "ProgramContext.h"

class CGun
{
public:
	CGun(const IActor & actor);

	void Draw(CProgramContext &context, IRenderer3D &renderer)const;
	void Update(float deltaSec);
	bool OnKeyDown(const SDL_KeyboardEvent &event);
	bool OnKeyUp(const SDL_KeyboardEvent &event);

private:
	const IActor & m_actor;
	std::set<unsigned> m_keysPressed;
	std::vector<std::unique_ptr<CBullet>> m_bullets;
};

