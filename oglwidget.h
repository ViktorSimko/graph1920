#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QMouseEvent>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "graph_engine.h"

class OGLWidget : public QOpenGLWidget
{
public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

    void setGraphEngine(graph_engine* ge);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent *event);

private:
    graph_engine* ge;

    int rotationX = 0, rotationY = 0;
    float last_x = 0.0, last_y = 0.0;
    double zoom = 0.2;
};

#endif // OGLWIDGET_H