#include "noiseglnoisy.h"

NoiseGLNoisy::NoiseGLNoisy(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

NoiseGLNoisy::~NoiseGLNoisy()
{

}

void NoiseGLNoisy::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        last_x = event->x();
        last_y = event->y();
    }
}

void NoiseGLNoisy::mouseMoveEvent(QMouseEvent* event) {
    rotationX += (float)(event->y() - last_y);
    rotationY += (float)(event->x() - last_x);

    last_x = event->x();
    last_y = event->y();

    emit dimensionChanged(rotationX, rotationY, last_x, last_y);

    this->repaint();
}

void NoiseGLNoisy::setDimensions(int rotationX, int rotationY, float last_x, float last_y) {
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->last_x = last_x;
    this->last_y = last_y;
    this->repaint();
}

void NoiseGLNoisy::setGraphEngine(graph_engine* ge) {
    this->ge = ge;
}

void NoiseGLNoisy::initializeGL()
{
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
}

void NoiseGLNoisy::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float xy_aspect = (float) this->size().width() / (float) this->size().height();
    gluPerspective(45, xy_aspect, 0.01, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,3,0,0,0,0,1,0);

    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);

    this->ge->drawMesh();
}

void NoiseGLNoisy::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,3,0,0,0,0,1,0);
}