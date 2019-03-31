#pragma once

#define EPSILON 0.0001

class Point2D {
public:
	double x, y;

	Point2D() {
		x = y = 0;
	}

	Point2D(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

class Point3D {
public:
	double x, y, z;

	Point3D() {
		x = y = z = 0;
	}

	Point3D(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};

class Vector {
public:
	double x, y, z;

	Vector() {
		x = y = z = 0.0;
	}

	Vector(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector(const Point3D& p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	void operator=(const Vector& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector operator+(const Vector& v) {
		auto res = Vector(x + v.x, y + v.y, z + v.z);
		return res;
	}

	Vector operator-(const Vector& v) {
		auto res = Vector(x - v.x, y - v.y, z - v.z);
		return res;
	}

	Vector operator-() {
		auto res = Vector(-x, -y, -z);
		return res;
	}

	Vector operator*(double d) {
		auto res = Vector(x * d, y * d, z * d);
		return res;
	}

	Vector operator/(double d) {
		auto res = Vector(x / d, y / d, z / d);
		return res;
	}

	bool operator==(const Vector& v) {
		auto res = (-EPSILON < x - v.x && x - v.x < EPSILON)
			&& (-EPSILON < y - v.y && y - v.y < EPSILON)
			&& (-EPSILON < z - v.z && z - v.z < EPSILON);
		return  res;
	}

	double operator*(const Vector& v) {//WTF?
		auto res = x * v.x + y * v.y + z * v.z;
		return res;
	}

	void normalize() {
		double len = length();
		x /= len;
		y /= len;
		z /= len;
	}

	double length() {
		auto res = sqrtf(x * x + y * y + z * z);
		return res;
	}

	// c1 = a2b3 - a3b2
	// c2 = a3b1 - a1b3
	// c3 = a1b2 - a2b1	
	Vector crossProduct(const Vector& v) {
		Vector result;
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;

		return result;
	}

	Point3D toPoint3D() {
		auto res = Point3D(x, y, z);
		return res;
	}
};

class Color {
public:
	int r, g, b;

	Color() {
		r = g = b = 0;
	}

	Color(int _r, int _g, int _b) {
		r = _r;
		g = _g;
		b = _b;
	}
};

std::vector<std::string> wordsInLine(std::string line, char separator = ' ') {
	std::vector<std::string> words;

	std::string word = "";

	for (auto c : line)
	{
		if (c == separator)
		{
			if (word != "") {
				words.push_back(word);
			}
			word = "";
		}
		else
		{
			word = word + c;
		}
	}

	if (word != "") {
		words.push_back(word);
	}

	return words;
}