#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QThread>

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
    void processInputData(const QList<QByteArray> &inputData);

signals:
    void parsedPacket(const QByteArray &data);

protected:
    void run() override;

private:
    ParserState currentState;

};

#endif // DATAPARSER_H
