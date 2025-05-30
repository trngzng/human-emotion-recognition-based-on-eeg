#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QByteArray>
#include <QMutex>

class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);

signals:
    void valueOfEegChannels(const QByteArray &channel1, const QByteArray &channel2);

public slots:
    void packetDectection(const QList<QByteArray> &inputData);

private:
    void packetParser(const QByteArray &data);

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
    ParserState currentState = IDLE;
    uint8_t eegChannel1[4];
    uint8_t eegChannel2[4];
};

#endif // DATAPARSER_H
