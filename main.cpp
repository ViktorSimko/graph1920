#include <tuple>

#include "io.h"
#include "object.h"

int main() {
	try {
		std::tuple<std::vector<Point3D>, std::vector<Point2D>, std::vector<Point3D>> result = readMesh("test.obj", MeshType::OBJ);

		std::vector<Point3D> vertices = std::get<0>(result);
		std::vector<Point2D> uvs = std::get<1>(result);
		std::vector<Point3D> normals = std::get<2>(result);

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