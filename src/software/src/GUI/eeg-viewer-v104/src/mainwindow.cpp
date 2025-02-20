#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEvent>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->cBSerialName->installEventFilter(this);

    /* List all available COM */
    QList<QSerialPortInfo> ports = serialDevice.getAvailablePorts();
    QList<QString> strPorts;
    for (int i = 0; i < ports.size(); i++)
    {
        strPorts.append(ports.at(i).portName());
    }
    ui->cBSerialName->addItems(strPorts);

    /* List all baudrates supported */
    QList<qint32> bauRates = serialDevice.getStandardBaudRates();
    QList<QString> strBaudRates;
    for (int i = 0; i < bauRates.size(); i++)
    {
        strBaudRates.append(QString::number(bauRates.at(i)));
    }
    ui->cBSerialBaud->addItems(strBaudRates);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pBtnSerialConnect_clicked()
{
    if (ui->pBtnSerialConnect->text() == "Connect")
    {
        if(serialDevice.connect(QSerialPortInfo::availablePorts().at(ui->cBSerialName->currentIndex()).portName(), ui->cBSerialBaud->currentText().toUInt()))
        {
            ui->pBtnSerialConnect->setText("Disconnect");
            ui->pBtnSerialConnect->setStyleSheet("QPushButton {color: rgb(170, 0, 0)}");
            ui->cBSerialName->setDisabled(true);
            ui->cBSerialBaud->setDisabled(true);
            ui->tBrTransceivedData->append("<span style=\"color: rgb(0,85,0);\">"
                                           + ui->cBSerialName->currentText() + " CONNECTED at baudrate "
                                           + ui->cBSerialBaud->currentText() + "</span>");
        }
        else
        {
            ui->tBrTransceivedData->append("<span style=\"color: rgb(255, 178, 44);\">"
                                           + ui->cBSerialName->currentText()
                                           + " connecting ERROR "
                                           + "</span>");
        }
    }
    else if ((ui->pBtnSerialConnect->text() == "Disconnect"))
    {
        serialDevice.disconnect();
        ui->pBtnSerialConnect->setText("Connect");
        ui->pBtnSerialConnect->setStyleSheet("QPushButton {color: rgb(0, 85, 0)}");
        ui->cBSerialName->setDisabled(false);
        ui->cBSerialBaud->setDisabled(false);
        ui->tBrTransceivedData->append("<span style=\"color: rgb(170,0,0);\">"
                                       + ui->cBSerialName->currentText()
                                       + " DISCONNECTED" + "</span>");
    }
}

void MainWindow::updateSerialDeviceList()
{
    QList<QSerialPortInfo> devices = serialDevice.getAvailablePorts();
    QStringList portNames;
    static QStringList portNamesOld;

    foreach (auto item, devices)
        portNames.append(item.portName());

    if ((devices.count() >= 1) &&
        ((!QSet<QString>(portNames.begin(), portNames.end()).intersects(QSet<QString>(portNamesOld.begin(), portNamesOld.end()))) ||
         (portNames.count() != portNamesOld.count())))
    {
        ui->cBSerialName->clear();

        for (int i = 0; i < devices.count(); ++i)
        {
            ui->cBSerialName->addItem("(" + devices.at(i).portName() + ")" + " " + devices.at(i).description());
        }
    }
    else if ((devices.count() < 1) && !ui->cBSerialName->itemText(0).startsWith("No COM devices"))
    {
        ui->cBSerialName->clear();
        ui->cBSerialName->addItem("No serial devices detected :(");
        ui->cBSerialName->setCurrentIndex(ui->cBSerialName->count() - 1);
    }


    portNamesOld = portNames;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->cBSerialName && event->type() == QEvent::MouseButtonPress)
    {
        updateSerialDeviceList();
        return false;
    }
        return QMainWindow::eventFilter(obj, event);
}
