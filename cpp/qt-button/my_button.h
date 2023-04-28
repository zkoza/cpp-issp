#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
  public:
    MyButton(QWidget* parent = nullptr);

  private:
    int counter = 0;
    QWidget * parent;
    void jump();

  public slots:
    void on_clicked();
};

#endif // MYBUTTON_H
