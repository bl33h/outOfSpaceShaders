#pragma once
#include "colors.h"
#include <vector>
#include <glm/glm.hpp>
#include "framebuffer.h"
#include <glm/glm.hpp>

extern glm::vec3 L;

std::pair<float, float> barycentricCoordinates(const glm::ivec2& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c);
std::vector<Fragment> line(const glm::vec3& v1, const glm::vec3& v2);
