#include <QtWidgets/QtWidgets>
#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->setFocusPolicy ( Qt::StrongFocus );
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        last_x = event->x();
        last_y = event->y();
    }
}

void OGLWidget::mouseMoveEvent(QMouseEvent* event) {
    rotationX += (float)(event->y() - last_y);
    rotationY += (float)(event->x() - last_x);

    last_x = event->x();
    last_y = event->y();

    this->repaint();
}

void OGLWidget::wheelEvent(QWheelEvent *event) {
    zoom += (event->delta() > 0) ? 0.05 : -0.05;
    if (zoom < 0) {
        zoom = 0;
    }
    qDebug() << zoom;
    this->repaint();
}

void OGLWidget::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Down) {
        translateZ += 0.1;
    }

    if(event->key() == Qt::Key_Up) {
        translateZ -= 0.1;
    }

    this->repaint();
}


void OGLWidget::setGraphEngine(graph_engine* ge) {
    this->ge = ge;
}

void OGLWidget::initializeGL()
{
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0f);
}

void OGLWidget::paintGL()
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

    glScalef(zoom, zoom, zoom);

    this->ge->drawMesh();
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,3,0,0,0,0,1,0);
}