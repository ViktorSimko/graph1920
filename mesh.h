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

	MeshVertex() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
		//valid = false;
	}

	MeshVertex(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	MeshVertex(double x, double y, double z, int edgeIndex) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->halfEdgeIndex = edgeIndex;
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
	}

	bool operator==(const MeshVertex& v) {
		if (x == v.x && y == v.y && z == v.z) {
			return true;
		}
		return false;
	}
};


class MeshHalfEdge {
public:
	int pairHalfEdgeIndex; // A pár half-edge indexe vagy -1 ha szélsõ

	MeshHalfEdge() {
		pairHalfEdgeIndex = -1;
	}

	MeshHalfEdge(int pairIndex) {
		pairHalfEdgeIndex = pairIndex;
	}
};

class MeshFace {
public:
	int v[3]; // v0, v1, v2 indexek a Vertex Arrayre
	//MeshHalfEdge HalfEdgeArray[3];

	MeshFace() {
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}

	MeshFace(int v0, int v1, int v2) {
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
};


enum TYPE { A = 0, B = 1 };

class Mesh {
	int V; // # old vertices
	int F; // # old faces
	int H; // # of half-edges
	int E; // # of edges
	vector<int> copyEM;
	vector<MeshVertex> copyNV;
	vector<MeshFace> copyNF;
	vector<MeshHalfEdge> copyHA;

	double b(int n = 4) {
		return 1.0 / 64.0 * (40 - pow((3 + 2 * cos(2 * M_PI / (double)n)), 2));
	}

public:
	Color faceColor, pointColor;
	vector<MeshVertex> VerticesArray; // VA
	vector<MeshFace> FacesArray; // FA
	vector<MeshHalfEdge> HalfEdgeArray; // HA

	vector<Triangle> Triangles;
	vector<Point2D> UVArray;
	vector<Point3D> NormalsArray;

	// ei : The old half-edge index
	// type : 0=A or 1=B, denoting which part of the new half-edge is being matched
	int nIndex(int ei, TYPE type) {
		int offsets[] = { 3, 1, 6, 4, 0, 7 }; // Data table for index offsets - matches new half-edges

		int op = HalfEdgeArray[ei].pairHalfEdgeIndex;

		if (op == -1) {
			return -1; // Boundary edge
		}
		int bp = 12 * (op / 3); // New position of the split-edges for the face with pair op

									 // Return the matching new index
		return bp + offsets[2 * (op % 3) + type];
	}

	Mesh() {
		faceColor = LIGHT_CREAM_COLOR;
		pointColor = HEAVY_BROWN_COLOR;
		//VerticesArray.reserve();
		//FacesArray.reserve();
	}

	void beforeAlgorithmStarts() {
		V = VerticesArray.size();
		F = FacesArray.size();
		H = HalfEdgeArray.size();;
		E = H / 2;
	}

	void computeNewEdgeVertices() {
		vector<MeshVertex> NV(V + E); // The new vertices: # old vertices + # edges
		vector<int> EM(H); // Edge map -> store indices mapping half-edges to new vertex indices

		// Initialize all to -1	to indicate half-edge is not mapped yet
		for (int i = 0, n = EM.size(); i < n; i++) {
			EM[i] = -1;
		}

		//int plusf = 0; // 3f, 3f + 1 és 3f + 2 miatt
		// For each half-edge h if EM[h]=-1 then insert a vertex on the edge

		int start = VerticesArray.size();
		int offset = 0;

		cout << "VerticesArray: " << VerticesArray.size() << endl;
		cout << "FacesArray: " << FacesArray.size() << endl;
		cout << "HalfEdgeArray: " << HalfEdgeArray.size() << endl;
		cout << "EM.size(): " << EM.size() << endl;
		cout << "NV.size(): " << NV.size() << endl;

		for (int h = 0, n = HalfEdgeArray.size(); h < n; h++) {

			if (EM[h] == -1) {
				MeshVertex newVertex;

				// A half edgehez tartozó face beazonosítása
				int faceIndex = (h) / 3;

				// 3f megy v0->v1 3f + 1 megy v1->v2 3f + 2 megy v2->v0 sorrendben

				newVertex.x = (3.0 / 8.0) * VerticesArray[FacesArray[faceIndex].v[(h + 1) % 3]].x +
					(3.0 / 8.0) * VerticesArray[FacesArray[HalfEdgeArray[h].pairHalfEdgeIndex / 3].v[(HalfEdgeArray[h].pairHalfEdgeIndex + 1) % 3]].x
					+ (1.0 / 8.0) * VerticesArray[FacesArray[faceIndex].v[(h + 2) % 3]].x +
					(1.0 / 8.0) * VerticesArray[FacesArray[HalfEdgeArray[h].pairHalfEdgeIndex / 3].v[(HalfEdgeArray[h].pairHalfEdgeIndex + 2) % 3]].x;

				newVertex.y = (3.0 / 8.0) * VerticesArray[FacesArray[faceIndex].v[(h + 1) % 3]].y + (3.0 / 8.0) * VerticesArray[FacesArray[HalfEdgeArray[h].pairHalfEdgeIndex / 3].v[(HalfEdgeArray[h].pairHalfEdgeIndex + 1) % 3]].y
					+ (1.0 / 8.0) * VerticesArray[FacesArray[faceIndex].v[(h + 2) % 3]].y + (1.0 / 8.0) * VerticesArray[FacesArray[HalfEdgeArray[h].pairHalfEdgeIndex / 3].v[(HalfEdgeArray[h].pairHalfEdgeIndex + 2) % 3]].y;

				newVertex.z = (3.0 / 8.0) * VerticesArray[FacesArray[faceIndex].v[(h + 1) % 3]].z + (3.0 / 8.0) * VerticesArray[FacesArray[HalfEdgeArray[h].pairHalfEdgeIndex / 3].v[(HalfEdgeArray[h].pairHalfEdgeIndex + 1) % 3]].z
					+ (1.0 / 8.0) * VerticesArray[FacesArray[faceIndex].v[(h + 2) % 3]].z + (1.0 / 8.0) * VerticesArray[FacesArray[HalfEdgeArray[h].pairHalfEdgeIndex / 3].v[(HalfEdgeArray[h].pairHalfEdgeIndex + 2) % 3]].z;

				newVertex.halfEdgeIndex = -1;

				NV[start + offset] = newVertex;

				EM[h] = start + offset;
				EM[HalfEdgeArray[h].pairHalfEdgeIndex] = start + offset;

				offset++;
			}

		}

		//copyEM = EM;
		//copyNV = NV;
		// Copy data
		copyEM.clear();
		copyNV.clear();

		for (int i = 0; i < EM.size(); i++) {
			copyEM.push_back(EM[i]);
		}

		for (int i = 0; i < NV.size(); i++) {
			copyNV.push_back(NV[i]);
		}

	}

	void updateOriginalVertices() {

		for (int i = 0; i < VerticesArray.size(); i++) {
			Point3D sum;

			int n = 0;
			int start_edge = VerticesArray[i].halfEdgeIndex;
			int current_edge = start_edge;

			do {
				current_edge = HalfEdgeArray[current_edge].pairHalfEdgeIndex;

				sum.x += (VerticesArray[FacesArray[current_edge / 3].v[(current_edge + 1) % 3]].x);
				sum.y += (VerticesArray[FacesArray[current_edge / 3].v[(current_edge + 1) % 3]].y);
				sum.z += (VerticesArray[FacesArray[current_edge / 3].v[(current_edge + 1) % 3]].z);

				n++;

				current_edge = 3 * (current_edge / 3) + ((current_edge + 2) % 3);
			} while (current_edge != start_edge);

			double b = (1.0 / 64.0) * (40 - std::pow(3 + 2 * std::cos((2 * M_PI) / n), 2));

			Point3D vertex;
			vertex.x = VerticesArray[i].x * (1 - b) + sum.x * (b / n);
			vertex.y = VerticesArray[i].y * (1 - b) + sum.y * (b / n);
			vertex.z = VerticesArray[i].z * (1 - b) + sum.z * (b / n);

			copyNV[i] = vertex;
			copyNV[i].halfEdgeIndex = -1;
		}
	}

	// Each old face will become 4 new faces
	void splitFaces() {
		// Allocate an array NF for new faces of size 4F
		vector<MeshFace> NF(4 * F);

		for (int f = 0, n = F; f < n; f++) {
			int j[3];
			j[0] = copyEM[3 * f];
			j[1] = copyEM[3 * f + 1];
			j[2] = copyEM[3 * f + 2];

			NF[4 * f] = MeshFace(j[2], FacesArray[f].v[0], j[0]);
			NF[4 * f + 1] = MeshFace(j[0], FacesArray[f].v[1], j[1]);
			NF[4 * f + 2] = MeshFace(j[1], FacesArray[f].v[2], j[2]);
			NF[4 * f + 3] = MeshFace(j[2], j[0], j[1]);

			// half-edgek beállítása
			copyNV[NF[4 * f].v[1]].halfEdgeIndex = f * 12;
			copyNV[NF[4 * f + 1].v[1]].halfEdgeIndex = f * 12 + 3;
			copyNV[NF[4 * f + 2].v[1]].halfEdgeIndex = f * 12 + 6;

			copyNV[copyEM[f * 3]].halfEdgeIndex = f * 12 + 1;
			copyNV[copyEM[f * 3 + 1]].halfEdgeIndex = f * 12 + 4;
			copyNV[copyEM[f * 3 + 2]].halfEdgeIndex = f * 12 + 2;
		}

		//copyNF = NF;
		copyNF.clear();
		for (int i = 0; i < NF.size(); i++) {
			copyNF.push_back(NF[i]);
		}
	}

	void createNewHalfEdgeInfo() {
		vector<MeshHalfEdge> NE(12 * F); // New half-edge array, 3 per new face, each old face becomes 4 new faces, giving 12

		for (int f = 0; f < F; f++) {
			int b = 12 * f;

			NE[b] = MeshHalfEdge(nIndex((f * 3) + 2, B)); //e2
			NE[b + 1] = MeshHalfEdge(nIndex((f * 3) + 0, A)); // e0
			NE[b + 2] = MeshHalfEdge(b + 9);
			NE[b + 3] = MeshHalfEdge(nIndex((f * 3) + 0, B)); //e0
			NE[b + 4] = MeshHalfEdge(nIndex((f * 3) + 1, A)); //e0
			NE[b + 5] = MeshHalfEdge(b + 10);
			NE[b + 6] = MeshHalfEdge(nIndex((f * 3) + 1, B)); //e1
			NE[b + 7] = MeshHalfEdge(nIndex((f * 3) + 2, A)); //e2
			NE[b + 8] = MeshHalfEdge(b + 11);
			NE[b + 9] = MeshHalfEdge(b + 2);
			NE[b + 10] = MeshHalfEdge(b + 5);
			NE[b + 11] = MeshHalfEdge(b + 8);
		}

		//copyHA = NE;
		copyHA.clear();
		for (int i = 0; i < NE.size(); i++) {
			copyHA.push_back(NE[i]);
		}
	}

	void refreshData() {
		VerticesArray.clear();
		for (int i = 0; i < copyNV.size(); i++) {
			VerticesArray.push_back(copyNV[i]);
		}
		FacesArray.clear();
		for (int i = 0; i < copyNF.size(); i++) {
			FacesArray.push_back(copyNF[i]);
		}
		HalfEdgeArray.clear();
		for (int i = 0; i < copyHA.size(); i++) {
			HalfEdgeArray.push_back(copyHA[i]);
		}
		//VerticesArray = copyNV;
		//FacesArray = copyNF;
		//HalfEdgeArray = copyHA;
	}

	void execute() {
		beforeAlgorithmStarts();

		computeNewEdgeVertices();
		updateOriginalVertices();
		splitFaces();
		createNewHalfEdgeInfo();

		refreshData();
	}

};