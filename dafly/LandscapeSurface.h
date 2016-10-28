#pragma once

#include "libutil.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

// Вершина с трёхмерной позицией и нормалью.
struct SVertexP3N
{
    glm::vec3 position;
    glm::vec3 normal;

    SVertexP3N() = default;
    SVertexP3N(const glm::vec3 &position)
        : position(position)
    {
    }
};

class CLandscapeSurface final : public ISceneObject
{
public:
    /// Инициализирует индексированную сетку треугольников
    /// @param rangeX - диапазон, где x - нижняя граница, y - верхняя граница
    /// @param rangeZ - диапазон, где x - нижняя граница, y - верхняя граница
    void Tesselate(const glm::vec2 &rangeX, const glm::vec2 &rangeZ, float step);

    // IBody interface.
    void Update(float) final {}
    void Draw() const final;

private:
    std::vector<SVertexP3N> m_vertices;
    std::vector<uint32_t> m_indicies;
};
