//
// Created by Lajos Kovács on 2019. 10. 07..
//

#include "BoundaryBox.h"
#include "math.h"
BoundaryBox::BoundaryBox() {}

BoundaryBox::BoundaryBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float cogX, float cogY,
                         float cogZ) {
    this->minX = minX;
    this->minY = minY;
    this->minZ = minZ;

    this->maxX = maxX;
    this->maxY = maxY;
    this->maxZ = maxZ;

    this->cogX = cogX;
    this->cogY = cogY;
    this->cogZ = cogZ;

    this->diagonalLength = abs(minX - maxX) + abs(minY - maxY) + abs(minZ - maxZ);

}