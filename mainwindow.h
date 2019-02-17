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

    void on_pushButtonControl0_clicked(bool checked);

    void on_pushButtonControl0_ON_clicked(bool checked);

    void on_pushButtonControl0_OFF_clicked(bool checked);

    void on_pushButtonControl1_ON_clicked(bool checked);

    void on_pushButtonControl1_OFF_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

    void on_enDebugger_clicked(bool checked);

    void on_pushButtonControl2_ON_clicked();

    void on_pushButtonControl2_OFF_clicked();

    void on_pushButtonControl3_ON_clicked();

    void on_pushButtonControl3_OFF_clicked();

    void on_pushButton_2_clicked(bool checked);

    void on_pushButton_2_clicked();

private:
    void fillPortsInfo();
    bool openSerialPort();
    void closeSerialPort();
    void showStatusMessage(const QString &message);
    void updateButtons();
    uint8_t getState();
    QString write(QString string);


    Ui::MainWindow *ui;

    QSerialPort *serial;
    QString ComPorName;
    bool portFound;
    QLabel *status;

    bool relayState[2];
    bool isConnected;
    uint8_t lastState;

protected:
  void keyPressEvent(QKeyEvent * event);

};

#endif // MAINWINDOW_H
