#ifndef GRAPH_ENGINE_H
#define GRAPH_ENGINE_H

#include "io.h"
#include "mesh.h"
#include "loop_schema.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class graph_engine {

public:

    void initObject();
    void applySchema();
    void drawMesh();

    void mouseClick(int button, int button_state, int x, int y);
    void mouseMotion(int x, int y);

private:

    Mesh mesh_object;

};

#endif