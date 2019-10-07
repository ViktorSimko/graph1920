#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "helper.h"
#include "triangle.h"

inline Color LIGHT_CREAM_COLOR = Color(0, 0, 0);
inline Color HEAVY_BROWN_COLOR = Color(0, 0, 0);

using namespace std;

class MeshVertex {
public:
	float x, y, z;
	int halfEdgeIndex;
	int valence;
	std::vector<int> heIndices;

	MeshVertex() {
		x = y = z = 0.f;
		halfEdgeIndex = -1;
		valence = 0;
	}

	MeshVertex(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
		halfEdgeIndex = -1;
		valence = 0;
	}

	MeshVertex(float _x, float _y, float _z, int _halfEdgeIndex) {
		x = _x;
		y = _y;
		z = _z;
		halfEdgeIndex = _halfEdgeIndex;
		valence = 0;
	}

	void operator=(const Point3D& point) {
		x = point.x;
		y = point.y;
		z = point.z;
	}

	void operator=(const Vector& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	void operator=(const MeshVertex& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		halfEdgeIndex = v.halfEdgeIndex;
		heIndices = v.heIndices;
		valence = v.valence;
	}

	void operator+=(const MeshVertex& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	bool operator==(const MeshVertex& v) {
		auto res = x == v.x && y == v.y && z == v.z;
		return res;
	}

	MeshVertex operator+(const MeshVertex& v) {
		return MeshVertex(x + v.x, y + v.y, z + v.z, halfEdgeIndex);
	}

	MeshVertex operator*(const float delta) {
		return MeshVertex(x * delta, y * delta, z * delta, halfEdgeIndex);
	}

	MeshVertex operator/(const float delta) {
		return MeshVertex(x / delta, y / delta, z / delta, halfEdgeIndex);
	}

	Vector toVector() {
		auto res = Vector(x, y, z);
		return res;
	}
};


class MeshHalfEdge {
public:
	int pairHalfEdgeIndex; // A p�r half-edge indexe vagy -1 ha sz�ls�
	int faceIndex;
	int vIndex; //The index of vertex at the starting of the edge
	int next;
	int prev;

	MeshHalfEdge() {
		pairHalfEdgeIndex = -1;
	}

	MeshHalfEdge(int pairIndex) {
		pairHalfEdgeIndex = pairIndex;
	}

	bool operator==(const MeshHalfEdge& v) {
		auto res = pairHalfEdgeIndex == v.pairHalfEdgeIndex;
		return res;
	}
};

class MeshFace {
public:
	int v[4]; // v0, v1, v2 indexek a Vertex Arrayre
	int HalfEdgeArray[4]; //0.indexu HE -> 0.indexu vertexbol indulo HE

	MeshFace() {
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		v[3] = 0;
	}

	MeshFace(int v0, int v1, int v2) {
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}

	MeshFace(int v0, int v1, int v2, int v3) {
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		v[3] = v3;
	}

};


enum TYPE { A = 0, B = 1 };

class Mesh {

public:
	bool quadratic;
	Color faceColor, pointColor;
	vector<MeshVertex> VerticesArray; // VA
	vector<MeshFace> FacesArray; // FA
	vector<MeshHalfEdge> HalfEdgeArray; // HA

	vector<Triangle> Triangles;
	vector<Point2D> UVArray;
	vector<Point3D> NormalsArray;

	Mesh() {
		quadratic = false;
		faceColor = LIGHT_CREAM_COLOR;
		pointColor = HEAVY_BROWN_COLOR;
		//VerticesArray.reserve();
		//FacesArray.reserve();
	}

	~Mesh() {}

	void addNoise() {
		srand(time(NULL));
		for (int i = 0; i < VerticesArray.size(); ++i) {
			float x = (rand() % 10 - 5) / 150.f;
			float y = (rand() % 10 - 5) / 150.f;
			float z = (rand() % 10 - 5) / 150.f;

			VerticesArray[i].x += x;
			VerticesArray[i].y += y;
			VerticesArray[i].z += z;
		}
	}

};