#pragma once
#include "schema.h"
#include "mesh.h"
#include <map>
#include <vector>

class ButterflySchema : public Schema {
private:

	float _a, _b, _c;

	std::vector<int> EM;

	inline void computeNewEdgeVertices(Mesh &mesh) {
		// Initialize all to -1	to indicate half-edge is not mapped yet
		EM.resize(mesh.HalfEdgeArray.size(), -1);

		for (int i = 0; i < mesh.HalfEdgeArray.size(); i++) {
			if (EM[i] != -1)
				continue;

			auto p1_2 = mesh.HalfEdgeArray[i];
			auto p2_3 = mesh.HalfEdgeArray[p1_2.next];
			auto p3_1 = mesh.HalfEdgeArray[p1_2.prev];
			auto p1_4 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[p1_2.pairHalfEdgeIndex].next];
			auto p4_2 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[p1_2.pairHalfEdgeIndex].prev];
			auto p5_4 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[p1_4.pairHalfEdgeIndex].prev];
			auto p6_2 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[p4_2.pairHalfEdgeIndex].prev];
			auto p7_3 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[p2_3.pairHalfEdgeIndex].prev];
			auto p8_1 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[p3_1.pairHalfEdgeIndex].prev];

			auto p1 = mesh.VerticesArray[p1_2.vIndex];
			auto p2 = mesh.VerticesArray[p2_3.vIndex];
			auto p3 = mesh.VerticesArray[p3_1.vIndex];
			auto p4 = mesh.VerticesArray[p4_2.vIndex];
			auto p5 = mesh.VerticesArray[p5_4.vIndex];
			auto p6 = mesh.VerticesArray[p6_2.vIndex];
			auto p7 = mesh.VerticesArray[p7_3.vIndex];
			auto p8 = mesh.VerticesArray[p8_1.vIndex];

			float w = 1.f / _c;
			MeshVertex q = (p1 + p2) * _a + (p3 + p4) * (_b) + (p5 + p6 + p7 + p8) * -_c;

			mesh.VerticesArray.push_back(q);
			EM[i] = mesh.VerticesArray.size() - 1;
			EM[mesh.HalfEdgeArray[i].pairHalfEdgeIndex] = mesh.VerticesArray.size() - 1;
		}
	}

	inline void connectNewEdgeVertices(Mesh& mesh) {
		mesh.FacesArray.clear();

		int v[3];
		for (int edgeIndex = 0; edgeIndex < mesh.HalfEdgeArray.size(); edgeIndex += 3) {
			auto edge = mesh.HalfEdgeArray[edgeIndex];

			//Left
			v[0] = edge.vIndex;
			v[1] = EM[edgeIndex];
			v[2] = EM[edge.prev];
			auto f1 = MeshFace(v[0], v[1], v[2]);

			//Right
			v[0] = EM[edgeIndex];
			v[1] = mesh.HalfEdgeArray[edge.next].vIndex;
			v[2] = EM[edge.next];
			auto f2 = MeshFace(v[0], v[1], v[2]);

			//Up
			v[0] = mesh.HalfEdgeArray[edge.prev].vIndex;
			v[1] = EM[edge.prev];
			v[2] = EM[edge.next];
			auto f3 = MeshFace(v[0], v[1], v[2]);

			//Middle
			v[0] = EM[edge.prev];
			v[1] = EM[edgeIndex];
			v[2] = EM[edge.next];
			auto f4 = MeshFace(v[0], v[1], v[2]);

			mesh.FacesArray.push_back(f1);
			mesh.FacesArray.push_back(f2);
			mesh.FacesArray.push_back(f3);
			mesh.FacesArray.push_back(f4);
		}
	}

public:
	ButterflySchema() {
		_a = 1.0 / 2.0;
		_b = 1.0 / 8.0;
		_c = 1.0 / 16.0;
	}

	ButterflySchema(float a, float b, float c) {
		_a = a;
		_b = b;
		_c = c;
	}

	Mesh apply(Mesh mesh) {
		computeNewEdgeVertices(mesh);
		connectNewEdgeVertices(mesh);
		initHalfEdges(mesh);

		return mesh;
	}
};
