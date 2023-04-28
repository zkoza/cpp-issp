#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QColor>

class myWidget : public QWidget
{
    QColor color;
  public:
    myWidget(QWidget* parent);
    virtual void paintEvent(QPaintEvent *event) override;
  public slots:
    void red_changed(int value);
    void green_changed(int value);
    void blue_changed(int value);

};

#endif // MYWIDGET_H
