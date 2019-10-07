#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "NoiseGenerator.h"
#include "BoundaryBox.h"
#include "mesh.h"
#include <vector>
#include "math.h"
#include <QtWidgets/QtWidgets>
#include <random>


NoiseGenerator::NoiseGenerator(Mesh mesh, BoundaryBox* boundaryBox) {
    this->mesh = mesh;
    this->boundaryBox = boundaryBox;
}

Mesh NoiseGenerator::generateNoise(int noise, int point) {
    srand(time(NULL));
    float diagonalLength = this->boundaryBox->diagonalLength * 0.2;

    float noisePercentage = (float) noise / 100;
    float pointPercentage = (float) point / 100;

    float noiseLevel = diagonalLength * noisePercentage;

    std::vector<int> indices;
    Mesh newMesh = this->mesh;

    int numberOfPoints = round(this->mesh.VerticesArray.size() * pointPercentage);

    int index;
    int i = 0;
    while (i < numberOfPoints) {
        index = rand() % this->mesh.VerticesArray.size();
        if(std::find(indices.begin(), indices.end(), index) == indices.end()) {
            indices.push_back(index);
            i++;
        }
    }

    for (auto ind : indices) {
        if (noiseLevel == 0) {
            return newMesh;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        float xNoise = dis(gen) * noiseLevel;
        float yNoise = dis(gen) * noiseLevel;
        float zNoise = dis(gen) * noiseLevel;



        int xPrefix = rand() % 2 + 1;
        int yPrefix = rand() % 2 + 1;
        int zPrefix = rand() % 2 + 1;

        newMesh.VerticesArray[ind].x += pow(-1, xPrefix) * xNoise;
        newMesh.VerticesArray[ind].y += pow(-1, yPrefix) * yNoise;
        newMesh.VerticesArray[ind].z += pow(-1, zPrefix) * zNoise;
    }


    return newMesh;

}

NoiseGenerator::~NoiseGenerator() {}