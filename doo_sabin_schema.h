#pragma once

#include "schema.h"
#include "mesh.h"
#include "io.h"


class DooSabin : public Schema{
    private:
    
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

			MeshVertex edgePoint = ( edge1 + edge2) * 0.5;

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

		return (v0 + v1 + v2 + v3) * 0.25;
	}

	void computeNewVertexPoints() {
		
		for (int i = 0; i < mesh.VerticesArray.size(); ++i) {
			MeshVertex S = mesh.VerticesArray[i];
			int n = S.valence;
			std::cout<< n << std::endl;


			MeshVertex a;
            MeshVertex b;
            MeshVertex c;
            MeshVertex d;

			for (int j = 0; j < n; ++j) {
				int heIndex = S.heIndices[j];

				int f = mesh.HalfEdgeArray[heIndex].faceIndex;
				a = getFacePoint(f);

                b =mesh.VerticesArray[mesh.HalfEdgeArray[heIndex].vIndex];
				d = mesh.VerticesArray[mesh.HalfEdgeArray[mesh.HalfEdgeArray[heIndex].next].vIndex];

            	int f_point = mesh.FacesArray[f].v[0]; 
				c = mesh.VerticesArray[f_point];
                MeshVertex newVertex = (a + b + c + d) / 4;
			    newMesh.VerticesArray.push_back(newVertex);
                //v[heIndex] = newMesh.VerticesArray.size() - 1;
				
			}

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
				v[3] = EM[mesh.HalfEdgeArray[heIndex].next];

				MeshFace face = MeshFace(v[0], v[1], v[2], v[3]);
				newMesh.FacesArray.push_back(face);
			}

		}
	}

    public:
        Mesh apply(Mesh _mesh) {
            mesh = _mesh;
            newMesh = Mesh();
            newMesh.quadratic = true;

            beforeAlgorithmStarts();
            initHalfEdges(newMesh);
            computeNewFacePoints();
            computeNewEdgeVertices();
            computeNewVertexPoints();

            return newMesh;
        }
};