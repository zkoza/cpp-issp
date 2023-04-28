#include "my_button.h"
#include <qrandom.h>

MyButton::MyButton(QWidget* parent) : QPushButton("", parent), parent{parent}
{
    setText(QString("%1").arg(counter));
    connect(this, &MyButton::clicked, this, &MyButton::on_clicked);
    setStyleSheet("color:red; font-size:50px; font-weight:bold");
    setGeometry(0, 0, 2 * width(), 60);
    jump();
}

void MyButton::jump()
{
    const auto max_x = parent->width() - width();
    const auto max_y = parent->height() - height();
    const auto x = QRandomGenerator::global()->bounded(max_x);
    const auto y = QRandomGenerator::global()->bounded(max_y);
    move(x, y);
}

void MyButton::on_clicked()
{
    const int size = 3;
    int red[size] = {255, 0, 0};
    int green[size] = {0, 255, 0};
    int blue[size] = {0, 0, 255};

    counter++;
    setText(QString("%1").arg(counter));
    int idx = counter % size;
    QString s = "color:rgb(%1,%2,%3); font-size:50px; font-weight:bold";
    setStyleSheet(s.arg(red[idx]).arg(green[idx]).arg(blue[idx]));

    jump();
}
