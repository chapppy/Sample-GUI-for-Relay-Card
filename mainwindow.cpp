#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QRegularExpression>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isConnected(false)
{
    ui->setupUi(this);

    //relayState = {false, false};

    serial = new QSerialPort(this);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::showPortInfo);

    fillPortsInfo();
    updateButtons();
}

MainWindow::~MainWindow()
{
  closeSerialPort();
  delete ui;
}

void MainWindow::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();

    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->vidLabel->setText(tr("Identifier: %1").arg(list.count() > 5 ? list.at(5) +" - "+ (list.count() > 6 ? list.at(6) : tr(blankString)) : tr(blankString) ));

    /*ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
   */
}

void MainWindow::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }
}

void MainWindow::openSerialPort()
{
    serial->setPortName(ui->serialPortInfoListBox->currentText());
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {
       showStatusMessage(tr("Connected to %1")
                          .arg(ui->serialPortInfoListBox->currentText()));
       isConnected = true;

       on_pushButtonControl1_ON_clicked(true);  // Activate relay1

    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));

        isConnected = false;
    }
}

void MainWindow::closeSerialPort()
{
  if (serial->isOpen())
  {
     serial->close();
     isConnected = false;
  }

  showStatusMessage(tr("Disconnected"));
}

void MainWindow::on_pushButtonConnect_clicked()
{
   if(isConnected == false)
   {
      openSerialPort();
   }
   else
   {
     closeSerialPort();
   }

   if(isConnected == true)
   {
      ui->pushButtonConnect->setText("disconnect");
   }
   else
   {
      ui->pushButtonConnect->setText("connect");
   }

   updateButtons();
}

void MainWindow::on_pushButtonControl0_clicked(bool checked)
{
  (void) checked;
  // serial->write("Disable 0\r");
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::updateButtons()
{
  if(isConnected == false)
  {
    ui->pushButtonControl0_OFF->setEnabled(false);
    ui->pushButtonControl0_ON->setEnabled(false);
    ui->pushButtonControl1_OFF->setEnabled(false);
    ui->pushButtonControl1_ON->setEnabled(false);
    ui->enDebugger->setEnabled(false);
  }
  else
  {
    uint8_t state = getState();

    ui->enDebugger->setEnabled(true);

    if((state &0x01) == 0x01)
    {
      ui->pushButtonControl0_OFF->setEnabled(false);
      ui->pushButtonControl0_ON->setEnabled(true);
    }
    else
    {
      ui->pushButtonControl0_OFF->setEnabled(true);
      ui->pushButtonControl0_ON->setEnabled(false);

    }

    if((state &0x02) == 0x02)
    {
      ui->pushButtonControl1_OFF->setEnabled(false);
      ui->pushButtonControl1_ON->setEnabled(true);
    }
    else
    {
      ui->pushButtonControl1_OFF->setEnabled(true);
      ui->pushButtonControl1_ON->setEnabled(false);
    }

  }
}

uint8_t MainWindow::getState()
{
  uint8_t returnVal = 0;

  QString response = write("Report binary\r");


  QRegularExpression re("Channels state 0x(\\w)\r");
  QRegularExpressionMatch match = re.match(response);
  if (match.hasMatch())
  {
      QString matched = match.captured(1); //
      returnVal = matched.toUInt(NULL, 16);
  }

  return returnVal;
}

QString MainWindow::write(QString string)
{
  QString response;

  serial->write(string.toStdString().c_str());
  if (serial->waitForBytesWritten(5000))
  {
    // read response
    if (serial->waitForReadyRead(5000))
    {
      QByteArray responseData = serial->readAll();
      while (serial->waitForReadyRead(10))
      responseData += serial->readAll();

      response = QString(responseData);
          //emit this->response(response);
    }
    else
    {
      //emit timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
    }

  }
  else
  {
     //emit timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
  }

  showStatusMessage(response);

  return response;
}

void MainWindow::on_pushButtonControl0_ON_clicked(bool checked)
{
  (void) checked;
  write("Disable 0\r");
  updateButtons();
}

void MainWindow::on_pushButtonControl0_OFF_clicked(bool checked)
{
  (void) checked;
  write("Enable 0\r");
  updateButtons();
}

void MainWindow::on_pushButtonControl1_ON_clicked(bool checked)
{
  (void) checked;
  write("Disable 1\r");
  updateButtons();
}

void MainWindow::on_pushButtonControl1_OFF_clicked(bool checked)
{
  (void) checked;
  write("Enable 1\r");
  updateButtons();
}

void MainWindow::on_pushButton_clicked(bool checked)
{
  (void) checked;
  fillPortsInfo();
}

void MainWindow::on_enDebugger_clicked(bool checked)
{
  if(checked == true)
  {
    write("Configure 3, OUTPP, PPDOWN\r"); //PPDOWN   PPUP
    write("Disable 3\r");
  }
  else
  {
    write("Enable 3\r");
    write("Configure 3, IN, PPNO\r");
  }
}