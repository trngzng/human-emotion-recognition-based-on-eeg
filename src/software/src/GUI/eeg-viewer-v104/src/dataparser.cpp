#include "dataparser.h"
#include <QDebug>

// Các hằng số cho byte bắt đầu và byte kết thúc
const uint8_t startOfPacket = 0xAA;
const uint8_t endOfPacket = 0x55;

DataParser::DataParser(QObject *parent)
    : QThread(parent)
{
}

DataParser::~DataParser()
{
    if (isRunning()) {
        requestInterruption();
        quit();
        wait();
    }
}

void DataParser::processInputData(const QList<QByteArray> &inputData)
{
    static QByteArray packetBuffer;
    static int expectedDataLength = 0; // Số byte dữ liệu cần nhận (có thể xác định từ byte lệnh)

    // Duyệt qua từng chunk dữ liệu
    for (const QByteArray &chunk : inputData)
    {
        for (int i = 0; i < chunk.size(); ++i)
        {
            uint8_t byte = chunk.at(i);
            switch (DataParser::currentState)
            {
            case IDLE:
                // Chờ nhận byte bắt đầu
                if (byte == startOfPacket)
                {
                    packetBuffer.clear();
                    packetBuffer.append(byte);
                    DataParser::currentState = WAIT_CMD;
                }
                break;
            case WAIT_CMD:
                // Byte tiếp theo là lệnh hoặc byte chỉ báo độ dài
                packetBuffer.append(byte);
                // Ví dụ: giả sử byte này chứa thông tin về độ dài payload ở 5 bit thấp
                expectedDataLength = (byte & 0x1F) + 1;
                DataParser::currentState = WAIT_DATA;
                break;
            case WAIT_DATA:
                packetBuffer.append(byte);
                if (packetBuffer.size() >= (2 + expectedDataLength))
                {
                    // Đã nhận đủ dữ liệu payload, chuyển sang nhận CRC hoặc end tùy thiết kế
                    // Nếu không có CRC, có thể chuyển luôn sang WAIT_END
                    DataParser::currentState = WAIT_END;
                }
                break;
            case WAIT_CRC:
                // Nếu bạn có thêm CRC, thực hiện ở đây: append byte, sau đó kiểm tra nếu đủ 2 byte CRC rồi chuyển sang WAIT_END
                packetBuffer.append(byte);
                if (packetBuffer.size() >= 2 + expectedDataLength + 2)
                {
                    DataParser::currentState = WAIT_END;
                }
                break;
            case WAIT_END:
                packetBuffer.append(byte);
                if (byte == endOfPacket) {
                    // Gói tin hoàn chỉnh được nhận
                    emit parsedPacket(packetBuffer);  // Hàm xử lý gói tin, tự định nghĩa theo logic của bạn
                }
                // Sau khi xử lý, reset FSM
                DataParser::currentState = IDLE;
                packetBuffer.clear();
                expectedDataLength = 0;
                break;
            default:
                DataParser::currentState = IDLE;
                packetBuffer.clear();
                expectedDataLength = 0;
                break;
            }
        }
    }
}

void DataParser::run()
{

}
