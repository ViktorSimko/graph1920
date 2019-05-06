#include <QtWidgets/QtWidgets>
#include "mainwidget.h"


// Constructor for main widget
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
   box_a = new QDoubleSpinBox();
   box_a->setValue(3/8.);
   box_b = new QDoubleSpinBox();
   box_b->setValue(3/8.);
   box_c = new QDoubleSpinBox();
   box_c->setValue(1/8.);
   box_d = new QDoubleSpinBox();
   box_d->setValue(1/8.);
   button_load = new QPushButton(tr("Load"));
   button_save = new QPushButton(tr("Save"));
   button_loop = new QPushButton(tr("Loop"));
   button_butterfly = new QPushButton(tr("Butterfly"));
   button_squareroot = new QPushButton(tr("Square root 3"));
   button_catmull = new QPushButton(tr("Catmull-Clark"));

   button_reset = new QPushButton("Reset");

   oglWidget_ = new OGLWidget();
   ge = new graph_engine();
   oglWidget_->setGraphEngine(ge);
   ge->initObject("test.obj");


   QGridLayout *mainLayout = new QGridLayout;

   mainLayout->addWidget(button_load, 0, 0, 1, 2);
   mainLayout->addWidget(button_save, 0, 2, 1, 2);
   mainLayout->addWidget(button_loop, 1, 0);
   mainLayout->addWidget(button_butterfly, 1, 1);
   mainLayout->addWidget(button_squareroot, 1, 2);
   mainLayout->addWidget(button_catmull, 1, 3);
   mainLayout->addWidget(box_a, 2, 0);
   mainLayout->addWidget(box_b, 2, 1);
   mainLayout->addWidget(box_c, 2, 2);
   mainLayout->addWidget(box_d, 2, 3);
   mainLayout->addWidget(oglWidget_, 3, 0, 1, 4);
   mainLayout->addWidget(button_reset, 4, 1, 1, 2);

   connect(button_load, SIGNAL (released()), this, SLOT (load()));
   connect(button_save, SIGNAL (released()), this, SLOT (save()));
   connect(button_loop, SIGNAL (released()), this, SLOT (applyLoop()));
   connect(button_butterfly, SIGNAL (released()), this, SLOT (applyButterfly()));
   connect(button_squareroot, SIGNAL (released()), this, SLOT (applySquareroot()));
   connect(button_catmull, SIGNAL (released()), this, SLOT (applyCatmull()));
   connect(button_reset, SIGNAL (released()), this, SLOT (reset()));

   setLayout(mainLayout);
   setWindowTitle(tr("Graph2019"));
}

void MainWidget::load() {
   QString file = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("Object files (*.obj *.ply *.stl)"), nullptr, QFileDialog::Option::DontUseNativeDialog);
   this->ge->initObject(file.toUtf8().constData());
   oglWidget_->repaint();
}

void MainWidget::save() {
   QString file = QFileDialog::getSaveFileName(this, tr("Open"), QDir::currentPath(), tr("Object files (*.obj *.ply *.stl)"), nullptr, QFileDialog::Option::DontUseNativeDialog);
   this->ge->saveObject(file.toUtf8().constData());
}

void MainWidget::applyLoop() {
   std::cout << "applying loop schema" << std::endl;
   ge->applyLoopSchema(box_a->value(), box_b->value(), box_c->value(), box_d->value());
   oglWidget_->repaint();
}

void MainWidget::applyButterfly() {
   std::cout << "applying butterfly schema" << std::endl;
   ge->applyButterflySchema(box_a->value(), box_b->value(), box_c->value());
   oglWidget_->repaint();
}

void MainWidget::applySquareroot() {
   std::cout << "applying squareroot 3 schema" << std::endl;
   ge->applySquarerootSchema();
   oglWidget_->repaint();
}

void MainWidget::applyCatmull() {
   std::cout << "applying catmull clark schema" << std::endl;
   ge->applyCatmullSchema();
   oglWidget_->repaint();
}

void MainWidget::reset() {
   ge->reset();
   oglWidget_->repaint();
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
