//
// Created by Lajos Kovács on 2019. 10. 07..
//

#ifndef GRAPH2019_BOUNDARYBOX_H
#define GRAPH2019_BOUNDARYBOX_H


class BoundaryBox {
public:
    BoundaryBox();
    BoundaryBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float cogX, float cogY, float cogZ);
    float minX, minY, minZ, maxX, maxY, maxZ, cogX, cogY, cogZ;
    float diagonalLength;
};


#endif //GRAPH2019_BOUNDARYBOX_H
