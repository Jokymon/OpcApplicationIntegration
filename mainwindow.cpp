#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <open62541.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    config(nullptr),
    server(nullptr)
{
    ui->setupUi(this);

    blinkTimer.setInterval(500);
    blinkTimer.setSingleShot(false);
    QObject::connect(&blinkTimer, &QTimer::timeout,
                     this, &MainWindow::blink);

    messagePumpTimer.setInterval(10);
    messagePumpTimer.setSingleShot(false);
    QObject::connect(&messagePumpTimer, &QTimer::timeout,
                     this, &MainWindow::opcMessagePump);

    config = UA_ServerConfig_new_default();
    server = UA_Server_new(config);
    UA_Server_run_startup(server);

    messagePumpTimer.start();
}

MainWindow::~MainWindow()
{
    messagePumpTimer.stop();
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    delete ui;
}

void MainWindow::blink()
{
    auto checked = ui->led->isChecked();
    ui->led->setChecked(!checked);
}

void MainWindow::opcMessagePump()
{
    UA_Server_run_iterate(server, 0);
}
