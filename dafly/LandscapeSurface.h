#pragma once

#include "libutil.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class CLandscapeSurface final : public ICollidableObject
{
public:
	CLandscapeSurface(float scale);
	void Draw(IRenderer3D &renderer)const;
	bool CheckCollision(const glm::vec3 & position)const;

private:
	CMeshP3NT2 m_mesh;
	std::vector<SVertexP3NT2> m_surfaceData;
	float m_scale;
	glm::ivec2 m_size;
};
