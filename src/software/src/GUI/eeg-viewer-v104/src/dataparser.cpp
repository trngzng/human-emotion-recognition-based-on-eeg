#include "dataparser.h"
#include <QDebug>

// Các hằng số cho byte bắt đầu và byte kết thúc
const uint8_t startOfPacket = 0xAA;
const uint8_t endOfPacket = 0x55;

DataParser::DataParser(QObject *parent)
    : QThread(parent)
{
    connect(this, &DataParser::receivedPacket, this, &DataParser::packetParser);
}

DataParser::~DataParser()
{
    if (isRunning()) {
        requestInterruption();
        quit();
        wait();
    }
}

void DataParser::packetDectection(const QList<QByteArray> &inputData)
{
    static QByteArray packetBuffer;

    for (const QByteArray &chunk : inputData)
    {
        for (int i = 0; i < chunk.size(); ++i)
        {
            uint8_t byte = chunk.at(i);
            switch (DataParser::currentState)
            {
            case IDLE:
                if (byte == startOfPacket)
                {
                    packetBuffer.clear();
                    packetBuffer.append(byte);
                    DataParser::currentState = WAIT_CMD;
                }
                break;
            case WAIT_CMD:
                packetBuffer.append(byte);
                DataParser::currentState = WAIT_DATA;
                break;
            case WAIT_DATA:
                packetBuffer.append(byte);
                if (packetBuffer.size() >= 34)
                {
                    DataParser::currentState = WAIT_END;
                }
                break;
            case WAIT_CRC:
                packetBuffer.append(byte);
                if (packetBuffer.size() >= 36)
                {
                    DataParser::currentState = WAIT_END;
                }
                break;
            case WAIT_END:
                packetBuffer.append(byte);
                if (byte == endOfPacket) {
                    // Gói tin hoàn chỉnh được nhận
                    emit receivedPacket(packetBuffer);
                    DataParser::currentState = IDLE;
                    packetBuffer.clear();
                }

                break;
            default:
                DataParser::currentState = IDLE;
                packetBuffer.clear();
                break;
            }
        }
    }
}

void DataParser::packetParser(const QByteArray &data)
{
    if ((static_cast<uint8_t>(data.at(0)) == startOfPacket) && (static_cast<uint8_t>(data.at(data.size() - 1)) == endOfPacket))
    {
        memcpy(&(DataParser::currentPacket), data.constData(), sizeof(PacketTypeDef));
        emit parsedPacket(DataParser::currentPacket);
    }
}

void DataParser::run()
{

}
