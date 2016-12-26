#include "stdafx.h"
#include "LandscapeSurface.h"
#include <algorithm>

namespace
{
	glm::ivec3 GetPixel(const SDLSurfacePtr & surface, unsigned x, unsigned y)
	{
		int bpp = surface->format->BytesPerPixel;
		//assert(bpp == 3);
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return {p[0], p[1], p[2]};
			else
				return{ p[2], p[1], p[0] };
	}

	glm::ivec2 GetIndex(glm::vec2 const & position, float scale)
	{
		const unsigned ci = int(std::roundf(std::roundf(position.x) / scale));
		const unsigned ri = int(std::roundf(std::roundf(position.y) / scale));
		return{ ci, ri };
	}

	unsigned GetSingleIndex(glm::ivec2 indices, glm::ivec2 size)
	{
		return indices.x * size.y + indices.y;
	}

	struct CLandscapeTesselator : SMeshDataP3NT2
	{
	public:
		const float MIN_SCALE = 1.f;
		glm::ivec2 size;

		void Tesselate(float scale)
		{
			assert(scale >= MIN_SCALE);
			p_heightmap = CFilesystemUtils::LoadFileImage("res/heightmap.png");
			p_normalmap = CFilesystemUtils::LoadFileImage("res/normalmap.png");
			assert(p_heightmap->w == p_normalmap->w);
			assert(p_heightmap->h == p_normalmap->h);
			size = { p_heightmap->w, p_heightmap->h };
			MakeVertexAttributes(scale);
			MakeTriangleStripIndicies();
		}

	private:
		SDLSurfacePtr p_heightmap;
		SDLSurfacePtr p_normalmap;

		void MakeVertexAttributes(float scale)
		{
			const unsigned rowCount = size.x;
			const unsigned columnCount = size.y;
			for (unsigned ci = 0; ci < columnCount; ++ci)
			{
			    const unsigned x = unsigned(ci);
			    for (unsigned ri = 0; ri < rowCount; ++ri)
			    {
			        const unsigned z = unsigned(ri);

					SVertexP3NT2 vertex;
					vertex.position = GetPosition(x, z, scale);
					vertex.normal = GetNormal(x, z);
					vertex.texCoord = { float(ri) / rowCount * scale * 10, float(ci) / columnCount * scale * 10 };

					vertices.push_back(vertex);
			    }
			}
		}

		void MakeTriangleStripIndicies()
		{
			const unsigned rowCount = size.x;
			const unsigned columnCount = size.y;
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

		glm::vec3 GetPosition(unsigned x, unsigned z, float scale)const
		{
			glm::ivec3 pixel = GetPixel(p_heightmap, x, z);
			const float y = float(pixel.r) / 255.f * 20.f;
			return{ x * scale, y * scale, z * scale };
		}

		glm::vec3 GetNormal(unsigned x, unsigned z)const
		{
			glm::ivec3 pixel = GetPixel(p_normalmap, x, z);
			const float nx = float(pixel.r) / 255.f;
			const float nz = float(pixel.g) / 255.f;
			const float ny = float(pixel.b) / 255.f;
			return{ nx, ny, nz };
		}
	};
}

CLandscapeSurface::CLandscapeSurface(float scale)
	: m_mesh(MeshType::TriangleStrip)
	, m_scale(scale)
{
	CLandscapeTesselator tesselator;
	tesselator.Tesselate(scale);
	m_surfaceData = tesselator.vertices;
	m_size = tesselator.size;
	m_mesh.Copy(tesselator);
}

void CLandscapeSurface::Draw(IRenderer3D &renderer) const
{
	glDisable(GL_CULL_FACE);
	m_mesh.Draw(renderer);
	glEnable(GL_CULL_FACE);
}

bool CLandscapeSurface::CheckCollision(const glm::vec3 & position) const
{
	const glm::ivec2 xRange = { int(std::floorf(position.x / m_scale)), int(std::ceilf(position.x / m_scale)) };
	const glm::ivec2 yRange = { int(std::floorf(position.z / m_scale)), int(std::ceilf(position.z / m_scale)) };

	const unsigned topLeftIndex     = GetSingleIndex({ xRange.x, yRange.x }, m_size);
	const unsigned bottomRightIndex = GetSingleIndex({ xRange.y, yRange.y }, m_size);
	const unsigned topRightIndex    = GetSingleIndex({ xRange.x, yRange.y }, m_size);
	const unsigned bottomLeftIndex  = GetSingleIndex({ xRange.y, yRange.x }, m_size);
	const float firstDiagonalHeight = (m_surfaceData[topLeftIndex].position.y + m_surfaceData[bottomRightIndex].position.y) / 2.f;
	const float secondDiagonalHeight = (m_surfaceData[topRightIndex].position.y + m_surfaceData[bottomLeftIndex].position.y) / 2.f;
	const float surfaceHeight = std::fmaxf(firstDiagonalHeight, secondDiagonalHeight);

	//const glm::ivec2 unscaledPosition = { int(std::floorf(position.x / m_scale)), int(std::floorf(position.z / m_scale)) };
	//const float surfaceHeight = m_surfaceData[GetSingleIndex(unscaledPosition, m_size)].position.y;

	return (position.y < surfaceHeight);
}
