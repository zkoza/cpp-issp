#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTimer>

class MyWidget : public QWidget
{
    Q_OBJECT
  public:
    constexpr auto static timer_interval = 200;
    constexpr auto static num_disks = 8;
    explicit MyWidget(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);

  signals:

  public slots:
    void on_timer();
    void toggle_animation();

  private:
    int hidden = 0;
    QTimer timer;
};

#endif  // MYWIDGET_H
