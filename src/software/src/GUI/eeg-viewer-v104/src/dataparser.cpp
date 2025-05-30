#include "dataparser.h"
#include <QDebug>

// Các hằng số cho byte bắt đầu và byte kết thúc
const uint8_t startByte = 0x55;
const uint8_t endByte = 0xAA;

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
    static int len;

    for (const QByteArray &chunk : inputData)
    {
        for (int i = 0; i < chunk.size(); ++i)
        {
            uint8_t byte = chunk.at(i);
            switch (DataParser::currentState)
            {
            case IDLE:
                if (byte == startByte)
                {
                    packetBuffer.clear();
                    packetBuffer.append(byte);
                    DataParser::currentState = START_BYTE_DETECTION;
                }
                else
                {
                    DataParser::currentState = IDLE;
                }
                break;

            case START_BYTE_DETECTION:
                if (byte == startByte)
                {
                    packetBuffer.append(byte);
                    DataParser::currentState = WAIT_SOP;
                }
                else
                {
                    packetBuffer.clear();
                    DataParser::currentState = IDLE;
                }

                break;

            case WAIT_SOP:
                if (byte == startByte)
                {
                    packetBuffer.append(byte);
                    DataParser::currentState = PACKET_DETECTION;
                }
                else
                {
                    packetBuffer.clear();
                    DataParser::currentState = IDLE;
                }
                break;

            case PACKET_DETECTION:
            case WAIT_VALID_CMD:
                if (byte == 0x00)
                {
                    packetBuffer.append(byte);
                    DataParser::currentState = WAIT_VALID_LEN;
                }
                else
                {
                    packetBuffer.clear();
                    DataParser::currentState = IDLE;
                }
                break;

            case WAIT_VALID_LEN:
                len = byte;
                packetBuffer.append(byte);
                DataParser::currentState = RECEIVE_PAYLOAD;
                break;

            case RECEIVE_PAYLOAD:
                packetBuffer.append(byte);
                len--;
                if (len > 0)
                {
                    DataParser::currentState = RECEIVE_PAYLOAD;
                }
                else
                {
                    DataParser::currentState = WAIT_END_BYTES;
                }

                break;

            case WAIT_END_BYTES:
                if (byte == endByte)
                {
                    packetBuffer.append(byte);
                    DataParser::currentState = CONT_WAIT_END_BYTE;
                }
                else
                {
                    packetBuffer.clear();
                    DataParser::currentState = IDLE;
                }
                break;

            case CONT_WAIT_END_BYTE:
                if (byte == endByte)
                {
                    packetBuffer.append(byte);
                    DataParser::currentState = WAIT_LAST_END_BYTE;
                }
                else
                {
                    packetBuffer.clear();
                    DataParser::currentState = IDLE;
                }
                break;

            case WAIT_LAST_END_BYTE:
                if (byte == endByte)
                {
                    packetBuffer.append(byte);
                    DataParser::currentState = IDLE;
                    DataParser::packetParser(packetBuffer);
                }
                else
                {
                    packetBuffer.clear();
                    DataParser::currentState = IDLE;
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
    for (int i = 0; i < 4; i++)
    {
        eegChannel1[i] = static_cast<uint8_t>(data[5 + i]);
        eegChannel2[i] = static_cast<uint8_t>(data[9 + i]);
    }

    QByteArray eeg1(reinterpret_cast<const char*>(eegChannel1), 4);
    QByteArray eeg2(reinterpret_cast<const char*>(eegChannel2), 4);

    emit valueOfEegChannels(eeg1, eeg2);
}

void DataParser::run()
{

}
