#ifndef NOISEGLORIGINAL_H
#define NOISEGLORIGINAL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QMouseEvent>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "graph_engine.h"

class NoiseGLOriginal : public QOpenGLWidget
{
    Q_OBJECT
public:
    NoiseGLOriginal(QWidget *parent = 0);
    ~NoiseGLOriginal();

    void setGraphEngine(graph_engine* ge);

public slots:
    void setDimensions(int rotationX, int rotationY, float last_x, float last_y);

signals:
    void dimensionChanged(int rotationX, int rotationY, float last_x, float last_y);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    graph_engine* ge;

    int rotationX = 0, rotationY = 0;
    float last_x = 0.0, last_y = 0.0;
};

#endif // NOISEGLORIGINAL_H