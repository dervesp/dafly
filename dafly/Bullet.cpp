#include "stdafx.h"
#include "Bullet.h"

namespace
{
	const float MAX_LIFETIME = 10.f;
	const float SPEED = 50.f;

	struct CSphereTesselator : SMeshDataP3NT2
	{
	public:
		static const unsigned MIN_PRECISION = 4;

		void Tesselate(unsigned slices, unsigned stacks)
		{
			assert((slices >= MIN_PRECISION) && (stacks >= MIN_PRECISION));
			MakeVertexAttributes(slices, stacks);
			MakeTriangleStripIndicies(slices, stacks);
		}

	private:
		void MakeVertexAttributes(unsigned columnCount, unsigned rowCount)
		{
			for (unsigned ci = 0; ci < columnCount; ++ci)
			{
				const float u = float(ci) / float(columnCount - 1);
				for (unsigned ri = 0; ri < rowCount; ++ri)
				{
					const float v = float(ri) / float(rowCount - 1);

					SVertexP3NT2 vertex;
					vertex.position = GetPositionOnSphere(u, v);
					vertex.normal = vertex.position;
					vertex.texCoord = { 1.f - u, v };

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

		glm::vec3 GetPositionOnSphere(float u, float v)const
		{
			const float radius = 1.f;
			const float latitude = float(M_PI) * (1.f - v); // 𝝅∙(𝟎.𝟓-𝒗)
			const float longitude = float(2.0 * M_PI) * u; // 𝟐𝝅∙𝒖
			const float latitudeRadius = radius * sinf(latitude);

			return{ cosf(longitude) * latitudeRadius,
				cosf(latitude) * radius,
				sinf(longitude) * latitudeRadius };
		}
	};
}


CBullet::CBullet()
	: m_position({0, 0, 0})
	, m_direction({0, 0, 0, 0})
	, m_mesh(MeshType::TriangleStrip)
{
	CSphereTesselator tesselator;
	tesselator.Tesselate(10, 10);
	m_mesh.Copy(tesselator);
}


CBullet::CBullet(const glm::vec3 & position, const glm::quat & direction)
	: m_position(position)
	, m_direction(direction)
	, m_mesh(MeshType::TriangleStrip)
{
	m_position += glm::vec3({ 0.f, -5.f, 0.f });
	m_position += glm::vec3({ 0.f, 0.f, -5 }) * m_direction;

	CSphereTesselator tesselator;
	tesselator.Tesselate(10, 10);
	m_mesh.Copy(tesselator);
}

bool  CBullet::IsAlive() const
{
	return m_lifetime < MAX_LIFETIME;
}

void CBullet::Update(float deltaSec)
{
	const float distance = deltaSec * SPEED;
	glm::vec3 translate = glm::vec3({ 0.f, 0.f, -distance }) * m_direction;
	m_position = m_position + translate;
	m_lifetime += deltaSec;
}

glm::vec3 CBullet::GetPosition() const
{
	return m_position;
}


glm::quat CBullet::GetDirection() const
{
	return m_direction;
}

void CBullet::Draw(IRenderer3D &renderer) const
{
	glDisable(GL_CULL_FACE);
	m_mesh.Draw(renderer);
	glEnable(GL_CULL_FACE);
}

