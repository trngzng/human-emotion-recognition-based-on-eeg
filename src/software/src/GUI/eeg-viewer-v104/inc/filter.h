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
    void reset();
    float applyFilter(FilterType type, float input);

private:
    QList<float> output;
    QList<float> input;
};

#endif // FILTER_H
