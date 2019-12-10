#include <QtWidgets/QtWidgets>
#include "mainwidget.h"
#include "noisewidget.h"
#include "json.hpp"
#include "jsonio.h"

// Constructor for main widget
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
   button_load = new QPushButton(tr("Load"));
   button_save = new QPushButton(tr("Save"));
   button_loop = new QPushButton(tr("Loop"));
   button_butterfly = new QPushButton(tr("Butterfly"));
   button_squareroot = new QPushButton(tr("Square root 3"));
   button_catmull = new QPushButton(tr("Catmull-Clark"));
   button_doosabin = new QPushButton(tr("Doo-Sabin"));

   list_custom_schemes = new QListWidget();
   initCustomSchemaList();
   button_custom = new QPushButton(tr("Modify custom"));
   button_custom_apply = new QPushButton(tr("Apply custom"));
   button_noise = new QPushButton(tr("Generate Noise"));


   button_reset = new QPushButton("Reset");

   oglWidget_ = new OGLWidget();
   ge = new graph_engine();
   oglWidget_->setGraphEngine(ge);
   ge->initObject("test.obj");

//   noiseWidget = new NoiseWidget;
//   noiseWidget->setModal(true);
//   noiseWidget->setGraphEngine(ge);


   QGridLayout *mainLayout = new QGridLayout;

   mainLayout->addWidget(button_load, 0, 0, 1, 3);
   mainLayout->addWidget(button_save, 0, 3, 1, 3);


   mainLayout->addWidget(button_loop, 1, 0);
   mainLayout->addWidget(button_butterfly, 1, 1);
   mainLayout->addWidget(button_squareroot, 1, 2);
   mainLayout->addWidget(button_catmull, 1, 3);
   mainLayout->addWidget(button_doosabin, 1, 4);

   enableAllSchemes(false);

   mainLayout->addWidget(list_custom_schemes, 2, 0, 1, 0);
   mainLayout->addWidget(button_custom, 3, 0, 1, 3);
   mainLayout->addWidget(button_custom_apply, 3, 3, 1, 2);

   mainLayout->addWidget(button_noise, 4, 1, 1, 3);

   mainLayout->addWidget(oglWidget_, 5, 0, 1, 5);
   mainLayout->addWidget(button_reset, 6, 1, 1, 3);

   connect(button_load, SIGNAL (released()), this, SLOT (load()));
   connect(button_save, SIGNAL (released()), this, SLOT (save()));
   connect(button_loop, SIGNAL (released()), this, SLOT (applyLoop()));
   connect(button_butterfly, SIGNAL (released()), this, SLOT (applyButterfly()));
   connect(button_squareroot, SIGNAL (released()), this, SLOT (applySquareroot()));
   connect(button_catmull, SIGNAL (released()), this, SLOT (applyCatmull()));
   connect(button_doosabin, SIGNAL (released()), this, SLOT (applyDooSabin()));
   connect(button_reset, SIGNAL (released()), this, SLOT (reset()));

   connect(button_noise, SIGNAL (released()), this, SLOT (showNoiseGenerator()));
   connect(button_custom, SIGNAL (released()), this, SLOT (showCustomSchemaModifier()));
   connect(button_custom_apply, SIGNAL (released()), this, SLOT (applyCustom()));

   setLayout(mainLayout);
   setWindowTitle(tr("Graph2019"));
}

void MainWidget::load() {
   enableAllSchemes(true);
   QString file = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("Object files (*.obj *.ply *.stl)"), nullptr, QFileDialog::Option::DontUseNativeDialog);
   this->ge->initObject(file.toUtf8().constData());
   enableAllSchemes(false);
   oglWidget_->repaint();
}

void MainWidget::save() {
   QString file = QFileDialog::getSaveFileName(this, tr("Open"), QDir::currentPath(), tr("Object files (*.obj *.ply *.stl)"), nullptr, QFileDialog::Option::DontUseNativeDialog);
   this->ge->saveObject(file.toUtf8().constData());
}

void MainWidget::applyLoop(float a, float b, float c, float d) {
   std::cout << "applying loop schema" << std::endl;
   ge->applyLoopSchema(a, b, c, d);
   oglWidget_->repaint();
}

void MainWidget::applyButterfly(float a, float b, float c) {
   std::cout << "applying butterfly schema" << std::endl;
   ge->applyButterflySchema(a, b, c);
   oglWidget_->repaint();
}

void MainWidget::applySquareroot(float a, float b, float c) {
   std::cout << "applying squareroot 3 schema" << std::endl;
   ge->applySquarerootSchema(a, b, c);
   oglWidget_->repaint();
}

void MainWidget::applyCatmull(float a, float b, float c, float d) {
   std::cout << "applying catmull clark schema" << std::endl;
   ge->applyCatmullSchema(a, b, c, d);
   oglWidget_->repaint();
}

void MainWidget::applyDooSabin() {
    std::cout << "applying Doo Sabin schema" << std::endl;
    ge->applyDooSabinSchema();
    oglWidget_->repaint();
}

void MainWidget::reset() {
   ge->reset();
   oglWidget_->repaint();
}

void MainWidget::showNoiseGenerator() {
    noiseWidget = new NoiseWidget(this, ge);
    ge->initNoiseGenerator();
    noiseWidget->setModal(true);
    noiseWidget->exec();
}

void MainWidget::enableAllSchemes(bool isEnabled) {
    if(!ge->isMeshObjectQuadratic()) {
        button_catmull->setEnabled(isEnabled);
        button_doosabin->setEnabled(isEnabled);
    } else {
        button_loop->setEnabled(isEnabled);
        button_butterfly->setEnabled(isEnabled);
        button_squareroot->setEnabled(isEnabled);
    }
}

void MainWidget::showCustomSchemaModifier() {
  int row = list_custom_schemes->currentRow();
  int i = 0;
  for (const auto& item : custom_schemes.items())
  {
        for (const auto& val : item.value().items())
        {
            if (i == row) {
              selected_custom_scheme_name = val.key();
              selected_custom_scheme_key = item.key();
            }
            ++i;
        }
  }
    customSchemaWidget = new CustomSchemaWidget(custom_schemes, this, selected_custom_scheme_key, selected_custom_scheme_name);
    customSchemaWidget->setModal(true);
    customSchemaWidget->exec();
    custom_schemes = JsonHandler::ReadJson("data.json");
}

void MainWidget::applyCustom() {
   int row = list_custom_schemes->currentRow();
   int i = 0;
   json selected_custom_json;
    for (const auto& item : custom_schemes.items())
   {
        for (const auto& val : item.value().items())
        {
            if (i == row) {
              selected_custom_scheme_key = item.key();
              selected_custom_scheme_name = val.key();
              selected_custom_json = custom_schemes[selected_custom_scheme_key][selected_custom_scheme_name];
              selected_custom_json["name"] = selected_custom_scheme_name;
            }
            ++i;
        }
   }

   std::transform(selected_custom_scheme_name.begin(), selected_custom_scheme_name.end(), selected_custom_scheme_name.begin(),
    [](unsigned char c){ return std::tolower(c); });
   float a, b, c, d, e, f;
   a = selected_custom_json["weights"]["A"];
   b = selected_custom_json["weights"]["B"];

   if (!selected_custom_scheme_name.find("butterfly")) {
   c = selected_custom_json["weights"]["C"];
        applyButterfly(a, b, c);
   } else if (!selected_custom_scheme_name.find("loop")) {
     c = selected_custom_json["weights"]["C"];
     d = selected_custom_json["weights"]["D"];
    applyLoop(a, b, c, d);
   } else if (!selected_custom_scheme_name.find("catmullclark")) {
     c = selected_custom_json["weights"]["C"];
     d = selected_custom_json["weights"]["D"];
     applyCatmull(a, b, c, d);
   } else if (!selected_custom_scheme_name.find("squareroot3")) {
     c = selected_custom_json["weights"]["C"];
     applySquareroot(a, b, c);
   }

}

void MainWidget::initCustomSchemaList() {
    list_custom_schemes->clear();
   custom_schemes = JsonHandler::ReadJson("data.json");
   for (const auto& item : custom_schemes.items())
   {
        for (const auto& val : item.value().items())
        {
            QListWidgetItem* item = new QListWidgetItem();
            QString qs = QString(val.key().c_str());
            item->setText(qs);
            list_custom_schemes->addItem(item);
        }
   }
   list_custom_schemes->setCurrentRow(0);
   list_custom_schemes->setFixedHeight(60);
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
