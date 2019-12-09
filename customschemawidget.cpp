#include "customschemawidget.h"
#include <QPainter>
#include <QtWidgets/QtWidgets>
#include <QLineEdit>
#include <string>
#include <QGraphicsItem>

#include <iostream>
#include <map>
#include "graph_engine.h"

#include "json.hpp"
#include "jsonio.h"

CustomSchemaWidget::CustomSchemaWidget(QWidget *parent, json& items, std::string appr, std::string name)
    : QDialog(parent)
{

    this->scheme_name = name;
    this->scheme_key = appr;

    this->selected_item = items[appr][name];
    this->custom_schemes = items;

    label_name = new QLabel(this);
    label_name->setText("Scheme name:");
    label_name->move(16, 16);

    line_name = new QLineEdit(scheme_name.c_str(), this);
    line_name->setReadOnly(true);
    line_name->setFixedWidth(256);
    line_name->setFixedHeight(32);
    line_name->move(16, 48);

    this->ofsX = 120;
    this->ofsY = 220;
    this->sc = 60;
    weights = getWeightBoxes();

    save_button = new QPushButton("Save", this);
    save_button->move(300, 16);

    label_save = new QLabel("", this);
    label_save->setFixedWidth(200);
    label_save->move(300, 48);
     connect(save_button, SIGNAL (released()), this, SLOT (saveCustomScheme()));

    setWindowTitle(tr("Custom schemas"));
    setFixedHeight(400);
    setFixedWidth(600);
}

QDoubleSpinBox* CustomSchemaWidget::getNewDoubleSpinBoxForWeight() {
    QDoubleSpinBox* spinbox = new QDoubleSpinBox(this);
    spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinbox->setSingleStep(0.00001);
    spinbox->setRange(-1.0, 1.0);
    spinbox->setDecimals(5);
    spinbox->setFixedWidth(70);
    return spinbox;
}

void CustomSchemaWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);

    if (!scheme_name.find("Butterfly")) {
      auto top_0 = QPoint(0 * sc + ofsX, 0 * sc + ofsY);
      auto top_24 = QPoint(1 * sc + ofsX, 0 * sc + ofsY);
      auto top_44 = QPoint(2 * sc + ofsX, 0 * sc + ofsY);

      auto middle_0 = QPoint(0.5 * sc + ofsX, 1 * sc + ofsY);
      auto middle_22 = QPoint(1.5 * sc + ofsX, 1 * sc + ofsY);

      auto bottom_0 = QPoint(0 * sc + ofsX, 2 * sc + ofsY);
      auto bottom_24 = QPoint(1 * sc + ofsX, 2 * sc + ofsY);
      auto bottom_44 = QPoint(2 * sc + ofsX, 2 * sc + ofsY);
      painter.drawLine(top_0,top_44);

      painter.drawLine(top_0,middle_0);
      painter.drawLine(middle_0,top_24);
      painter.drawLine(top_24,middle_22);
      painter.drawLine(middle_22,top_44);

      painter.drawLine(middle_0,middle_22);

      painter.drawLine(bottom_0,middle_0);
      painter.drawLine(middle_0,bottom_24);
      painter.drawLine(bottom_24,middle_22);
      painter.drawLine(middle_22,bottom_44);
      painter.drawLine(bottom_0,bottom_44);

    } else if (!scheme_name.find("Loop")) {
      auto top_24 = QPoint(1 * sc + ofsX, 0 * sc + ofsY);
      auto middle_0 = QPoint(0.5 * sc + ofsX, 1 * sc + ofsY);
      auto middle_22 = QPoint(1.5 * sc + ofsX, 1 * sc + ofsY);
      auto bottom_24 = QPoint(1 * sc + ofsX, 2 * sc + ofsY);

      painter.drawLine(top_24,middle_0);
      painter.drawLine(middle_0,middle_22);
      painter.drawLine(middle_22, top_24);

      painter.drawLine(middle_0,bottom_24);
      painter.drawLine(bottom_24,middle_22);
    } else if (!scheme_name.find("CatmullClark")) {
      auto top_a = QPoint(0 * sc + ofsX, 0 * sc + ofsY);
      auto top_b = QPoint(0 * sc + ofsX, 1 * sc + ofsY);
      auto top_c = QPoint(1 * sc + ofsX, 1 * sc + ofsY);
      auto top_d = QPoint(1 * sc + ofsX, 0 * sc + ofsY);

      painter.drawLine(top_a, top_b);
      painter.drawLine(top_b, top_c);
      painter.drawLine(top_c, top_d);
      painter.drawLine(top_d, top_a);
    } else if (!scheme_name.find("SquareRoot3")) {
      auto top_a = QPoint(0.5 * sc + ofsX, 0 * sc + ofsY);
      auto top_b = QPoint(0 * sc + ofsX, 1 * sc + ofsY);
      auto top_c = QPoint(1 * sc + ofsX, 1 * sc + ofsY);

      painter.drawLine(top_a, top_b);
      painter.drawLine(top_b, top_c);
      painter.drawLine(top_c, top_a);
    }
}

std::map<char, QDoubleSpinBox*> CustomSchemaWidget::getWeightBoxes() {
  std::map<char, QDoubleSpinBox*> weights = std::map<char, QDoubleSpinBox*>();
  
  int mgn = 16;
  int width;
  int height;
  if (!scheme_name.find("Butterfly")) {
    QDoubleSpinBox* a_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* b_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c_spinbox = getNewDoubleSpinBoxForWeight();
    width = a_spinbox->width();
    height = a_spinbox->height();
    a_spinbox->setValue(selected_item["weights"]["A"]);
    b_spinbox->setValue(selected_item["weights"]["B"]);
    c_spinbox->setValue(selected_item["weights"]["C"]);
    a_spinbox->move(0.5 * sc + ofsX - width, + 1 * sc + ofsY - height / 2.0);
    b_spinbox->move(1 * sc + ofsX - width / 2.0, + 0 * sc + ofsY - height);
    c_spinbox->move(0 * sc + ofsX - width, + 0 * sc + ofsY - height / 2.0);
    weights['A'] = a_spinbox;
    weights['B'] = b_spinbox;
    weights['C'] = c_spinbox;

    QDoubleSpinBox* a2 = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* b2 = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c2 = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c3 = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c4 = getNewDoubleSpinBoxForWeight();
    a2->setValue(selected_item["weights"]["A"]);
    b2->setValue(selected_item["weights"]["B"]);
    c2->setValue(selected_item["weights"]["C"]);
    c3->setValue(selected_item["weights"]["C"]);
    c4->setValue(selected_item["weights"]["C"]);

    a2->move(1.5 * sc + ofsX, + 1 * sc + ofsY - height / 2.0);
    b2->move(1 * sc + ofsX - width / 2.0, + 2 * sc + ofsY);
    c2->move(2 * sc + ofsX, + 0 * sc + ofsY - height / 2.0);
    c3->move(0 * sc + ofsX - width, + 2 * sc + ofsY - height / 2.0);
    c4->move(2 * sc + ofsX, + 2 * sc + ofsY - height / 2.0);

  } else if (!scheme_name.find("Loop")) {
    QDoubleSpinBox* a_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* b_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* d_spinbox = getNewDoubleSpinBoxForWeight();
    width = a_spinbox->width();
    height = a_spinbox->height();
    a_spinbox->setValue(selected_item["weights"]["A"]);
    b_spinbox->setValue(selected_item["weights"]["B"]);
    c_spinbox->setValue(selected_item["weights"]["C"]);
    d_spinbox->setValue(selected_item["weights"]["D"]);
    a_spinbox->move(1 * sc + ofsX - width / 2.0, + 0 * sc + ofsY - height);
    b_spinbox->move(1 * sc + ofsX - width / 2.0, + 2 * sc + ofsY);
    c_spinbox->move(0.5 * sc + ofsX - width, + 1 * sc + ofsY);
    d_spinbox->move(1.5 * sc + ofsX, + 1 * sc + ofsY);
    weights['A'] = a_spinbox;
    weights['B'] = b_spinbox;
    weights['C'] = c_spinbox;
    weights['D'] = d_spinbox;
  } else if (!scheme_name.find("CatmullClark")) {
    QDoubleSpinBox* a_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* b_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* d_spinbox = getNewDoubleSpinBoxForWeight();
    width = a_spinbox->width();
    height = a_spinbox->height();
    a_spinbox->setValue(selected_item["weights"]["A"]);
    b_spinbox->setValue(selected_item["weights"]["B"]);
    c_spinbox->setValue(selected_item["weights"]["C"]);
    d_spinbox->setValue(selected_item["weights"]["D"]);
    a_spinbox->move(0 * sc + ofsX - width, + 0 * sc + ofsY);
    b_spinbox->move(0 * sc + ofsX - width, + 1 * sc + ofsY);
    c_spinbox->move(1 * sc + ofsX, + 1 * sc + ofsY);
    d_spinbox->move(1 * sc + ofsX, + 0 * sc + ofsY);
    weights['A'] = a_spinbox;
    weights['B'] = b_spinbox;
    weights['C'] = c_spinbox;
    weights['D'] = d_spinbox;
  } else if (!scheme_name.find("SquareRoot3")) {
    QDoubleSpinBox* a_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* b_spinbox = getNewDoubleSpinBoxForWeight();
    QDoubleSpinBox* c_spinbox = getNewDoubleSpinBoxForWeight();
    width = a_spinbox->width();
    height = a_spinbox->height();
    a_spinbox->setValue(selected_item["weights"]["A"]);
    b_spinbox->setValue(selected_item["weights"]["B"]);
    c_spinbox->setValue(selected_item["weights"]["C"]);
    a_spinbox->move(0.5 * sc + ofsX - width / 2.0, + 0 * sc + ofsY - height);
    b_spinbox->move(0 * sc + ofsX - width, + 1 * sc + ofsY);
    c_spinbox->move(1 * sc + ofsX, + 1 * sc + ofsY);
    weights['A'] = a_spinbox;
    weights['B'] = b_spinbox;
    weights['C'] = c_spinbox;
  }
  
  return weights;
}

void CustomSchemaWidget::saveCustomScheme() {

    for(std::map<char, QDoubleSpinBox*>::iterator iter = weights.begin(); iter != weights.end(); ++iter) {
      char c = iter->first;
      QDoubleSpinBox* box = iter->second;
      std::string x = std::string(1, c);
      selected_item["weights"][x] = box->value();
    }

    custom_schemes[scheme_key][scheme_name] = selected_item;

    JsonHandler::WriteJson("data.json", custom_schemes);
    label_save->setText("Saved successfully.");
}

CustomSchemaWidget::~CustomSchemaWidget()
{

}
