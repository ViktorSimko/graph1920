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

    update();
}

void OGLWidget::wheelEvent(QWheelEvent *event) {
    zoom += (event->delta() > 0) ? 0.005 : -0.005;
    if (zoom < 0) {
        zoom = 0;
    }

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
    glViewport(0, 0, QWidget::width(), QWidget::height());
    glPointSize(5.0f);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glEnable(GL_DEPTH_TEST);

}

void OGLWidget::paintGL()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float xy_aspect;
//    float xy_aspect = (float) this->size().width() / (float) this->size().height();
//    gluPerspective(45, xy_aspect, 0.01, 100.0);

    glFrustum(0, 0, -1, 1, 1, 1.0);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0,0,3,0,0,0,0,1,0);

    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);

    glScalef(zoom, zoom, zoom);

    this->ge->drawMesh();
    glFlush();
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    float xy_aspect = (float) w / (float) h;
//    glLoadIdentity();
//    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glFrustum(-xy_aspect * .08, xy_aspect * .08, -.08, .08, .1, 15.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0,0,3,0,0,0,0,1,0);
}