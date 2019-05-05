#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <set>
#include <map>

#include "helper.h"
#include "mesh.h"
#include "third_party_libs/easy_as_ply/PlyReader.h"

enum MeshType {
	OBJ,
	PLY,
	STL
};

inline void initHalfEdges(Mesh& mesh) {

	std::map<std::pair<int, int>, int> heIndices;

	if (mesh.quadratic) {
		for (int i = 0; i < mesh.FacesArray.size(); ++i) {
			int v[4] = { mesh.FacesArray[i].v[0], mesh.FacesArray[i].v[1], mesh.FacesArray[i].v[2], mesh.FacesArray[i].v[3] };
			heIndices[std::make_pair(v[0], v[1])] = heIndices.size();
			heIndices[std::make_pair(v[1], v[2])] = heIndices.size();
			heIndices[std::make_pair(v[2], v[3])] = heIndices.size();
			heIndices[std::make_pair(v[3], v[0])] = heIndices.size();
			mesh.VerticesArray[v[0]].valence = 0;
			mesh.VerticesArray[v[1]].valence = 0;
			mesh.VerticesArray[v[2]].valence = 0;
			mesh.VerticesArray[v[3]].valence = 0;
			mesh.VerticesArray[v[0]].heIndices.clear();
			mesh.VerticesArray[v[1]].heIndices.clear();
			mesh.VerticesArray[v[2]].heIndices.clear();
			mesh.VerticesArray[v[3]].heIndices.clear();
		}
		mesh.HalfEdgeArray.resize(heIndices.size());
		for (int i = 0; i < mesh.FacesArray.size(); ++i) {
			int v[4] = { mesh.FacesArray[i].v[0], mesh.FacesArray[i].v[1], mesh.FacesArray[i].v[2], mesh.FacesArray[i].v[3] };

			mesh.VerticesArray[v[0]].valence++;
			mesh.VerticesArray[v[1]].valence++;
			mesh.VerticesArray[v[2]].valence++;
			mesh.VerticesArray[v[3]].valence++;
			int i0 = heIndices[std::make_pair(v[3], v[0])];
			int i1 = heIndices[std::make_pair(v[0], v[1])];
			int i2 = heIndices[std::make_pair(v[1], v[2])];
			int i3 = heIndices[std::make_pair(v[2], v[3])];
			mesh.VerticesArray[v[0]].heIndices.push_back(i1);
			mesh.VerticesArray[v[1]].heIndices.push_back(i2);
			mesh.VerticesArray[v[2]].heIndices.push_back(i3);
			mesh.VerticesArray[v[3]].heIndices.push_back(i0);

			//Find pairs of the half edges
			mesh.HalfEdgeArray[i0] = MeshHalfEdge(heIndices[std::make_pair(v[0], v[3])]);
			mesh.HalfEdgeArray[i0].faceIndex = i;
			mesh.HalfEdgeArray[i1] = MeshHalfEdge(heIndices[std::make_pair(v[1], v[0])]);
			mesh.HalfEdgeArray[i1].faceIndex = i;
			mesh.HalfEdgeArray[i2] = MeshHalfEdge(heIndices[std::make_pair(v[2], v[1])]);
			mesh.HalfEdgeArray[i2].faceIndex = i;
			mesh.HalfEdgeArray[i3] = MeshHalfEdge(heIndices[std::make_pair(v[3], v[2])]);
			mesh.HalfEdgeArray[i3].faceIndex = i;

			//Find next half edges inside the face
			mesh.HalfEdgeArray[i0].next = heIndices[std::make_pair(v[0], v[1])];
			mesh.HalfEdgeArray[i1].next = heIndices[std::make_pair(v[1], v[2])];
			mesh.HalfEdgeArray[i2].next = heIndices[std::make_pair(v[2], v[3])];
			mesh.HalfEdgeArray[i3].next = heIndices[std::make_pair(v[3], v[0])];

			//Find previous half edges inside the face
			mesh.HalfEdgeArray[i0].prev = heIndices[std::make_pair(v[2], v[3])];
			mesh.HalfEdgeArray[i1].prev = heIndices[std::make_pair(v[3], v[0])];
			mesh.HalfEdgeArray[i2].prev = heIndices[std::make_pair(v[0], v[1])];
			mesh.HalfEdgeArray[i3].prev = heIndices[std::make_pair(v[1], v[2])];

			//Storing vertex indices at starting points
			mesh.HalfEdgeArray[i0].vIndex = v[3];
			mesh.HalfEdgeArray[i1].vIndex = v[0];
			mesh.HalfEdgeArray[i2].vIndex = v[1];
			mesh.HalfEdgeArray[i3].vIndex = v[2];

			//Storing half edge indices in faces too
			mesh.FacesArray[i].HalfEdgeArray[0] = heIndices[std::make_pair(v[0], v[1])];
			mesh.FacesArray[i].HalfEdgeArray[1] = heIndices[std::make_pair(v[1], v[2])];
			mesh.FacesArray[i].HalfEdgeArray[2] = heIndices[std::make_pair(v[2], v[3])];
			mesh.FacesArray[i].HalfEdgeArray[3] = heIndices[std::make_pair(v[3], v[0])];
		}
	}
	else {
		for (int i = 0; i < mesh.FacesArray.size(); ++i) {
			int v[3] = { mesh.FacesArray[i].v[0], mesh.FacesArray[i].v[1], mesh.FacesArray[i].v[2] };
			heIndices[std::make_pair(v[0], v[1])] = heIndices.size();
			heIndices[std::make_pair(v[1], v[2])] = heIndices.size();
			heIndices[std::make_pair(v[2], v[0])] = heIndices.size();
			mesh.VerticesArray[v[0]].valence = 0;
			mesh.VerticesArray[v[1]].valence = 0;
			mesh.VerticesArray[v[2]].valence = 0;
			mesh.VerticesArray[v[0]].heIndices.clear();
			mesh.VerticesArray[v[1]].heIndices.clear();
			mesh.VerticesArray[v[2]].heIndices.clear();
		}
		mesh.HalfEdgeArray.resize(heIndices.size());
		for (int i = 0; i < mesh.FacesArray.size(); ++i) {
			int v[3] = { mesh.FacesArray[i].v[0], mesh.FacesArray[i].v[1], mesh.FacesArray[i].v[2] };

			mesh.VerticesArray[v[0]].valence++;
			mesh.VerticesArray[v[1]].valence++;
			mesh.VerticesArray[v[2]].valence++;
			int i0 = heIndices[std::make_pair(v[2], v[0])];
			int i1 = heIndices[std::make_pair(v[0], v[1])];
			int i2 = heIndices[std::make_pair(v[1], v[2])];
			mesh.VerticesArray[v[0]].heIndices.push_back(i1);
			mesh.VerticesArray[v[1]].heIndices.push_back(i2);
			mesh.VerticesArray[v[2]].heIndices.push_back(i0);

			//Find pairs of the half edges
			mesh.HalfEdgeArray[i0] = MeshHalfEdge(heIndices[std::make_pair(v[0], v[2])]);
			mesh.HalfEdgeArray[i0].faceIndex = i;
			mesh.HalfEdgeArray[i1] = MeshHalfEdge(heIndices[std::make_pair(v[1], v[0])]);
			mesh.HalfEdgeArray[i1].faceIndex = i;
			mesh.HalfEdgeArray[i2] = MeshHalfEdge(heIndices[std::make_pair(v[2], v[1])]);
			mesh.HalfEdgeArray[i2].faceIndex = i;

			//Find next half edges inside the face
			mesh.HalfEdgeArray[i0].next = heIndices[std::make_pair(v[0], v[1])];
			mesh.HalfEdgeArray[i1].next = heIndices[std::make_pair(v[1], v[2])];
			mesh.HalfEdgeArray[i2].next = heIndices[std::make_pair(v[2], v[0])];


			//Find previous half edges inside the face
			mesh.HalfEdgeArray[i0].prev = heIndices[std::make_pair(v[1], v[2])];
			mesh.HalfEdgeArray[i1].prev = heIndices[std::make_pair(v[2], v[0])];
			mesh.HalfEdgeArray[i2].prev = heIndices[std::make_pair(v[0], v[1])];

			//Storing vertex indices at starting points
			mesh.HalfEdgeArray[i0].vIndex = v[2];
			mesh.HalfEdgeArray[i1].vIndex = v[0];
			mesh.HalfEdgeArray[i2].vIndex = v[1];

			//Storing half edge indices in faces too
			mesh.FacesArray[i].HalfEdgeArray[0] = heIndices[std::make_pair(v[0], v[1])];
			mesh.FacesArray[i].HalfEdgeArray[1] = heIndices[std::make_pair(v[1], v[2])];
			mesh.FacesArray[i].HalfEdgeArray[2] = heIndices[std::make_pair(v[2], v[0])];
		}
	}
}

/*
	http://www.opengl-tutorial.org/hu/beginners-tutorials/tutorial-7-model-loading/
*/
inline Mesh readMeshObj(std::string path) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

	std::map<std::pair<int, int>, int> heIndices;

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
			int vertexIndex[4], uvIndex[4], normalIndex[4];

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

			if (content.size() == 5) {
				mesh.quadratic = true;

				std::string fourthPart = content[4];
				std::vector<std::string> fourthPartNumbers = wordsInLine(fourthPart, '/');

				vertexIndex[3] = std::stoi(fourthPartNumbers[0]) - 1;
				if (fourthPartNumbers.size() > 1)
					uvIndex[3] = std::stoi(fourthPartNumbers[1]) - 1;
				if (fourthPartNumbers.size() > 2)
					normalIndex[3] = std::stoi(fourthPartNumbers[2]) - 1;

				//mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));
				//Add half-edge indices to edges PER FACE
				//If added only to vertices, indices are overwritten by other face's data
				MeshFace f = MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2], vertexIndex[3]);
				mesh.FacesArray.push_back(f);

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
			}
			else {
				heIndices[std::make_pair(vertexIndex[0], vertexIndex[1])] = heIndices.size();
				heIndices[std::make_pair(vertexIndex[1], vertexIndex[2])] = heIndices.size();
				heIndices[std::make_pair(vertexIndex[2], vertexIndex[0])] = heIndices.size();

				//mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));
				//Add half-edge indices to edges PER FACE
				//If added only to vertices, indices are overwritten by other face's data
				MeshFace f = MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]);
				mesh.FacesArray.push_back(f);

				//for loop scheme
				mesh.VerticesArray[vertexIndex[0]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[2], vertexIndex[0])];
				mesh.VerticesArray[vertexIndex[1]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[0], vertexIndex[1])];
				mesh.VerticesArray[vertexIndex[2]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[1], vertexIndex[2])];

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
	}

	file.close();

	initHalfEdges(mesh);

	return mesh;
}

inline Mesh readMeshStl(std::string path) {
	Mesh mesh = Mesh();

	std::map<std::pair<int, int>, int> heIndices;

	std::ifstream file;
	file.open(path);

	if (!file.is_open()) {
		throw std::invalid_argument("Impossible to open the file!");
	}

	std::string line;

	std::getline(file, line);

	if (wordsInLine(line)[0] != "solid") {
		throw std::invalid_argument("An STL file must start with the keyword 'solid'!");
	}

	//MeshFace face = MeshFace();

	int verticesRead = 0;
	std::set<MeshVertex> vertices;

	while (std::getline(file, line)) {
		std::vector<std::string> content = wordsInLine(line);

		if (content[0] == "facet" && content[1] == "normal") {
			Point3D normal = Point3D(std::stod(content[2]), std::stod(content[3]), std::stod(content[4]));
			mesh.NormalsArray.push_back(normal);

			std::getline(file, line); // outer loop

			int vertexIndex[3];
			MeshVertex debug[3];

			for (int i = 0; i < 3; i++) {
				std::getline(file, line);

				vertexIndex[i] = verticesRead;

				std::vector<std::string > vertexRow = wordsInLine(line);

				MeshVertex vertex = MeshVertex(std::stod(vertexRow[1]), std::stod(vertexRow[2]), std::stod(vertexRow[3]));
				debug[i] = vertex;
				//if (vertices.find(vertex) != vertices.end()) {
				//if(std::find(vertices.begin(), vertices.end(), vertex) != vertices.end()){
				bool found = false;

				for (int ii = 0; ii < verticesRead; ii++) {
					if (mesh.VerticesArray[ii] == vertex) {
						found = true;
						vertexIndex[i] = ii;
					}
				}

				if (!found) {
					//vertices.insert(vertex);
					mesh.VerticesArray.push_back(vertex);
					verticesRead++;
				}
			}

			heIndices[std::make_pair(vertexIndex[0], vertexIndex[1])] = heIndices.size();
			heIndices[std::make_pair(vertexIndex[1], vertexIndex[2])] = heIndices.size();
			heIndices[std::make_pair(vertexIndex[2], vertexIndex[0])] = heIndices.size();

			mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));
			//for loop scheme
			mesh.VerticesArray[vertexIndex[0]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[2], vertexIndex[0])];
			mesh.VerticesArray[vertexIndex[1]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[0], vertexIndex[1])];
			mesh.VerticesArray[vertexIndex[2]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[1], vertexIndex[2])];

			std::getline(file, line); // endloop

			std::getline(file, line); // endfacet

		}
	}

	file.close();
	initHalfEdges(mesh);

	return mesh;
}

inline Mesh readMeshPly(std::string path) {
	Mesh mesh = Mesh();
	EasyAsPLY::PlyReader reader;
	std::map<std::pair<int, int>, int> heIndices;

	reader.Open();

	if (reader.Read(path)) {
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

			int vertexIndex[3] = { std::stoi(content[0]), std::stoi(content[1]), std::stoi(content[2]) };

			mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));

			heIndices[std::make_pair(vertexIndex[0], vertexIndex[1])] = heIndices.size();
			heIndices[std::make_pair(vertexIndex[1], vertexIndex[2])] = heIndices.size();
			heIndices[std::make_pair(vertexIndex[2], vertexIndex[0])] = heIndices.size();

			//for loop scheme
			mesh.VerticesArray[vertexIndex[0]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[2], vertexIndex[0])];
			mesh.VerticesArray[vertexIndex[1]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[0], vertexIndex[1])];
			mesh.VerticesArray[vertexIndex[2]].halfEdgeIndex = heIndices[std::make_pair(vertexIndex[1], vertexIndex[2])];
		}

	}
	else {
		throw std::invalid_argument("Impossible to open the file!");
	}

	reader.Close();

	initHalfEdges(mesh);

	return mesh;
}

inline Mesh readMesh(std::string path, MeshType type) {
	switch (type)
	{
	case OBJ:
		return readMeshObj(path);
		break;
	case PLY:
		return readMeshPly(path);
		break;
	case STL:
		return readMeshStl(path);
		break;
	default:
		throw std::invalid_argument("Unknown type");
	}
}




inline void saveMeshObj(std::string path, Mesh mesh) {
	std::ofstream outFile;
	outFile.open(path);

	for (int i = 0, n = mesh.VerticesArray.size(); i < n; i++) {
		MeshVertex vertex = mesh.VerticesArray[i];
		outFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}

	for (int i = 0, n = mesh.NormalsArray.size(); i < n; i++) {
		Point3D norm = mesh.NormalsArray[i];
		outFile << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
	}

	for (int i = 0, n = mesh.FacesArray.size(); i < n; i++) {
		MeshFace face = mesh.FacesArray[i];
		outFile << "f " << face.v[0] + 1 << " " << face.v[1] + 1 << " " << face.v[2] + 1 << std::endl;
	}

	outFile.close();
}

inline void saveMeshStl(std::string path, Mesh mesh) {
	std::ofstream outFile;
	outFile.open(path);

	outFile << "solid model" << std::endl; // solid-dal kell kezdeni a fjlt, aztn egy nv

	for (int i = 0, n = mesh.FacesArray.size(); i < n; i++) {
		outFile << "facet" << std::endl; //<< "facet normal " << mesh.NormalsArray[i].x << " " << mesh.NormalsArray[i].y << " " << mesh.NormalsArray[i].z << std::endl;
		outFile << "outer loop" << std::endl;

		MeshFace face = mesh.FacesArray[i];

		for (int i = 0, n = sizeof(face.v) / sizeof(*face.v); i < n; i++) {
			outFile << "vertex " << mesh.VerticesArray[face.v[i]].x << " " << mesh.VerticesArray[face.v[i]].y << " " << mesh.VerticesArray[face.v[i]].z << std::endl;
		}

		outFile << "endloop" << std::endl;
		outFile << "endfacet" << std::endl;
	}

	outFile << "endsolid model" << std::endl;

	outFile.close();
}

inline void saveMeshPly(std::string path, Mesh mesh) {
	std::ofstream outFile;
	outFile.open(path);

	outFile << "ply" << std::endl;
	outFile << "format ascii 1.0" << std::endl;
	outFile << "element vertex " << mesh.VerticesArray.size() << std::endl;
	outFile << "property float x" << std::endl;
	outFile << "property float y" << std::endl;
	outFile << "property float z" << std::endl;
	outFile << "element face " << mesh.FacesArray.size() << std::endl;
	outFile << "property list uchar uint vertex_indices" << std::endl;
	outFile << "end_header" << std::endl;

	for (int i = 0, n = mesh.VerticesArray.size(); i < n; i++) {
		MeshVertex vertex = mesh.VerticesArray[i];
		outFile << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}

	for (int i = 0, n = mesh.FacesArray.size(); i < n; i++) {
		MeshFace face = mesh.FacesArray[i];
		outFile << sizeof(face.v) / sizeof(*face.v) << " " << face.v[0] << " " << face.v[1] << " " << face.v[2] << std::endl;
	}

	outFile.close();
}

inline void saveMesh(std::string path, Mesh mesh, MeshType type) {
	switch (type)
	{
	case OBJ:
		saveMeshObj(path, mesh);
		break;
	case PLY:
		//throw std::invalid_argument("Saving object to .ply is not implemented. Try saving to .obj or to .stl.");
		saveMeshPly(path, mesh);
		break;
	case STL:
		saveMeshStl(path, mesh);
		break;
	default:
		throw std::invalid_argument("Unknown type");
	}
}
