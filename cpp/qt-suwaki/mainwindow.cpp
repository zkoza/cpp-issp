#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->redSlider, &QSlider::valueChanged, ui->widget, &myWidget::red_changed);
    connect(ui->greenSlider, &QSlider::valueChanged, ui->widget, &myWidget::green_changed);
    connect(ui->blueSlider, &QSlider::valueChanged, ui->widget, &myWidget::blue_changed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

