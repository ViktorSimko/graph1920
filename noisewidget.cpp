#include <QtWidgets/QtWidgets>
#include "noisewidget.h"
#include "graph_engine.h"
#include "noisegloriginal.h"
#include "noiseglnoisy.h"

#include "math.h"


// Constructor for main widget
NoiseWidget::NoiseWidget(QWidget *parent, graph_engine* ge) :
    QDialog(parent)
{
   button_reset = new QPushButton("Reset");
   button_close = new QPushButton("Close");
   button_apply = new QPushButton("Apply");

   slider_noise = new QSlider(Qt::Horizontal, this);
   slider_points = new QSlider(Qt::Horizontal, this);
   graphEngine = ge;

   label_noise = new QLabel(this);
   label_noise->setText("Noise: " + QString::number(noise) + "%");
   label_points = new QLabel(this);
   label_points->setText("points: " + QString::number(points) + "% " + "(" + QString::number(points / 100.0 * graphEngine->getMeshPoints()) + ")");


   noiseGlOriginal = new NoiseGLOriginal(this);
   noiseGlNoisy = new NoiseGLNoisy(this);

   noiseGlOriginal->setGraphEngine(graphEngine);
   noiseGlNoisy->setGraphEngine(graphEngine);

   QGridLayout *mainLayout = new QGridLayout;

   mainLayout->addWidget(slider_noise, 1, 1, 1, 2);
   mainLayout->addWidget(slider_points, 1, 3, 1, 2);

   mainLayout->addWidget(label_noise, 2, 1, 1, 2);
   mainLayout->addWidget(label_points, 2, 3, 1, 2);

   mainLayout->addWidget(noiseGlOriginal, 3, 1, 1, 2);
   mainLayout->addWidget(noiseGlNoisy, 3, 3, 1, 2);

   mainLayout->addWidget(button_close, 4, 1);
   mainLayout->addWidget(button_reset, 4, 2);
   mainLayout->addWidget(button_apply, 4, 4);


    connect(noiseGlOriginal, &NoiseGLOriginal::dimensionChanged, noiseGlNoisy, &NoiseGLNoisy::setDimensions);
    connect(noiseGlNoisy, &NoiseGLNoisy::dimensionChanged, noiseGlOriginal, &NoiseGLOriginal::setDimensions);

   connect(button_reset, SIGNAL (released()), this, SLOT (reset()));
   connect(button_close, SIGNAL (released()), this, SLOT (close()));
   connect(button_apply, SIGNAL (released()), this, SLOT (apply()));

   connect(slider_noise, &QSlider::valueChanged, this, &NoiseWidget::noiseChanged);
   connect(slider_points, &QSlider::valueChanged, this, &NoiseWidget::pointsChanged);

    QRect rec = QApplication::desktop()->screenGeometry();
    int height = rec.height();
    int width = rec.width();

    qDebug() << height;
    qDebug() << width;

   int widgetHeight = height / 2;
   int widgetWidth = width / 2;
    qDebug() << widgetHeight;
    qDebug() << widgetWidth;

   if (widgetHeight > 768) {
       widgetHeight = 768;
   }
   if (widgetWidth > 1024) {
       widgetWidth = 1024;
   }

   setLayout(mainLayout);
   setWindowTitle(tr("Noise"));
   setFixedHeight(widgetHeight);
   setFixedWidth(widgetWidth);
}

void NoiseWidget::reset() {
    graphEngine->initNoiseGenerator();
    slider_noise->setValue(0);
    slider_points->setValue(0);
    noiseGlOriginal->repaint();
    noiseGlNoisy->repaint();
}

void NoiseWidget::noiseChanged(int value) {
    this->noise = value;
    label_noise->setText("Noise: " + QString::number(value) + "%");
    changedValue();
}

void NoiseWidget::pointsChanged(int value) {
    this->points = value;
    qDebug() << QString::number(value / 100.0);
    label_points->setText("points: " + QString::number(value) + "% " + "(" + QString::number(round(value / 100.0 * graphEngine->getMeshPoints())) + ")");
    changedValue();
}

void NoiseWidget::changedValue() {
    graphEngine->generateNoise(noise, points);
    noiseGlNoisy->repaint();
}


void NoiseWidget::apply() {
    graphEngine->applyNoise();
    close();
}

NoiseWidget::~NoiseWidget() {}
