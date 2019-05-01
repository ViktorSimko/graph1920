#include <graph_engine.h>

void graph_engine::initObject() {
    mesh_object = readMesh("test.obj", MeshType::OBJ);
}

void graph_engine::applySchema() {
    LoopSchema schema;
    mesh_object = schema.apply(mesh_object);
    glutPostRedisplay();
}

void graph_engine::drawMesh() {

    Color color = mesh_object.faceColor;
    for (auto face : mesh_object.FacesArray)
    {
        glBegin(GL_POLYGON);
        for (int vertexIdx : face.v)
        {
            MeshVertex v = mesh_object.VerticesArray[vertexIdx];
            //glColor3i(color.r, color.g, color.b);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3d(v.x / 3.0, v.y / 3.0, v.z / 3.0);
        }
        glEnd();
    }

    for (auto face : mesh_object.FacesArray)
    {
        glBegin(GL_LINE_LOOP);
        for (int vertexIdx : face.v)
        {
            MeshVertex v = mesh_object.VerticesArray[vertexIdx];
            //glColor3i(color.r, color.g, color.b);
            glLineWidth(10.0);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3d(v.x / 3.0, v.y / 3.0, v.z / 3.0);
            //printf("%lf %lf %lf\n", v.x, v.y, v.z);
        }
        glEnd();
    }
}
