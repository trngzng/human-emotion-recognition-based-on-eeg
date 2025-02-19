#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "inc/serial.h"

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

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void updateSerialDeviceList();
    void on_pBtnSerialConnect_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPortInfo info;
    Serial serial;

};
#endif // MAINWINDOW_H
