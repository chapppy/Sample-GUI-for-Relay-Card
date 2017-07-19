#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtCore/QtGlobal>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_pushButtonConnect_clicked();

    void on_pushButtonControl0_clicked(bool checked);

    void on_pushButtonControl0_ON_clicked(bool checked);

    void on_pushButtonControl0_OFF_clicked(bool checked);

    void on_pushButtonControl1_ON_clicked(bool checked);

    void on_pushButtonControl1_OFF_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

    void on_enDebugger_clicked(bool checked);

private:
    void showPortInfo(int idx);
    void fillPortsInfo();
    void openSerialPort();
    void closeSerialPort();
    void showStatusMessage(const QString &message);
    void updateButtons();
    uint8_t getState();
    QString write(QString string);


    Ui::MainWindow *ui;

    QSerialPort *serial;
    QLabel *status;

    bool relayState[2];
    bool isConnected;

};

#endif // MAINWINDOW_H
