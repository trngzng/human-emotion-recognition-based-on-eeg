#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QVector>
#include <QList>

enum class FilterType {
    LowPass,
    HighPass,
    Notch
};

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = nullptr);

    float process(float input);

private:
    FilterType type;

    QList<float> numerator;     // Tu so
    QList<float> denominator;   // Mau so

    QList<float> output;
    QList<float> input;

    void reset();
};

#endif // FILTER_H
