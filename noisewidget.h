#ifndef NOISEWIDGET_H
#define NOISEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include "noisegloriginal.h"
#include "noiseglnoisy.h"
#include "graph_engine.h"

class QPushButton;
class NoiseGLOriginal;
class NoiseGLNoisy;

class NoiseWidget : public QDialog
{
    Q_OBJECT

public:
    explicit NoiseWidget(QWidget *parent = 0, graph_engine* ge = nullptr);
    ~NoiseWidget();

private slots:
    void reset();
    void apply();

private:
    graph_engine* graphEngine;
    QPushButton* button_reset;
    QPushButton* button_close;
    QPushButton* button_apply;
    NoiseGLOriginal* noiseGlOriginal;
    NoiseGLNoisy* noiseGlNoisy;

};

#endif
