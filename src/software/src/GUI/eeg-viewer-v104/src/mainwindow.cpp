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

    serialDevice = new Serial();
    // serialThread = new QThread(this);
    // serialDevice->moveToThread(serialThread);
    // serialThread->start();

    notchFilterChannel1 = new Filter();
    notchFilterChannel2 = new Filter();
    lowPassFilterChannel1 = new Filter();
    lowPassFilterChannel2 = new Filter();

    /* List all available COM */
    QList<QSerialPortInfo> ports = serialDevice->getAvailablePorts();
    QList<QString> strPorts;
    for (int i = 0; i < ports.size(); i++)
    {
        strPorts.append(ports.at(i).portName());
    }
    ui->cBSerialName->addItems(strPorts);

    ui->cBSerialBaud->setEditable(true);

    /* List all baudrates supported */
    QList<qint32> bauRates = serialDevice->getStandardBaudRates();
    QList<QString> strBaudRates;
    for (int i = 0; i < bauRates.size(); i++)
    {
        strBaudRates.append(QString::number(bauRates.at(i)));
    }
    ui->cBSerialBaud->addItems(strBaudRates);

    dataParser = new DataParser();

    rawEegChannel1 = new Plotter(this);
    rawEegChannel2 = new Plotter(this);
    filteredEegChannel1 = new Plotter(this);
    filteredEegChannel2 = new Plotter(this);

    QVBoxLayout *layout1 = new QVBoxLayout(ui->gBRawEegChannel1);
    layout1->addWidget(rawEegChannel1);

    QVBoxLayout *layout2 = new QVBoxLayout(ui->gBRawEegChannel2);
    layout2->addWidget(rawEegChannel2);

    QVBoxLayout *layout3 = new QVBoxLayout(ui->gBFilteredEegChannel1);
    layout3->addWidget(filteredEegChannel1);

    QVBoxLayout *layout4 = new QVBoxLayout(ui->gBFilteredEegChannel2);
    layout4->addWidget(filteredEegChannel2);

    connect(serialDevice, &Serial::receivedData, dataParser, &DataParser::packetDectection, Qt::QueuedConnection);
    connect(dataParser, &DataParser::valueOfEegChannels, this, &MainWindow::convertValueEegChannels, Qt::QueuedConnection);

    // Timer chung update tất cả Plotter
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [=]() {
        rawEegChannel1->processBuffer();
        rawEegChannel2->processBuffer();
        filteredEegChannel1->processBuffer();
        filteredEegChannel2->processBuffer();
    });
    updateTimer->start(10); // 50fps mượt mà
}

MainWindow::~MainWindow()
{
    serialThread->quit();
    serialThread->wait();
    delete serialDevice;
    delete serialThread;
    delete ui;
}

void MainWindow::on_pBtnSerialConnect_clicked()
{
    if (ui->pBtnSerialConnect->text() == "Connect")
    {
        if(serialDevice->connectPort(QSerialPortInfo::availablePorts().at(ui->cBSerialName->currentIndex()).portName(), ui->cBSerialBaud->currentText().toUInt()))
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
        serialDevice->disconnectPort();
        ui->pBtnSerialConnect->setText("Connect");
        ui->pBtnSerialConnect->setStyleSheet("QPushButton {color: rgb(0, 85, 0)}");
        ui->cBSerialName->setDisabled(false);
        ui->cBSerialBaud->setDisabled(false);
        ui->tBrTransceivedData->append("<span style=\"color: rgb(170,0,0);\">"
                                       + ui->cBSerialName->currentText()
                                       + " DISCONNECTED" + "</span>");

        // === Clear dữ liệu Plotter ===
        rawEegChannel1->clearData();
        rawEegChannel2->clearData();
        filteredEegChannel1->clearData();
        filteredEegChannel2->clearData();

        notchFilterChannel1->reset();
        notchFilterChannel2->reset();
        lowPassFilterChannel1->reset();
        lowPassFilterChannel2->reset();
    }
}

void MainWindow::updateSerialDeviceList()
{
    QList<QSerialPortInfo> devices = serialDevice->getAvailablePorts();
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

float MainWindow::convertADCtoVoltage_AD7768(int32_t data, float vref, int32_t gain, int32_t offset)
{
    const double scale_factor = static_cast<double>(4194300) / static_cast<double>(1ULL << 42); // 4,194,300 / 2^42
    const double gain_factor = static_cast<double>(gain) / 4.0;
    const double k = gain_factor * scale_factor;

    double vin = ((static_cast<double>(data) / k) + static_cast<double>(offset)) * vref / (3.0 * static_cast<double>(1 << 21));

    return static_cast<float>(vin);
}

void MainWindow::convertValueEegChannels(const float &channel1, const float &channel2)
{
    // // Chuyển sang điện áp (nếu muốn)
    // float vref = 2.5f;    // Ref voltage
    // int32_t gain = 0x555555; // Default factory gain
    // int32_t offset = 0;      // Offset
    // float voltage1 = convertADCtoVoltage_AD7768(raw1, vref, gain, offset);
    // float voltage2 = convertADCtoVoltage_AD7768(raw2, vref, gain, offset);

    float temp1 = notchFilterChannel1->applyFilter(FilterType::Notch, channel1);
    float temp2 = notchFilterChannel2->applyFilter(FilterType::Notch, channel2);

    float fVoltage1 = lowPassFilterChannel1->applyFilter(FilterType::LowPass, temp1);
    float fVoltage2 = lowPassFilterChannel2->applyFilter(FilterType::LowPass, temp2);


    // Thêm vào Plotter
    rawEegChannel1->addDataToBuffer(channel1);
    rawEegChannel2->addDataToBuffer(channel2);
    filteredEegChannel1->addDataToBuffer(fVoltage1);
    filteredEegChannel2->addDataToBuffer(fVoltage2);
}

