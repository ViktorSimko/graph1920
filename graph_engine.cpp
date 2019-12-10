#include <QtWidgets/QtWidgets>
#include <graph_engine.h>
#include "NoiseGenerator.h"
#include "BoundaryBox.h"

void graph_engine::initObject(std::string path) {
    std::string extension = path.substr(path.rfind("."), path.length());
    auto type = MeshType::OBJ;
    if (extension == ".ply") {
        type = MeshType::PLY;
    }
    if (extension == ".stl") {
        type = MeshType::STL;
    }
    mesh_object = readMesh(path, type);
    original_mesh = mesh_object;
    calculateBoundary();
}

void graph_engine::saveObject(std::string path) {
    std::string extension = path.substr(path.rfind("."), path.length());
    auto type = MeshType::OBJ;
    if (extension == ".ply") {
        type = MeshType::PLY;
    }
    if (extension == ".stl") {
        type = MeshType::STL;
    }
    saveMesh(path, mesh_object, type);
}

void graph_engine::reset() {
    mesh_object = original_mesh;
    calculateBoundary();
}

void graph_engine::calculateBoundary() {
    int i = 0;
    for (MeshVertex v : mesh_object.VerticesArray)
    {
        if(i <= 0) {
            this->minX = v.x;
            this->maxX = v.x;

            this->minY = v.y;
            this->maxY = v.y;

            this->minZ = v.z;
            this->maxZ = v.z;
        }
        if(this->minX > v.x) {
            this->minX = v.x;
        }
        if(this->maxX < v.x) {
            this->maxX = v.x;
        }
        if(this->minY > v.y) {
            this->minY = v.y;
        }
        if(this->maxY < v.y) {
            this->maxY = v.y;
        }
        if(this->minZ > v.z) {
            this->minZ = v.z;
        }
        if(this->maxZ < v.z) {
            this->maxZ = v.z;
        }
        i++;
    }

    this->cogX = (this->minX + this->maxX) / 2;
    this->cogY = (this->minY + this->maxY) / 2;
    this->cogZ = (floorf(this->minZ * 100) / 100 + floorf(this->maxZ * 100) / 100) / 2;
}

void graph_engine::initNoiseGenerator() {
    noise_original = mesh_object;
    noise_noisy = mesh_object;
}

void graph_engine::applyLoopSchema(float a, float b, float c, float d) {
    LoopSchema schema(a, b, c, d);
    mesh_object = schema.apply(mesh_object);
    calculateBoundary();
}

void graph_engine::applyButterflySchema(float a, float b, float c) {
    ButterflySchema schema(a, b, c);
    mesh_object = schema.apply(mesh_object);
    calculateBoundary();
}

void graph_engine::applySquarerootSchema(float a, float b, float c) {
    SquareRoot3 schema(a, b, c);
    mesh_object = schema.apply(mesh_object);
    calculateBoundary();
}

void graph_engine::applyCatmullSchema(float a, float b, float c, float d) {
    CatmullClark schema(a, b, c, d);
    mesh_object = schema.apply(mesh_object);
    calculateBoundary();
}

void graph_engine::applyDooSabinSchema() {
    DooSabin schema;
    mesh_object = schema.apply(mesh_object);
    calculateBoundary();
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
            glVertex3d(v.x- cogX, v.y - cogY, v.z - cogZ);
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
            glVertex3d(v.x - cogX, v.y - cogY, v.z - cogZ);
            //printf("%lf %lf %lf\n", v.x, v.y, v.z);
        }
        glEnd();
    }

    double bMinX = minX - cogX;
    double bMinY = minY - cogY;
    double bMinZ = minZ - cogZ;

    double bMaxX = maxX - cogX;
    double bMaxY = maxY - cogY;
    double bMaxZ = maxZ - cogZ;

    glBegin(GL_POINTS);
    glColor4f(0.0f, 0.0f, 1.0f, 0.6f);
    glVertex3f(bMinX, bMinY, bMinZ);
    glVertex3f(bMaxX, bMaxY, bMaxZ);

    glVertex3f(-bMinX, bMinY, bMinZ);
    glVertex3f(bMinX, -bMinY, bMinZ);
    glVertex3f(bMinX, bMinY, -bMinZ);

    glVertex3f(-bMaxX, bMaxY, bMaxZ);
    glVertex3f(bMaxX, -bMaxY, bMaxZ);
    glVertex3f(bMaxX, bMaxY, -bMaxZ);
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glEnd();

}


void graph_engine::drawNoiseOriginal() {

    Color color = noise_original.faceColor;
    int end = noise_original.quadratic ? 4 : 3;
    for (auto face : noise_original.FacesArray)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < end; i++)
        {
            int vertexIdx = face.v[i];
            MeshVertex v = noise_original.VerticesArray[vertexIdx];
            glColor3f(1.0, 0.0, 0.0);
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();
    }

    for (auto face : noise_original.FacesArray)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < end; i++)
        {
            int vertexIdx = face.v[i];
            MeshVertex v = noise_original.VerticesArray[vertexIdx];
            glLineWidth(15.0);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();
    }
}

void graph_engine::drawNoiseNoisy() {

    Color color = noise_noisy.faceColor;
    int end = noise_noisy.quadratic ? 4 : 3;
    for (auto face : noise_noisy.FacesArray)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < end; i++)
        {
            int vertexIdx = face.v[i];
            MeshVertex v = noise_noisy.VerticesArray[vertexIdx];
            glColor3f(1.0, 0.0, 0.0);
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();
    }

    for (auto face : noise_noisy.FacesArray)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < end; i++)
        {
            int vertexIdx = face.v[i];
            MeshVertex v = noise_noisy.VerticesArray[vertexIdx];
            glLineWidth(15.0);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();
    }

}

void graph_engine::applyNoise() {
    this->mesh_object = noise_noisy;
}

int graph_engine::getMeshPoints() {
    return this->mesh_object.VerticesArray.size();
}

bool graph_engine::isMeshObjectQuadratic() {
    return this->mesh_object.quadratic;
}

void graph_engine::generateNoise(int noise, int points) {
    BoundaryBox* bb = new BoundaryBox(
            this->minX,
            this->minY,
            this->minZ,
            this->maxX,
            this->maxY,
            this->maxZ,
            this->cogX,
            this->cogY,
            this->cogZ
            );
    NoiseGenerator* noiseGenerator = new NoiseGenerator(this->noise_original, bb);
    noise_noisy = noiseGenerator->generateNoise(noise, points);
}
