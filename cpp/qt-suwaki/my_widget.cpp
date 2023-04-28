#include "my_widget.h"

#include <QPainter>

myWidget::myWidget(QWidget *parent)
    : QWidget(parent), color(0, 0, 0)
{

}

void myWidget::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(color));
    painter.drawRect(rect());
}

void myWidget::red_changed(int value)
{
    color.setRed(value);
    repaint();
}

void myWidget::green_changed(int value)
{
    color.setGreen(value);
    repaint();
}

void myWidget::blue_changed(int value)
{
    color.setBlue(value);
    repaint();
}
