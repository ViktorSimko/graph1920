#pragma once
class Point2D {
public:
	int x;
	int y;

	Point2D(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class Point3D {
public:
	int x;
	int y;
	int z;

	Point3D(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Color {
public:
	int r;
	int g;
	int b;

	Color() {

	}

	Color(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
};
