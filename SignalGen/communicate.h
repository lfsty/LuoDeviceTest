#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QObject>
#include <QSerialPort>
#include "ConstData.h"
#include <QTimer>

quint8 checkParity(QByteArray data);

struct SerialPortData
{
    qint32 Baut = 460800;
    QString PortName;
    QSerialPort::DataBits DataBits = QSerialPort::Data8;
    QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl;
    QSerialPort::StopBits StopBits = QSerialPort::OneStop;
    QSerialPort::Parity Parity = QSerialPort::NoParity;
};


class Communicate : public QObject
{
    Q_OBJECT
public:
    explicit Communicate(QObject* parent = nullptr);
    void OpenCom(SerialPortData com_data);
    void CloseCom();
    void MoveToThead(QThread* thread);
private:

    QSerialPort m_serialPort;
    void ReadData();
    //数据接收缓冲区
    QByteArray buffer;
    QByteArray  m_opendevice_message;
    QByteArray  m_closedevice_message;
    QByteArray m_SendData;
    QTimer m_send_data_timer;
    void parseFrame(const QByteArray& frameData);
signals:
    void sig_serialport_open();
    void sig_serialport_close();
};

#endif // COMMUNICATE_H
