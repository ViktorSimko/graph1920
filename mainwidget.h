#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include "oglwidget.h"
#include "graph_engine.h"

class QPushButton;
class OGLWidget;

// This is the declaration of our MainWidget class
// The definition/implementation is in mainwidget.cpp
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0); //Constructor
    ~MainWidget(); // Destructor

private slots:
    void load();
    void save();
    void applyLoop();
    void applyButterfly();
    void applySquareroot();
    void applyCatmull();
    void reset();

private:
    QPushButton* button_loop;
    QPushButton* button_butterfly;
    QPushButton* button_squareroot;
    QPushButton* button_catmull;
    QPushButton* button_reset;
    QPushButton* button_load;
    QPushButton* button_save;
    graph_engine* ge;
    OGLWidget* oglWidget_;

};

#endif // MAINWIDGET_H
