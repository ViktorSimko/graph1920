#ifndef GRAPH_ENGINE_H
#define GRAPH_ENGINE_H

#include "io.h"
#include "mesh.h"
#include "loop_schema.h"
#include "catmull_clark_schema.h"
#include "butterfly_schema.h"
#include "squareroot3_schema.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class graph_engine {

public:

    void initObject(std::string path);
    void saveObject(std::string path);
    void applyLoopSchema(float a, float b, float c, float d);
    void applyButterflySchema(float a, float b, float c);
    void applySquarerootSchema();
    void applyCatmullSchema();
    void drawMesh();
    void reset();

    void initNoiseGenerator();

    void mouseClick(int button, int button_state, int x, int y);
    void mouseMotion(int x, int y);

private:

    Mesh mesh_object;
    Mesh original_mesh;


    Mesh noise_original;
    Mesh noise_noisy;

};

#endif
