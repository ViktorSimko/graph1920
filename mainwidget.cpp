#include <QtWidgets/QtWidgets>
#include "mainwidget.h"

// Constructor for main widget
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
   button_loop = new QPushButton(tr("Loop"));
   button_butterfly = new QPushButton(tr("Butterfly"));
   button_squareroot = new QPushButton(tr("Square root 3"));
   button_catmull = new QPushButton(tr("Catmull-Clark"));

   button_reset = new QPushButton("Reset");

   oglWidget_ = new OGLWidget();
   ge = new graph_engine();
   oglWidget_->setGraphEngine(ge);
   ge->initObject();


   QGridLayout *mainLayout = new QGridLayout;

   mainLayout->addWidget(button_loop, 0, 0);
   mainLayout->addWidget(button_butterfly, 0, 1);
   mainLayout->addWidget(button_squareroot, 0, 2);
   mainLayout->addWidget(button_catmull, 0, 3);
   mainLayout->addWidget(oglWidget_, 1, 0, 1, 4);
   mainLayout->addWidget(button_reset, 2, 1, 1, 2);

   connect(button_loop, SIGNAL (released()), this, SLOT (applyLoop()));
   connect(button_butterfly, SIGNAL (released()), this, SLOT (applyButterfly()));
   connect(button_squareroot, SIGNAL (released()), this, SLOT (applySquareroot()));
   connect(button_catmull, SIGNAL (released()), this, SLOT (applyCatmull()));
   connect(button_reset, SIGNAL (released()), this, SLOT (reset()));

   setLayout(mainLayout);
   setWindowTitle(tr("Connecting buttons to processes.."));
}

void MainWidget::applyLoop() {
   std::cout << "applying loop schema" << std::endl;
   ge->applyLoopSchema();
}

void MainWidget::applyButterfly() {
   std::cout << "applying butterfly schema" << std::endl;
   ge->applyButterflySchema();
}

void MainWidget::applySquareroot() {
   std::cout << "applying squareroot 3 schema" << std::endl;
   ge->applySquarerootSchema();
}

void MainWidget::applyCatmull() {
   std::cout << "applying catmull clark schema" << std::endl;
   ge->applyCatmullSchema();
}

void MainWidget::reset() {
   ge->reset();
}

// Destructor
MainWidget::~MainWidget()
{
   delete button_loop;
   delete button_butterfly;
   delete button_catmull;
   delete button_squareroot;
   delete oglWidget_;
}
