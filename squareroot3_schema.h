#pragma once
#include "schema.h"
#include "mesh.h"
#include <map>

class SquareRoot3 : public Schema {
private:
	std::vector<MeshFace> newFaces;
	std::vector<MeshVertex> relaxedVertices;

	void computeNewVertices(Mesh& mesh) {
		for (int faceIndex = 0; faceIndex < mesh.FacesArray.size(); ++faceIndex) {
			auto face = mesh.FacesArray[faceIndex];

			auto p1 = mesh.VerticesArray[face.v[0]];
			auto p2 = mesh.VerticesArray[face.v[1]];
			auto p3 = mesh.VerticesArray[face.v[2]];

			auto midpoint = (p1 + p2 + p3) / 3.0;
			mesh.VerticesArray.push_back(midpoint);

			for (int i = 0; i < 3; ++i) {
				MeshVertex P = mesh.VerticesArray[face.v[i]];
				int n = P.valence;
				float B = (4.f - 2 * std::cos((2 * M_PI) / n)) / (9.0);

				MeshVertex M;
				for (int j = 0; j < n; ++j) {
					int vIndex = mesh.HalfEdgeArray[mesh.HalfEdgeArray[P.heIndices[j]].next].vIndex;
					M += mesh.VerticesArray[vIndex];
				}
				M = M / n;

				MeshVertex S = P * (1.0 - B) + M * B;
				relaxedVertices[face.v[i]] = S;
			}
		}

		for (int i = 0; i < relaxedVertices.size(); ++i)
			mesh.VerticesArray[i] = relaxedVertices[i];

		createNewFaces(mesh);
	}

	void createNewFaces(Mesh& mesh) {
		std::vector<MeshFace> newFaces;
		for (int faceIndex = 0; faceIndex < mesh.FacesArray.size(); ++faceIndex) {
			int faceVertex = relaxedVertices.size() + faceIndex;
			MeshFace old = mesh.FacesArray[faceIndex];

			//Down
			MeshFace f1;
			f1.v[0] = old.v[0];
			f1.v[1] = old.v[1];
			f1.v[2] = faceVertex;

			//Right
			MeshFace f2;
			f2.v[0] = old.v[1];
			f2.v[1] = old.v[2];
			f2.v[2] = faceVertex;

			//Left
			MeshFace f3;
			f3.v[0] = old.v[2];
			f3.v[1] = old.v[0];
			f3.v[2] = faceVertex;

			newFaces.push_back(f1);
			newFaces.push_back(f2);
			newFaces.push_back(f3);
		}

		mesh.FacesArray = newFaces;
	}

	void flipEdges(Mesh& mesh) {
		std::vector<MeshFace> newFaces;
		std::vector<bool> faceIsNotSplit(mesh.FacesArray.size(), true);

		for (int faceIndex = 0; faceIndex < mesh.FacesArray.size() - 3; faceIndex += 3) {
			auto df = mesh.FacesArray[faceIndex];
			auto rf = mesh.FacesArray[faceIndex + 1];
			auto lf = mesh.FacesArray[faceIndex + 2];

			//dfp - pair of the face on down
			int dfp = mesh.HalfEdgeArray[mesh.HalfEdgeArray[df.HalfEdgeArray[0]].pairHalfEdgeIndex].faceIndex;
			if (faceIsNotSplit[faceIndex] && faceIsNotSplit[dfp]) {
				int dfv = mesh.HalfEdgeArray[mesh.HalfEdgeArray[mesh.HalfEdgeArray[df.HalfEdgeArray[0]].pairHalfEdgeIndex].prev].vIndex;

				//Left
				MeshFace f1;
				f1.v[0] = df.v[0];
				f1.v[1] = dfv;
				f1.v[2] = df.v[2];

				//Right
				MeshFace f2;
				f2.v[0] = df.v[1];
				f2.v[1] = df.v[2];
				f2.v[2] = dfv;

				faceIsNotSplit[faceIndex] = false;
				faceIsNotSplit[dfp] = false;

				newFaces.push_back(f1);
				newFaces.push_back(f2);
			}

			//rfp - pair of the face on right
			int rfp = mesh.HalfEdgeArray[mesh.HalfEdgeArray[rf.HalfEdgeArray[0]].pairHalfEdgeIndex].faceIndex;
			if (faceIsNotSplit[faceIndex + 1] && faceIsNotSplit[rfp]) {
				int rfv = mesh.HalfEdgeArray[mesh.HalfEdgeArray[mesh.HalfEdgeArray[rf.HalfEdgeArray[0]].pairHalfEdgeIndex].prev].vIndex;

				//Down
				MeshFace f1;
				f1.v[0] = rf.v[0];
				f1.v[1] = rfv;
				f1.v[2] = rf.v[2];

				//Up
				MeshFace f2;
				f2.v[0] = rf.v[1];
				f2.v[1] = rf.v[2];
				f2.v[2] = rfv;

				faceIsNotSplit[faceIndex + 1] = false;
				faceIsNotSplit[rfp] = false;

				newFaces.push_back(f1);
				newFaces.push_back(f2);
			}

			//lfp - pair of the face on left
			int lfp = mesh.HalfEdgeArray[mesh.HalfEdgeArray[lf.HalfEdgeArray[0]].pairHalfEdgeIndex].faceIndex;
			if (faceIsNotSplit[faceIndex + 2] && faceIsNotSplit[lfp]) {
				int lfv = mesh.HalfEdgeArray[mesh.HalfEdgeArray[mesh.HalfEdgeArray[lf.HalfEdgeArray[0]].pairHalfEdgeIndex].prev].vIndex;

				//Up
				MeshFace f1 = MeshFace();
				f1.v[0] = lf.v[0];
				f1.v[1] = lfv;
				f1.v[2] = lf.v[2];

				//Down
				MeshFace f2 = MeshFace();
				f2.v[0] = lf.v[1];
				f2.v[1] = lf.v[2];
				f2.v[2] = lfv;

				faceIsNotSplit[faceIndex + 2] = false;
				faceIsNotSplit[lfp] = false;

				newFaces.push_back(f1);
				newFaces.push_back(f2);
			}
		}

		mesh.FacesArray = newFaces;
	}

public:
	Mesh apply(Mesh mesh) {
		auto va_size = mesh.VerticesArray.size();
		relaxedVertices.resize(va_size);

		computeNewVertices(mesh);
		initHalfEdges(mesh);
		flipEdges(mesh);
		initHalfEdges(mesh);

		return mesh;
	}
};
