#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>

#include "helper.h"
#include "object.h"
#include "mesh.h"

enum MeshType {
	OBJ
};

/*
	http://www.opengl-tutorial.org/hu/beginners-tutorials/tutorial-7-model-loading/
*/

std::tuple<std::vector<MeshVertex>, std::vector<Point2D>, std::vector<Point3D>> readObj(std::string path) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<MeshVertex> temp_vertices;
	std::vector<Point2D> temp_uvs;
	std::vector<Point3D> temp_normals;

	std::ifstream file;
	file.open(path);

	if (!file.is_open()) {
		throw std::invalid_argument("Impossible to open the file !");
	}

	std::string line;

	while (std::getline(file, line)) {

		std::vector<std::string> content = wordsInLine(line);

		if (line[0] == 'v' && line[1] == ' ') {
			MeshVertex vertex = MeshVertex(std::stod(content[1]), std::stod(content[2]), std::stod(content[3]));
			temp_vertices.push_back(vertex);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			Point2D uv = Point2D(std::stod(content[1]), std::stod(content[2]));
			temp_uvs.push_back(uv);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			Point3D normal = Point3D(std::stod(content[1]), std::stod(content[2]), std::stod(content[3]));
			temp_normals.push_back(normal);
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

			vertexIndex[0] = std::stoi(firstPartNumbers[0]);
			uvIndex[0] = std::stoi(firstPartNumbers[1]);
			normalIndex[0] = std::stoi(firstPartNumbers[2]);

			vertexIndex[1] = std::stoi(secondPartNumbers[0]);
			uvIndex[1] = std::stoi(secondPartNumbers[1]);
			normalIndex[1] = std::stoi(secondPartNumbers[2]);

			vertexIndex[2] = std::stoi(thirdPartNumbers[0]);
			uvIndex[2] = std::stoi(thirdPartNumbers[1]);
			normalIndex[2] = std::stoi(thirdPartNumbers[2]);

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

	std::vector<MeshVertex> out_vertices;

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		MeshVertex vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	file.close();

	return std::make_tuple(out_vertices, temp_uvs, temp_normals);
}

Mesh readMeshObj(std::string path) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<MeshVertex> temp_vertices;
	std::vector<Point2D> temp_uvs;
	std::vector<Point3D> temp_normals;

	Mesh mesh = Mesh();

	std::ifstream file;
	file.open(path);

	if (!file.is_open()) {
		throw std::invalid_argument("Impossible to open the file !");
	}

	std::string line;

	while (std::getline(file, line)) {

		std::vector<std::string> content = wordsInLine(line);

		if (line[0] == 'v' && line[1] == ' ') {
			MeshVertex vertex = MeshVertex(std::stod(content[1]), std::stod(content[2]), std::stod(content[3]));
			temp_vertices.push_back(vertex);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			Point2D uv = Point2D(std::stod(content[1]), std::stod(content[2]));
			temp_uvs.push_back(uv);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			Point3D normal = Point3D(std::stod(content[1]), std::stod(content[2]), std::stod(content[3]));
			temp_normals.push_back(normal);
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

			vertexIndex[0] = std::stoi(firstPartNumbers[0]);
			uvIndex[0] = std::stoi(firstPartNumbers[1]);
			normalIndex[0] = std::stoi(firstPartNumbers[2]);

			vertexIndex[1] = std::stoi(secondPartNumbers[0]);
			uvIndex[1] = std::stoi(secondPartNumbers[1]);
			normalIndex[1] = std::stoi(secondPartNumbers[2]);

			vertexIndex[2] = std::stoi(thirdPartNumbers[0]);
			uvIndex[2] = std::stoi(thirdPartNumbers[1]);
			normalIndex[2] = std::stoi(thirdPartNumbers[2]);

			mesh.FacesArray.push_back(MeshFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]));

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

	std::vector<MeshVertex> out_vertices;

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		MeshVertex vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	file.close();

	return mesh;
}

std::tuple<std::vector<MeshVertex>, std::vector<Point2D>, std::vector<Point3D>> readMesh(std::string path, MeshType type) {
	switch (type)
	{
	case OBJ:
		return readObj(path);
		break;
	default:
		throw std::invalid_argument("Unknown type");
	}
}