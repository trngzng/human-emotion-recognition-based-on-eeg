#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QMutex>
#include <QVector>
#include "qcustomplot.h"

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);
    void addDataToBuffer(float value);
    void processBuffer();

    void setMaxSamples(int samples);

public slots:
    void clearData();

private:
    void addPoint(float y);

    QCustomPlot *customPlot;
    QCPGraph *graph;

    QVector<double> xData, yData;
    QVector<float> buffer;
    QMutex mutex;

    int maxSamples = 500;
    int writeIndex = 0;
};

#endif // PLOTTER_H
