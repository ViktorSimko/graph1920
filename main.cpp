/****************************************************************
**
** Qt tutorial 1
**
****************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include "mainwidget.h"

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    
    MainWidget w;
    w.setFixedSize(500, 500);
    w.show();
    return a.exec();
}
