#include "stdafx.h"
#include "SkyFloor.h"
#include <algorithm>

namespace
{

	struct CSkyFloorTesselator : SMeshDataP3NT2
	{
	public:
		const float MIN_SCALE = 1.f;

		void Tesselate(float scale)
		{
			assert(scale >= MIN_SCALE);
			const unsigned columnCount = 100;
			const unsigned rowCount = 100;
			MakeVertexAttributes(columnCount, rowCount, scale);
			MakeTriangleStripIndicies(columnCount, rowCount);
		}

	private:
		void MakeVertexAttributes(unsigned columnCount, unsigned rowCount, float scale)
		{
			for (unsigned ci = 0; ci < columnCount; ++ci)
			{
				const unsigned x = unsigned(ci);
				for (unsigned ri = 0; ri < rowCount; ++ri)
				{
					const unsigned z = unsigned(ri);

					SVertexP3NT2 vertex;
					vertex.position = GetPosition(x, z, scale);
					vertex.normal = { 0.f, 0.f, 0.f };

					vertices.push_back(vertex);
				}
			}
		}

		void MakeTriangleStripIndicies(unsigned columnCount, unsigned rowCount)
		{
			indicies.clear();
			indicies.reserve((columnCount - 1) * rowCount * 2);

			for (unsigned ci = 0; ci < columnCount - 1; ++ci)
			{
				if (ci % 2 == 0)
				{
					for (unsigned ri = 0; ri < rowCount; ++ri)
					{
						unsigned index = ci * rowCount + ri;
						indicies.push_back(index + rowCount);
						indicies.push_back(index);
					}
				}
				else
				{
					for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
					{
						unsigned index = ci * rowCount + ri;
						indicies.push_back(index);
						indicies.push_back(index + rowCount);
					}
				}
			}
		}

		glm::vec3 GetPosition(unsigned x, unsigned z, float scale) const
		{
			return{ x * scale, -300.f , z * scale };
		}
	};
}

CSkyFloor::CSkyFloor(float scale)
	: m_mesh(MeshType::TriangleStrip)
{
	CSkyFloorTesselator tesselator;
	tesselator.Tesselate(scale);
	m_mesh.Copy(tesselator);
}

void CSkyFloor::Draw(IRenderer3D &renderer) const
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_mesh.Draw(renderer);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}
