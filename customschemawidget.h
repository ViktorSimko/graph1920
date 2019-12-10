#ifndef CUSTOMSCHEMAWIDGET_H
#define CUSTOMSCHEMAWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QLineEdit>
#include <QPaintEvent>
#include <QGraphicsItem>
#include <QRectF>
#include <QDoubleSpinBox>

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
    explicit CustomSchemaWidget(json& items, QWidget *parent = 0, std::string approx = "", std::string name = "");
    ~CustomSchemaWidget();

private slots:
    void saveCustomScheme();

private:
    int ofsX;
    int ofsY;
    int sc;
    std::string scheme_name;
    std::string scheme_key;
    json selected_item;
    json custom_schemes;
    
    QPushButton* save_button;
    QLabel* label_save;
    QLabel* label_name;
    QLineEdit* line_name;
    std::map<char, QDoubleSpinBox*> weights;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    QDoubleSpinBox* getNewDoubleSpinBoxForWeight();
    std::map<char, QDoubleSpinBox*> getWeightBoxes();

    int noise = 0;
    int points = 0;


};

#endif
