#include "serial.h"
#include <QDebug>

Serial::Serial(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

bool Serial::connectPort(const QString &nameCOM, quint32 baud)
{
    serial->setPortName(nameCOM);
    serial->open(QIODevice::ReadWrite);
    if (!serial->isOpen())
    {
        emit connectionError(tr("Can't open %1, error code %2").arg(nameCOM).arg(serial->error()), Qt::red);
        return false;
    }

    serial->setBaudRate(baud);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);

    isConnected = true;

    // Kết nối tín hiệu đọc dữ liệu
    connect(serial, &QSerialPort::readyRead, this, &Serial::getSerialData);

    return true;
}

void Serial::disconnectPort()
{
    if (serial->isOpen())
    {
        isConnected = false;
        serial->clear();
        serial->close();
        disconnect(serial, &QSerialPort::readyRead, this, &Serial::getSerialData);
    }
}

void Serial::transmitData(const QList<QString> &dataList)
{
    QByteArray data;
    for (const QString &str : dataList)
    {
        data.append(str.toUtf8());
    }

    if (serial->isOpen() && serial->isWritable())
        serial->write(data);
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
    if (serial->bytesAvailable() > 0) {
        buf.append(serial->read(serial->bytesAvailable()));
    }

    emit receivedData(buf);
}
