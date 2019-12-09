#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QListWidget>
#include "oglwidget.h"
#include "graph_engine.h"
#include "noisewidget.h"
#include "customschemawidget.h"
#include "json.hpp"
#include "jsonio.h"

class QPushButton;
class OGLWidget;
class NoiseWidget;

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
    void applyLoop(float a = 3.0/8.0, float b = 3.0/8.0,
    float c = 1.0/8.0, float d = 1.0/8.0);
    void applyButterfly(float a = 1.0/2.0, float b = 1.0/8.0,
    float c = 1.0/16.0);
    void applySquareroot(float a = 0.333, float b = 0.333, float c = 0.333);
    void applyCatmull(float a = 0.25, float b = 0.25, float c = 0.25, float d = 0.25);
    void applyDooSabin();
    void reset();
    void showNoiseGenerator();
    void showCustomSchemaModifier();
    void initCustomSchemaList();
    void applyCustom();
    void enableAllSchemes(bool isEnabled);

private:
    QDoubleSpinBox* box_a;
    QDoubleSpinBox* box_b;
    QDoubleSpinBox* box_c;
    QDoubleSpinBox* box_d;

    QPushButton* button_loop;
    QPushButton* button_butterfly;
    QPushButton* button_squareroot;
    QPushButton* button_catmull;
    QPushButton* button_doosabin;
    QPushButton* button_reset;
    QPushButton* button_load;
    QPushButton* button_save;
    QPushButton* button_noise;
    QPushButton* button_custom_apply;
    QPushButton* button_custom;

    QListWidget* list_custom_schemes;

    graph_engine* ge;

    OGLWidget* oglWidget_;

    NoiseWidget* noiseWidget;
    std::string selected_custom_scheme_name;
    std::string selected_custom_scheme_key;

    json custom_schemes;
    CustomSchemaWidget* customSchemaWidget;
};

#endif // MAINWIDGET_H
