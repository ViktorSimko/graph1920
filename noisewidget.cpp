#include <QtWidgets/QtWidgets>
#include "noisewidget.h"
#include "graph_engine.h"
#include "noisegloriginal.h"
#include "noiseglnoisy.h"


// Constructor for main widget
NoiseWidget::NoiseWidget(QWidget *parent, graph_engine* ge) :
    QDialog(parent)
{
   button_reset = new QPushButton("Reset");
   button_close = new QPushButton("Close");
   button_apply = new QPushButton("Apply");

    graphEngine = ge;

   noiseGlOriginal = new NoiseGLOriginal(this);
   noiseGlNoisy = new NoiseGLNoisy(this);

    noiseGlOriginal->setGraphEngine(graphEngine);
    noiseGlNoisy->setGraphEngine(graphEngine);

   QGridLayout *mainLayout = new QGridLayout;

   mainLayout->addWidget(noiseGlOriginal, 1, 1, 1, 2);
   mainLayout->addWidget(noiseGlNoisy, 1, 3, 1, 2);

   mainLayout->addWidget(button_close, 2, 1);
   mainLayout->addWidget(button_reset, 2, 2);
   mainLayout->addWidget(button_apply, 2, 4);


    connect(noiseGlOriginal, &NoiseGLOriginal::dimensionChanged, noiseGlNoisy, &NoiseGLNoisy::setDimensions);
    connect(noiseGlNoisy, &NoiseGLNoisy::dimensionChanged, noiseGlOriginal, &NoiseGLOriginal::setDimensions);

   connect(button_reset, SIGNAL (released()), this, SLOT (reset()));
   connect(button_close, SIGNAL (released()), this, SLOT (close()));

   setLayout(mainLayout);
   setWindowTitle(tr("Noise"));
   setFixedHeight(768);
   setFixedWidth(1024);
}

void NoiseWidget::reset() {
    graphEngine->initNoiseGenerator();
   noiseGlOriginal->repaint();
   noiseGlNoisy->repaint();
}


void NoiseWidget::apply() {}

NoiseWidget::~NoiseWidget() {}
