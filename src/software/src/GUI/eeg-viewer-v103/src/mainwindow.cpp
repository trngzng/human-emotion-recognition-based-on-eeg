#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSerialDeviceList()
{
    QList<QSerialPortInfo> devices = serial.getAvailiblePorts();
    QList<QString> portNames;
    static QList<QString> portNamesOld;

    foreach (auto item, devices)
        portNames.append(item.portName());

    if ((devices.count() >= 1) && (!(portNames.toSet().intersects(portNamesOld.toSet())) || (portNames.count() != portNamesOld.count())))
    {
        ui->cB_serialName->clear();

        for (auto i = 0; i < devices.count(); ++i)
        {
            ui->cB_serialName->addItem("(" + devices.at(i).portName() + ")" + " " + devices.at(i).description());
        }
    }
    else if ((devices.count() < 1) && !ui->cB_serialName->itemText(0).startsWith("No COM devices"))
    {
        ui->cB_serialName->clear();
        ui->cB_serialName->addItem("No serial devices detected :(");
        ui->cB_serialName->setCurrentIndex(ui->cB_serialName->count() - 1);
    }

    portNamesOld = portNames;
}
