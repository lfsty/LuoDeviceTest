#ifndef COMCOMMUNICATE_H
#define COMCOMMUNICATE_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QString>
#include "../ConstData.h"
#include <QMessageBox>

struct SerialPortData
{
    qint32 Baut = 460800;
    QString PortName;
    QSerialPort::DataBits DataBits = QSerialPort::Data8;
    QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl;
    QSerialPort::StopBits StopBits = QSerialPort::OneStop;
    QSerialPort::Parity Parity = QSerialPort::NoParity;
};

quint8 checkParity(QByteArray data);
int _24Bit2Int(const BYTE data[3]);
class ComCommunicate : public QObject
{
    Q_OBJECT
public:
    explicit ComCommunicate(QObject* parent = nullptr);

public:
    void MoveToThead(QThread* thread);
    void SetSerialPortData(SerialPortData serialport_data);
    void OpenSerialPort();
    void CloseSerialPort();
    void SetDeivce(QByteArray data);
    void OpenDevice();
    void CloseDeivce();
private:
    void OnSerialPortRecvData();
    void parseFrame(const QByteArray& frameData);
private:
    QSerialPort m_serialPort;
    QByteArray  m_opendevice_message;
    QByteArray  m_closedevice_message;
    QByteArray  m_setdevice_message;
    //数据接收缓冲区
    QByteArray buffer;

    bool m_is_device_open = false;

signals:
    void sig_serialport_open();
    void sig_serialport_close();
    void sig_device_set();
    void sig_device_open();
    void sig_device_close();
    void sig_recv_ch_data(QVector<float>);
    void sig_recv_ch_data_raw(QByteArray);
    void sig_recv_frame();

    void sig_sync_error();
    void sig_parity_error();
    void sig_serialNum_error();

    void sig_print_message(QString, QByteArray);
};

#endif // COMCOMMUNICATE_H
