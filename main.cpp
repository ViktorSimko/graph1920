#include <tuple>

#include "io.h"
#include "object.h"

int main() {
	try {
		Mesh result = readMesh("test.obj", MeshType::OBJ);

		std::vector<MeshVertex> vertices = result.VerticesArray;
		std::vector<Point2D> uvs = result.UVArray;
		std::vector<Point3D> normals = result.NormalsArray;

		std::cout << vertices.size() << std::endl;
		std::cout << uvs.size() << std::endl;
		std::cout << normals.size() << std::endl;

	}
	catch (std::invalid_argument e) {
		std::cout << "Oops";
	}

	std::cin.get();

	return 0;
}