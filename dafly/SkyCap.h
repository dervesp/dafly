#pragma once

#include "libutil.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class CSkyCap final
{
public:
	CSkyCap(float scale);
	void Draw(IRenderer3D &renderer)const;

private:
	CMeshP3NT2 m_mesh;
};
