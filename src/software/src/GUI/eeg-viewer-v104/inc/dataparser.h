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
    void valueOfEegChannels(const float &channel1, const float &channel2);

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
    float eegChannel[2];
};

#endif // DATAPARSER_H
