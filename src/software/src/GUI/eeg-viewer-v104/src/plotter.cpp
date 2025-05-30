#include "plotter.h"
#include <QtGlobal>

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    auto layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);

    graph = customPlot->addGraph();
    graph->setPen(QPen(Qt::blue));

    customPlot->xAxis->setLabel("Sample");
    customPlot->yAxis->setLabel("EEG Value");
    customPlot->xAxis->setRange(0, maxSamples);
    customPlot->yAxis->setRange(-5000, 5000);

    xData.resize(maxSamples);
    yData.resize(maxSamples, qQNaN());
    for (int i = 0; i < maxSamples; ++i) {
        xData[i] = i;
    }

    graph->setData(xData, yData);
}

void Plotter::setMaxSamples(int samples)
{
    maxSamples = samples;
    xData.resize(maxSamples);
    yData.resize(maxSamples, qQNaN());
    for (int i = 0; i < maxSamples; ++i) {
        xData[i] = i;
    }
    graph->setData(xData, yData);
    customPlot->replot();
}

void Plotter::addDataToBuffer(float value)
{
    QMutexLocker locker(&mutex);
    buffer.append(value);
}

void Plotter::clearData()
{
    QMutexLocker locker(&mutex);
    buffer.clear();
    yData.fill(qQNaN());
    writeIndex = 0;
    graph->setData(xData, yData);
    customPlot->replot();
}

void Plotter::processBuffer()
{
    QMutexLocker locker(&mutex);
    bool updated = false;

    while (!buffer.isEmpty()) {
        float y = buffer.takeFirst();
        addPoint(y);
        updated = true;
    }

    if (updated) {
        graph->setData(xData, yData);
        customPlot->replot();
    }
}

void Plotter::addPoint(float y)
{
    yData[writeIndex] = static_cast<double>(y);
    writeIndex = (writeIndex + 1) % maxSamples;
}
