#include "mainwindow.h"
#include "Windows.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,_serialPort(new QSerialPort(this))

{
    ui->setupUi(this);
    oldLine="";
    foreach( const QSerialPortInfo info, QSerialPortInfo::availablePorts() ){
        qDebug() << "Name        :" << info.portName();
        qDebug() << "Description :" << info.description();
        qDebug() << "Manufacturer:" << info.manufacturer() << "n";
    }
    _serialPort->setBaudRate(QSerialPort::Baud115200);
    //updateSerialPorts();

    mSerialScanTimer=new QTimer(this);
    //mSerialScanTimer->setInterval(100);
    //mSerialScanTimer->start();

    if ( _serialPort->open( QIODevice::ReadOnly ) )
       {
           connect( _serialPort, &QSerialPort::readyRead,
                    this,        &MainWindow::printData );
       }
       else
       {
           qDebug() << "Couldn't open the port [COM4].";
       }


}



MainWindow::~MainWindow()
{
    _serialPort->close();
    disconnect(_serialPort,&QSerialPort::readyRead,this,&MainWindow::printData);
    delete _serialPort;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    _serialPort->close();
    disconnect(_serialPort,&QSerialPort::readyRead,this,&MainWindow::printData);
    _serialPort->setPortName(this->ui->comboBox->currentText());

    // baudRate,dataBits,parity,stopBits:
    // Arduinoのスケッチで指定したものと同じ数値を指定する
    _serialPort->setBaudRate( QSerialPort::Baud115200 );
    // ちなみに以下はArduinoのスケッチでSerial.beginの第2引数を省略した時の値
    _serialPort->setDataBits( QSerialPort::Data8 );
    _serialPort->setParity(   QSerialPort::NoParity );
    _serialPort->setStopBits( QSerialPort::OneStop );

    if ( _serialPort->open( QIODevice::ReadOnly ) )
    {
        connect( _serialPort, &QSerialPort::readyRead,
                 this,        &MainWindow::printData );
    }
    else
    {
        qDebug() << "Couldn't open the port [COM4].";
    }

}


void MainWindow::printData()
{
    if ( sender() == _serialPort )
    {
        while(_serialPort->canReadLine()){
            QString data = QString( _serialPort->readLine() );
            data= data.replace("\r\n","");
            if(data.split(",").length()==4){
                if(oldLine.split(",").length()<3){
                    oldLine=data;
                    continue;
                }
                //qDebug()  <<data.split(",")[3].toInt();
                if(oldLine.split(",")[3].toInt()==100 && data.split(",")[3].toInt()==0){
                    qDebug()<<"push";
                    INPUT ip;
                    ip.type = INPUT_KEYBOARD;
                    ip.ki.wScan = 0; // hardware scan code for key
                    ip.ki.time = 0;
                    ip.ki.dwExtraInfo = 0;
                    byte x = VkKeyScan(QString("w").at(0).unicode());
                    ip.ki.wVk=x;
                    ip.ki.dwFlags=0;
                    SendInput(1,&ip,sizeof(INPUT));
                }else if(oldLine.split(",")[3].toInt()==0 && data.split(",")[3].toInt()==100){
                    qDebug()<<"pull";
                    INPUT ip;
                    ip.type = INPUT_KEYBOARD;
                    ip.ki.wScan = 0; // hardware scan code for key
                    ip.ki.time = 0;
                    ip.ki.dwExtraInfo = 0;
                    byte x = VkKeyScan(QString("w").at(0).unicode());
                    ip.ki.wVk=x;
                    ip.ki.dwFlags=KEYEVENTF_KEYUP;
                    SendInput(1,&ip,sizeof(INPUT));
                }
                if(oldLine.split(",")[2].toInt()!=data.split(",")[2].toInt()){
                    ui->dial->setValue((data.split(",")[2].toInt()*-1/10));
                    INPUT ip;
                    ip.type=INPUT_MOUSE;//https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
                    ip.mi.dx=0;
                    ip.mi.dy=0;
                    ip.mi.dwFlags=MOUSEEVENTF_WHEEL;
                    ip.mi.mouseData=(data.split(",")[2].toInt()-oldLine.split(",")[2].toInt())*WHEEL_DELTA/20;
                    ip.mi.time=0;
                    SendInput(1,&ip,sizeof(INPUT));
                }
                INPUT ip;
                ip.type=INPUT_MOUSE;//https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
                ip.mi.dx=(data.split(",")[1].toInt()-1800)/200;
                ip.mi.dy=(data.split(",")[0].toInt()-1800)/200;
                ip.mi.dwFlags=MOUSEEVENTF_MOVE;
                ip.mi.time=0;

                SendInput(1,&ip,sizeof(INPUT));




                MainWindow::oldLine=data;
            }
            //qDebug()<<oldLine<<":"<<data;
            //qDebug()  <<data.split(",")[3].toInt();
            ui->textEdit->append( data );
            if(ui->textEdit->toPlainText().length()>10000)ui->textEdit->clear();
        }


    }
}

void MainWindow::updateSerialPorts(){

}
