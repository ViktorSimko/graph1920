#pragma once

using namespace std;

#include <iostream>
#include <vector>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "helper.h"
#include "triangle.h"

Color LIGHT_CREAM_COLOR = Color(0, 0, 0);
Color HEAVY_BROWN_COLOR = Color(0, 0, 0);

class MeshVertex {
public:
	double x, y, z;
	//bool valid;
	int halfEdgeIndex; // vh - index of a half-edge ending on this vertex
	int valence;
	std::vector<int> heIndices; //Half-edge indices starting on this vertex

	MeshVertex() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
		//valid = false;
		valence = 0;
		heIndices.clear();
	}

	MeshVertex(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
		valence = 0;
		heIndices.clear();
	}

	MeshVertex(double x, double y, double z, int edgeIndex) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->halfEdgeIndex = edgeIndex;
		valence = 0;
		heIndices.clear();
	}

	void operator=(const Point3D& point) {
		x = point.x;
		y = point.y;
		z = point.z;
	}

	void operator=(const MeshVertex& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		//valid = v.valid;
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
		if (x == v.x && y == v.y && z == v.z) {
			return true;
		}
		return false;
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


};