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

private:
    graph_engine* ge;

    int rotationX, rotationY;
    float last_x, last_y;
};

#endif // OGLWIDGET_H