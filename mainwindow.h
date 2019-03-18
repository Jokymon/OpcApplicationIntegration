#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

struct UA_ServerConfig;
struct UA_Server;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateProgressBarValue(int value);

private slots:
    void blink();
    void opcMessagePump();

    void on_statusCheckbox_stateChanged(int arg1);

private:
    void addBoolVariable();
    void setBoolVariableValue(bool value);

    void addCallbackVariable();

    void addObject();

private:
    QTimer blinkTimer;
    QTimer messagePumpTimer;
    Ui::MainWindow *ui;

    UA_ServerConfig *config;
    UA_Server *server;
};

#endif // MAINWINDOW_H
