#include <tuple>

#include "io.h"
#include "object.h"
#include "mesh.h"
#include "loop_schema.h"

void printDetails(Mesh mesh) {
	std::vector<MeshVertex> vertices = mesh.VerticesArray;
	std::vector<Point2D> uvs = mesh.UVArray;
	std::vector<Point3D> normals = mesh.NormalsArray;

	std::cout << vertices.size() << std::endl;
	std::cout << uvs.size() << std::endl;
	std::cout << normals.size() << std::endl;
}

int main() {
	try {
		//Mesh mesh = readMesh("test.obj", MeshType::OBJ);
		Mesh mesh = readMesh("test.ply", MeshType::PLY);

		std::cout << "Before subdivision" << std::endl;

		printDetails(mesh);

		LoopSchema loopSchema = LoopSchema();

		mesh = loopSchema.apply(mesh);

		std::cout << "After subdivision" << std::endl;

		printDetails(mesh);

	}
	catch (std::invalid_argument e) {
		std::cout << "Oops" << std::endl;
		std::cout << e.what() << std::endl;
	}

	std::cin.get();

	return 0;
}