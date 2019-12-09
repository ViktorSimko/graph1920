#pragma once
#include "schema.h"
#include "mesh.h"

class CatmullClark : public Schema {
private:
	float _a, _b, _c, _d;

	Mesh mesh;
	Mesh newMesh;

	int V; // # old vertices
	int F; // # old faces
	int H; // # of half-edges
	int E; // # of edges

	std::vector<int> EM;

	void beforeAlgorithmStarts() {
		V = mesh.VerticesArray.size();
		F = mesh.FacesArray.size();
		H = mesh.HalfEdgeArray.size();;
		E = H / 2;
		EM.resize(H);
	}

	void computeNewFacePoints() {
		for (int faceIndex = 0; faceIndex < mesh.FacesArray.size(); faceIndex++) {
			MeshVertex vertex;
			auto facePoint = getFacePoint(faceIndex);
			vertex = facePoint;
			newMesh.VerticesArray.push_back(vertex);
		}
	}

	void computeNewEdgeVertices() {
		//Init
		for (int i = 0; i < H; i++)
			EM[i] = -1;

		for (int halfEdgeIndex = 0; halfEdgeIndex < mesh.HalfEdgeArray.size(); halfEdgeIndex++) {
			if (EM[halfEdgeIndex] != -1)
				continue;

			int faceIndex = mesh.HalfEdgeArray[halfEdgeIndex].faceIndex;

			int v1 = mesh.HalfEdgeArray[halfEdgeIndex].vIndex;
			int v2 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[halfEdgeIndex].next].vIndex;

			auto edge1 = mesh.VerticesArray[v1];
			auto edge2 = mesh.VerticesArray[v2];

			auto edgePair = mesh.HalfEdgeArray[halfEdgeIndex].pairHalfEdgeIndex;
			auto face1 = getFacePoint(faceIndex);
			auto face2 = getFacePoint(mesh.HalfEdgeArray[edgePair].faceIndex);

			MeshVertex edgePoint = (face1 * _a + face2 * _b + edge1 * _c + edge2 * _d);

			newMesh.VerticesArray.push_back(edgePoint);
			EM[halfEdgeIndex] = newMesh.VerticesArray.size() - 1;
			EM[mesh.HalfEdgeArray[halfEdgeIndex].pairHalfEdgeIndex] = newMesh.VerticesArray.size() - 1;
		}
	}

	MeshVertex getFacePoint(int faceIndex) {
		auto v0 = mesh.VerticesArray[mesh.FacesArray[faceIndex].v[0]];
		auto v1 = mesh.VerticesArray[mesh.FacesArray[faceIndex].v[1]];
		auto v2 = mesh.VerticesArray[mesh.FacesArray[faceIndex].v[2]];
		auto v3 = mesh.VerticesArray[mesh.FacesArray[faceIndex].v[3]];

		return (v0 * _a + v1 * _b + v2 * _c + v3 * _d);
	}

	void computeNewVertexPoints() {

		for (int i = 0; i < mesh.VerticesArray.size(); ++i) {
			MeshVertex S = mesh.VerticesArray[i];
			int n = S.valence;

			MeshVertex Q;
			MeshVertex R;
			for (int j = 0; j < n; ++j) {
				int heIndex = S.heIndices[j];

				int f = mesh.HalfEdgeArray[heIndex].faceIndex;
				Q += getFacePoint(f);

				int v1 = mesh.HalfEdgeArray[heIndex].vIndex;
				int v2 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[heIndex].next].vIndex;
				R += (mesh.VerticesArray[v1] + mesh.VerticesArray[v2]) / 2.0;
			}

			Q = Q / n;
			R = R / n;

			MeshVertex newVertex = (Q + R * 2 + S * (n - 3)) / n;
			newMesh.VerticesArray.push_back(newVertex);

			//Creating the new face
			int v[4];
			for (int j = 0; j < n; ++j) {
				int heIndex = S.heIndices[j];
				int f = mesh.HalfEdgeArray[heIndex].faceIndex;

				int v1 = mesh.HalfEdgeArray[heIndex].vIndex;
				int v2 = mesh.HalfEdgeArray[mesh.HalfEdgeArray[heIndex].next].vIndex;

				v[0] = newMesh.VerticesArray.size() - 1;
				v[1] = EM[heIndex];
				v[2] = f;
				v[3] = EM[mesh.HalfEdgeArray[heIndex].prev];

				MeshFace face = MeshFace(v[0], v[1], v[2], v[3]);
				newMesh.FacesArray.push_back(face);
			}

		}
	}

public:
	CatmullClark() {
		this->_a = 0.25;
		this->_b = 0.25;
		this->_c = 0.25;
		this->_d = 0.25;
	}

	CatmullClark(float a, float b, float c, float d) {
		this->_a = a;
		this->_b = b;
		this->_c = c;
		this->_d = d;
	}
	
	Mesh apply(Mesh _mesh) {
		mesh = _mesh;
		newMesh = Mesh();
		newMesh.quadratic = true;

		beforeAlgorithmStarts();
		computeNewFacePoints();
		computeNewEdgeVertices();
		computeNewVertexPoints();
		initHalfEdges(newMesh);

		return newMesh;
	}
};
