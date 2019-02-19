#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    blinkTimer.setInterval(500);
    blinkTimer.setSingleShot(false);
    QObject::connect(&blinkTimer, &QTimer::timeout,
                     this, &MainWindow::blink);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::blink()
{
    auto checked = ui->led->isChecked();
    ui->led->setChecked(!checked);
}
