#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QThread>

enum ParserState {
    IDLE = 0,
    START_BYTE_DETECTION,
    WAIT_SOP,
    PACKET_DETECTION,
    WAIT_VALID_CMD,
    WAIT_VALID_LEN,
    RECEIVE_PAYLOAD,
    WAIT_END_BYTES,
    INVALID_PACKET,
    CONT_WAIT_END_BYTE,
    WAIT_LAST_END_BYTE,
    FINISHED_PACKET
};


class DataParser : public QThread
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);
    ~DataParser();

public slots:
    void packetDectection(const QList<QByteArray> &inputData);

signals:
    void receivedPacket(const QByteArray &data);
    void valueOfEegChannels(const QByteArray &channel1, const QByteArray &channel2);

protected:
    void run() override;

private:
    void packetParser(const QByteArray &data);

private:
    ParserState currentState;
    uint8_t eegChannel1[4];
    uint8_t eegChannel2[4];
};

#endif // DATAPARSER_H
