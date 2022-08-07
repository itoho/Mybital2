#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void printData();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *_serialPort;
    QList<QSerialPortInfo> mSerialPorts;
    QTimer *mSerialScanTimer;
    QString currentPortName;
    QString oldLine;
    void updateSerialPorts();
};
#endif // MAINWINDOW_H
