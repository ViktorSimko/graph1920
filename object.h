#pragma once
#include <vector>
#include "helper.h"

class Object {
public:
	std::vector<Point3D> vertices;
	Color color;

	Object(std::vector<Point3D> vertices, Color color=Color(0, 0, 0)) {
		this->vertices = vertices;
		this->color = Color(color.r, color.g, color.b);
	}
};