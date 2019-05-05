#include <graph_engine.h>

void graph_engine::initObject() {
    mesh_object = readMesh("test.obj", MeshType::OBJ);
    original_mesh = mesh_object;
}

void graph_engine::reset() {
    mesh_object = original_mesh;
}

void graph_engine::applyLoopSchema() {
    LoopSchema schema;
    mesh_object = schema.apply(mesh_object);
}

void graph_engine::applyButterflySchema() {
    ButterflySchema schema;
    mesh_object = schema.apply(mesh_object);
}

void graph_engine::applySquarerootSchema() {
    SquareRoot3 schema;
    mesh_object = schema.apply(mesh_object);
}

void graph_engine::applyCatmullSchema() {
    CatmullClark schema;
    mesh_object = schema.apply(mesh_object);
}

void graph_engine::drawMesh() {

    Color color = mesh_object.faceColor;
    int end = mesh_object.quadratic ? 4 : 3;
    for (auto face : mesh_object.FacesArray)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < end; i++)
        {
            int vertexIdx = face.v[i];
            MeshVertex v = mesh_object.VerticesArray[vertexIdx];
            //glColor3i(color.r, color.g, color.b);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3d(v.x / 2.0, v.y / 2.0, v.z / 2.0);
        }
        glEnd();
    }

    for (auto face : mesh_object.FacesArray)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < end; i++)
        {
            int vertexIdx = face.v[i];
            MeshVertex v = mesh_object.VerticesArray[vertexIdx];
            //glColor3i(color.r, color.g, color.b);
            glLineWidth(15.0);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3d(v.x / 2.0, v.y / 2.0, v.z / 2.0);
            //printf("%lf %lf %lf\n", v.x, v.y, v.z);
        }
        glEnd();
    }
}
