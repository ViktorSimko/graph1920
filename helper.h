#pragma once

#include <cmath>

class Point2D {
public:
	double x;
	double y;

	Point2D() {
		x = 0;
		y = 0;
	}

	Point2D(double x, double y) {
		this->x = x;
		this->y = y;
	}
};

class Point3D {
public:
	double x;
	double y;
	double z;

	Point3D() {
		x = 0;
		y = 0;
		z = 0;
	}

	Point3D(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Vector {
public:
	double x, y, z;


	Vector() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Vector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void operator=(const Vector& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	double operator*(const Vector& v) {
		double result = x * v.x + y * v.y + z * v.z;

		return result;
	}

	Vector operator-(const Vector& v) {
		Vector result;

		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;
	}

	double norm() {
		//double norm = 0.0;

		//norm = sqrt((*this) * v);

		//return norm;
		return sqrt((*this) * (*this));
	}

	void makeNorm() {
		double norm_ = this->norm();

		x /= norm_;
		y /= norm_;
		z /= norm_;

	}

	void divide(double div) {
		x /= div;
		y /= div;
		z /= div;
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

	Vector(Point3D p) {
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}

	Point3D asPoint() {
		return Point3D(x, y, z);
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


std::vector<std::string> wordsInLine(std::string line, char separator=' ') {
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
