#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QTime>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = nullptr);

    bool connectPort(const QString &nameCOM, quint32 baud);
    void disconnectPort();
    void transmitData(const QList<QString> &data);
    bool connectionState();
    QList<QSerialPortInfo> getAvailablePorts();
    QList<int> getStandardBaudRates();

signals:
    void receivedData(const QList<QByteArray> data);
    void connectionError(const QString &msg, const Qt::GlobalColor color);
    void timeout(const QString &msg, const Qt::GlobalColor color);

private slots:
    void getSerialData();

private:
    QSerialPort *serial;
    bool isConnected = false;
};

#endif // SERIAL_H
