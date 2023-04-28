#include "mainwindow.h"

#include "my_button.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MyButton * b = new MyButton(&w);
//    b->setGeometry(100, 100, b->width(), b->height());
    MyButton * b2 = new MyButton(&w);
    MyButton * b3 = new MyButton(&w);
    w.show();
    return a.exec();
}
