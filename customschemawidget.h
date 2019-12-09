#ifndef CUSTOMSCHEMAWIDGET_H
#define CUSTOMSCHEMAWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QtWidgets>
#include <QLineEdit>
#include <QPaintEvent>
#include <QGraphicsItem>
#include <QRectF>

#include "customschemawidget.h"
#include "graph_engine.h"

#include "json.hpp"
#include "jsonio.h"
#include <iostream>
#include <map>

class QPushButton;
class NoiseGLOriginal;
class NoiseGLNoisy;
class QSlider;

class CustomSchemaWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CustomSchemaWidget(QWidget *parent, json& items, std::string approx, std::string val);
    ~CustomSchemaWidget();

private slots:
    void saveCustomScheme();
    //void reset();
    //void apply();
    //void noiseChanged(int value);
    //void pointsChanged(int value);

private:
    int ofsX;
    int ofsY;
    int sc;
    std::string scheme_name;
    std::string scheme_key;
    graph_engine* graphEngine;
    json selected_item;
    json custom_schemes;
    
    QPushButton* save_button;
    QLabel* label_save;
    QLabel* label_name;
    QLineEdit* line_name;
    std::map<char, QDoubleSpinBox*> weights;
    //void changedValue();
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    QDoubleSpinBox* getNewDoubleSpinBoxForWeight();
    std::map<char, QDoubleSpinBox*> getWeightBoxes();

    int noise = 0;
    int points = 0;


};

#endif
