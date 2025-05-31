#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "inc/serial.h"
#include "inc/plotter.h"
#include "inc/dataparser.h"
#include "inc/filter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void convertValueEegChannels(const QByteArray &channel1, const QByteArray &channel2);

signals:
    void sendRawEegDataToPlot(int32_t channel1, int32_t channel2);
    void sendFilteredEegDataToPlot(int32_t channel1, int32_t channel2);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void updateSerialDeviceList();
    void on_pBtnSerialConnect_clicked();
    float convertADCtoVoltage_AD7768(int32_t data, float vref, int32_t gain, int32_t offset);

private:
    Ui::MainWindow *ui;
    Serial *serialDevice;
    QThread *serialThread;
    DataParser *dataParser;
    Filter *notchFilterChannel1;
    Filter *notchFilterChannel2;
    Filter *lowPassFilterChannel1;
    Filter *lowPassFilterChannel2;
    Plotter *rawEegChannel1; // EEG Channel 1
    Plotter *rawEegChannel2; // EEG Channel 2
    Plotter *filteredEegChannel1; // EEG Channel 1
    Plotter *filteredEegChannel2; // EEG Channel 2
};


#endif // MAINWINDOW_H
