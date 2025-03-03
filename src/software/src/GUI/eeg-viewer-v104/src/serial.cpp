#include "serial.h"

Serial::Serial(QObject *parent)
    : QThread{parent}
{

}

bool Serial::connect(QString nameCOM, quint32 baud)
{
    serial.setPortName(nameCOM);
    serial.open(QIODevice::ReadWrite);
    if (!serial.isOpen())
    {
        emit connectionError(tr("Can't open %1, error code %2")
                               .arg(nameCOM).arg(serial.error()),Qt::red);
        return false;
    }
    else
    {
        serial.setBaudRate(baud);
        serial.setDataBits(QSerialPort::Data8);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setParity(QSerialPort::NoParity);
        isConnected = true;
        if (!isRunning())
            start();
        return true;
    }
}

void Serial::disconnect()
{
    isConnected = false;
    terminate();
    serial.close();
}

void Serial::transmitData(const QList<QString> &dataList)
{
    QByteArray data;
    for (const QString &str : dataList) {
        data.append(str.toUtf8());
    }
    if (serial.isOpen() && serial.isWritable())
        serial.write(data);
}

bool Serial::connectionState()
{
    return isConnected;
}

QList<QSerialPortInfo> Serial::getAvailablePorts()
{
    return QSerialPortInfo::availablePorts();
}

QList<int> Serial::getStandardBaudRates()
{
    return QSerialPortInfo::standardBaudRates();
}

void Serial::getSerialData()
{
    QList<QByteArray> buf;
    buf.append(serial.readAll());
    emit receivedData(buf);
}

void Serial::run()
{
    while (isConnected)
    {
        if(serial.waitForReadyRead(1000))
            getSerialData();
        else
            emit timeout(tr("Wait read request timeout %1")
                             .arg(QTime::currentTime().toString()), Qt::darkMagenta);
    }
}
