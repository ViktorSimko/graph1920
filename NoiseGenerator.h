//
// Created by Lajos Kovács on 2019. 10. 07..
//

#ifndef GRAPH2019_NOISEGENERATOR_H
#define GRAPH2019_NOISEGENERATOR_H

#include "mesh.h"
#include "BoundaryBox.h"


class NoiseGenerator {
public:
    NoiseGenerator(Mesh mesh, BoundaryBox* boundaryBox);
    ~NoiseGenerator();

    Mesh generateNoise(int noise, int point);
private:
    Mesh mesh;
    BoundaryBox* boundaryBox;
};


#endif //GRAPH2019_NOISEGENERATOR_H
