#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>

#include "helper.h"
#include "mesh.h"
#include "third_party_libs/easy_as_ply/PlyReader.h"

enum MeshType {
	OBJ,
	PLY
};

int getHalfEdgeIndex(int vertex0, int vertex1, std::vector<std::pair<int, int>> indices) {

	for (int i = 0; i < indices.size(); ++i) {
		if (indices[i].first == vertex0 && indices[i].second == vertex1) {
			return i;
		}
	}
	return -1;
}

void initHalfEdges(Mesh& mesh, std::vector<std::pair<int, int>> heIndices) {
	//Find pairs of the half edges
	mesh.HalfEdgeArray.resize(heIndices.size());
	for (int i = 0; i < mesh.FacesArray.size(); ++i) {
		int v[3] = { mesh.FacesArray[i].v[0], mesh.FacesArray[i].v[1], mesh.FacesArray[i].v[2] };
		int i0 = getHalfEdgeIndex(v[2], v[0], heIndices);
		int i1 = getHalfEdgeIndex(v[0], v[1], heIndices);
		int i2 = getHalfEdgeIndex(v[1], v[2], heIndices);

		mesh.HalfEdgeArray[i0] = MeshHalfEdge(getHalfEdgeIndex(v[0], v[2], heIndices));
		mesh.HalfEdgeArray[i1] = MeshHalfEdge(getHalfEdgeIndex(v[1], v[0], heIndices));
		mesh.HalfEdgeArray[i2] = MeshHalfEdge(getHalfEdgeIndex(v[2], v[1], heIndices));
	}
}

/*
	http://www.opengl-tutorial.org/hu/beginners-tutorials/tutorial-7-model-loading/
*/
Mesh readMeshObj(std::string path) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	
	std::vector<std::pair<int, int>> heIndices;

	Mesh mesh = Mesh();

	std::ifstream file;
	file.open(path);

	if (!file.is_open()) {
		throw std::invalid_argument("Impossible to open the file!");
	}

	std::string line;

	while (std::getline(file, line)) {

		std::vector<std::string> content = wordsInLine(line);

		if (line[0] == 'v' && line[1] == ' ') {
			MeshVertex vertex = MeshVertex(std::stod(content[1]), std::stod(content[2]), std::stod(content[3]));
			mesh.VerticesArray.push_back(vertex);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			Point2D uv = Point2D(std::stod(content[1]), std::stod(content[2]));
			mesh.UVArray.push_back(uv);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			Point3D normal = Point3D(std::stod(content[1]), std::stod(content[2]), std::stod(content[3]));
			mesh.NormalsArray.push_back(normal);
		}
		else if (line[0] == 'f') {
			std::string vertex1, vertex2, vertex3;
			int vertexIndex[3], uvIndex[3], normalIndex[3];

			if (content.size() < 4) {
				throw std::invalid_argument("File can't be read by our simple parser : ( Try exporting with other options\n");
			}

			std::string firstPart = content[1];
			std::string secondPart = content[2];
			std::string thirdPart = content[3];

			std::vector<std::string> firstPartNumbers = wordsInLine(firstPart, '/');
			std::vector<std::string> secondPartNumbers = wordsInLine(secondPart, '/');
			std::vector<std::string> thirdPartNumbers = wordsInLine(thirdPart, '/');

			vertexIndex[0] = std::stoi(firstPartNumbers[0]) - 1;
			if (firstPartNumbers.size() > 1)
				uvIndex[0] = std::stoi(firstPartNumbers[1]) - 1;
			if (firstPartNumbers.size() > 2)
				normalIndex[0] = std::stoi(firstPartNumbers[2]) - 1;

			vertexIndex[1] = std::stoi(secondPartNumbers[0]) - 1;
			if (secondPartNumbers.size() > 1)
				uvIndex[1] = std::stoi(secondPartNumbers[1]) - 1;
			if (secondPartNumbers.size() > 2)
				normalIndex[1] = std::stoi(secondPartNumbers[2]) - 1;

			vertexIndex[2] = std::stoi(thirdPartNumbers[0]) - 1;
			if (thirdPartNumbers.size() > 1)
				uvIndex[2] = std::stoi(thirdPartNumbers[1]) - 1;
			if (thirdPartNumbers.size() > 2)
				normalIndex[2] = std::stoi(thirdPartNumbers[2]) - 1;

			heIndices.push_back(std::make_pair(vertexIndex[0], vertexIndex[1]));
			heIndices.push_back(std::make_pair(vertexIndex[1], vertexIndex[2]));
			heIndices.push_back(std::make_pair(vertexIndex[2], vertexIndex[0]));

			mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));
			mesh.VerticesArray[vertexIndex[0]].halfEdgeIndex = getHalfEdgeIndex(vertexIndex[2], vertexIndex[0], heIndices);
			mesh.VerticesArray[vertexIndex[1]].halfEdgeIndex = getHalfEdgeIndex(vertexIndex[0], vertexIndex[1], heIndices);
			mesh.VerticesArray[vertexIndex[2]].halfEdgeIndex = getHalfEdgeIndex(vertexIndex[1], vertexIndex[2], heIndices);

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	file.close();

	initHalfEdges(mesh, heIndices);

	return mesh;
}

Mesh readMeshPly(std::string path) {
	Mesh mesh = Mesh();
	EasyAsPLY::PlyReader reader;
	std::vector<std::pair<int, int>> heIndices;

	reader.Open();

	if (reader.Read(path)){
		// Get all the vertices
		int vertexIndex = reader.GetFormat()->GetElementIndex("vertex");
		std::vector<EasyAsPLY::PlyElement*>* vertices = reader.GetData()->GetElements(vertexIndex);

		int xIndex = reader.GetFormat()->GetElement("vertex")->GetPropertyIndex("x");
		int yIndex = reader.GetFormat()->GetElement("vertex")->GetPropertyIndex("y");
		int zIndex = reader.GetFormat()->GetElement("vertex")->GetPropertyIndex("z");

		for (int i = 0, n = vertices->size(); i < n; i++) {
			EasyAsPLY::PlyElement* vertex = vertices->at(i);

			mesh.VerticesArray.push_back(MeshVertex(vertex->GetProperty(xIndex)->GetValue<double>(), vertex->GetProperty(yIndex)->GetValue<double>(), vertex->GetProperty(zIndex)->GetValue<double>()));
		}

		int faceIndex = reader.GetFormat()->GetElementIndex("face");
		std::vector<EasyAsPLY::PlyElement*>* faces = reader.GetData()->GetElements(faceIndex);

		int idx = reader.GetFormat()->GetElement("face")->GetPropertyIndex("vertex_indices");

		for (int i = 0, n = faces->size(); i < n; i++) {
			EasyAsPLY::PlyElement* face = faces->at(i);
			std::string face_content = faces->at(i)->GetProperty(idx)->GetValue<std::string>();
			
			std::vector<std::string> content = wordsInLine(face_content);

			int vertexIndex[3] = { std::stoi(content[0]), std::stoi(content[1]), std::stoi(content[2])};

			mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));

			heIndices.push_back(std::make_pair(vertexIndex[0], vertexIndex[1]));
			heIndices.push_back(std::make_pair(vertexIndex[1], vertexIndex[2]));
			heIndices.push_back(std::make_pair(vertexIndex[2], vertexIndex[0]));

			mesh.VerticesArray[vertexIndex[0]].halfEdgeIndex = getHalfEdgeIndex(vertexIndex[2], vertexIndex[0], heIndices);
			mesh.VerticesArray[vertexIndex[1]].halfEdgeIndex = getHalfEdgeIndex(vertexIndex[0], vertexIndex[1], heIndices);
			mesh.VerticesArray[vertexIndex[2]].halfEdgeIndex = getHalfEdgeIndex(vertexIndex[1], vertexIndex[2], heIndices);
		}

	}
	else {
		throw std::invalid_argument("Impossible to open the file!");
	}

	reader.Close();

	initHalfEdges(mesh, heIndices);

	return mesh;
}

Mesh readMesh(std::string path, MeshType type) {
	switch (type)
	{
	case OBJ:
		return readMeshObj(path);
		break;
	case PLY:
		return readMeshPly(path);
		break;
	default:
		throw std::invalid_argument("Unknown type");
	}
}