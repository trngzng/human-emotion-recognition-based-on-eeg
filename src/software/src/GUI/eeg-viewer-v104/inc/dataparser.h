#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QThread>

#pragma pack(1)  // Chống trình biên dịch tự động canh chỉnh bộ nhớ
typedef struct
{
    uint8_t sop;
    uint8_t length : 5;
    uint8_t cmd : 3;
    uint8_t data[32];
    uint16_t crc;
    uint16_t eop;
} PacketTypeDef;
#pragma pack()

enum ParserState {
    IDLE,
    WAIT_CMD,
    WAIT_DATA,
    WAIT_CRC,
    WAIT_END
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
    void parsedPacket(const PacketTypeDef &packet);

protected:
    void run() override;

private:
    void packetParser(const QByteArray &data);

private:
    ParserState currentState;
    PacketTypeDef currentPacket;
};

#endif // DATAPARSER_H
