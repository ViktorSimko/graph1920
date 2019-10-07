#ifndef NOISEWIDGET_H
#define NOISEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

#include "noisegloriginal.h"
#include "noiseglnoisy.h"
#include "graph_engine.h"

class QPushButton;
class NoiseGLOriginal;
class NoiseGLNoisy;
class QSlider;

class NoiseWidget : public QDialog
{
    Q_OBJECT

public:
    explicit NoiseWidget(QWidget *parent = 0, graph_engine* ge = nullptr);
    ~NoiseWidget();

private slots:
    void reset();
    void apply();
    void noiseChanged(int value);
    void pointsChanged(int value);

private:
    graph_engine* graphEngine;
    QPushButton* button_reset;
    QPushButton* button_close;
    QPushButton* button_apply;
    QSlider* slider_noise;
    QSlider* slider_points;
    QLabel* label_noise;
    QLabel* label_points;
    NoiseGLOriginal* noiseGlOriginal;
    NoiseGLNoisy* noiseGlNoisy;

    void changedValue();

    int noise = 0;
    int points = 0;

};

#endif
