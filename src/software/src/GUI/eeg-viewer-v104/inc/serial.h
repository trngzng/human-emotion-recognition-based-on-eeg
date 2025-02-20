#ifndef SERIAL_H
#define SERIAL_H

#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QTime>

class Serial : public QThread
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = nullptr);

    bool connect(QString nameCOM, quint32 baud);
    void disconnect();
    void transmitData(const QList<QString> &data);
    bool connectionState();
    QList<QSerialPortInfo> getAvailablePorts();
    QList<int> getStandardBaudRates();

signals:
    void receivedData(QList<QByteArray>);
    void connectionError(QString, const Qt::GlobalColor);
    void timeout(QString, const Qt::GlobalColor);

private slots:
    void getSerialData();

protected:
    void run();

private:
    QSerialPort serial;
    QSerialPortInfo info;
    bool isConnected = false;
    QList<QByteArray> resMessBuf;
};

#endif // SERIAL_H
