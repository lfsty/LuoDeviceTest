#include "communicate.h"
#include <QDebug>
#include <QtMath>
Communicate::Communicate(QObject* parent)
    : QObject{parent}
{
    connect(&m_serialPort, &QSerialPort::readyRead, this, &Communicate::ReadData);
    {
        //打开
        m_opendevice_message.push_back(0x5A);
        m_opendevice_message.push_back(0x5A);
        m_opendevice_message.push_back(0x5A);
        m_opendevice_message.push_back(0x01);
        m_opendevice_message.push_back(0x02);
        m_opendevice_message.push_back(0x03);
        m_opendevice_message.push_back(char(0x00));
        m_opendevice_message.push_back(0x02);
        m_opendevice_message.push_back(char(0x00));
        m_opendevice_message.push_back(0x01);
        m_opendevice_message.push_back(char(0x02));
        m_opendevice_message.push_back(0x5B);
    }
    {
        //关闭
        m_closedevice_message.push_back(0x5A);
        m_closedevice_message.push_back(0x5A);
        m_closedevice_message.push_back(0x5A);
        m_closedevice_message.push_back(0x01);
        m_closedevice_message.push_back(0x02);
        m_closedevice_message.push_back(0x03);
        m_closedevice_message.push_back(char(0x00));
        m_closedevice_message.push_back(0x03);
        m_closedevice_message.push_back(char(0x00));
        m_closedevice_message.push_back(0x01);
        m_closedevice_message.push_back(char(0x03));
        m_closedevice_message.push_back(0x5B);
    }
    {
        // 信号初始化
        m_SendData.push_back(0x5A);
        m_SendData.push_back(0x5A);
        m_SendData.push_back(0x5A);
        m_SendData.push_back(0x01);
        m_SendData.push_back(0x02);
        m_SendData.push_back(0x03);
        m_SendData.push_back(char(0x00));
        m_SendData.push_back(0x02);
        m_SendData.push_back(char(0x00));
        m_SendData.push_back(0x1A);
        m_SendData.push_back(char(0x00));
        m_SendData.push_back(char(0x00));
        for(int i = 0; i < TOTAL_CH_NUM; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                m_SendData.push_back(char(0x00));
            }
        }
        m_SendData.push_back(checkParity(m_SendData));
    }
    {
        //定时器
        m_send_data_timer.setTimerType(Qt::TimerType::PreciseTimer);
        connect(&m_send_data_timer, &QTimer::timeout, this, [ = ]()
        {
            static double _time = 0;
            _time += 1.0 / m_freq;
            static double _data;
            _data = (qSin(2 * M_PI * 20 * _time) + qSin(2 * M_PI * 50 * _time) + qSin(2 * M_PI * 1 * _time)) * 5 + 2;
            int _tmp = int(_data / 1000 * 24 / 4500 * 8388608);
            auto _data_list = _Int224Bit(_tmp);
            for(int i = 0; i < 3; i++)
            {
                m_SendData[i + 12] = _data_list[i];
            }

            static quint16 index = 0;
            m_SendData[10] = index >> 8;
            m_SendData[11] = index;
            index++;

            BYTE parity = 0;
            int FrameLen = m_SendData.size();
            for (int i = 0; i < FrameLen - 1; i++)
            {
                parity ^= m_SendData.at(i);
            }

            m_SendData[m_SendData.size() - 1] = parity;

            m_serialPort.write(m_SendData);
        });
    }
}

void Communicate::OpenCom(SerialPortData com_data)
{
    m_serialPort.setPortName(com_data.PortName);
    m_serialPort.setBaudRate(com_data.Baut);
    m_serialPort.setDataBits(com_data.DataBits);
    m_serialPort.setFlowControl(com_data.FlowControl);
    m_serialPort.setStopBits(com_data.StopBits);
    m_serialPort.setParity(com_data.Parity);
    m_serialPort.open(QIODevice::ReadWrite);
    if (m_serialPort.isOpen())
    {
        emit sig_serialport_open();
    }
}

void Communicate::CloseCom()
{
    m_serialPort.close();
    if (!m_serialPort.isOpen())
    {
        emit sig_serialport_close();
    }
}

void Communicate::MoveToThead(QThread* thread)
{
    moveToThread(thread);
    m_serialPort.moveToThread(thread);
    m_send_data_timer.moveToThread(thread);
}

void Communicate::ReadData()
{
    QByteArray data = m_serialPort.readAll();
    buffer.append(data);

    while (!buffer.isEmpty())
    {
        // 同步帧
        while (!buffer.isEmpty())
        {
            if(buffer.size() < 3)
            {
                return;
            }
            if (buffer.at(0) == 0x5A && buffer.at(1) == 0x5A && buffer.at(2) == 0x5A)
            {
                break;
            }
            else
            {
//                qDebug() << QString("寻找同步帧：" + QString::asprintf("%02X", quint8(buffer.at(0)))).toUtf8().data();
                buffer.remove(0, 1);
            }
        }

        //数据解析
        if (buffer.size() < 10)
        {
            return;
        }
        else
        {
            quint16 commamd_len = buffer.at(8) << 8 | buffer.at(9);
            quint64 frame_len = 10 + commamd_len + 1;
            if (buffer.size() < frame_len)
            {
                return;
            }
            else
            {
                //处理一帧数据
                QByteArray frameData = buffer.mid(0, frame_len);
                parseFrame(frameData);
                buffer.remove(0, frame_len);
            }
        }
    }
}

void Communicate::parseFrame(const QByteArray& frameData)
{
    //奇偶校验
    BYTE parity = 0;
    int FrameLen = frameData.size();
    for (int i = 0; i < FrameLen - 1; i++)
    {
        parity ^= frameData.at(i);
    }
    if (parity != BYTE(frameData.at(FrameLen - 1)))
    {
//        QMessageBox::critical(this, "error", "奇偶校验错误!" + QString::asprintf("%02X", BYTE(frameData.at(FrameLen - 1))));
//        qDebug() << "奇偶校验错误！";
//        emit sig_parity_error();
        return;
    }
    BYTE low = 0x00;
    BYTE high = 0x00;

    UINT read_buf_index = 0;

    // 3字节同步帧检验
    for (int i = 0; i < 3; i++)
    {
        if (frameData.at(read_buf_index++) != 0x5A)
        {
//            QMessageBox::critical(this, "error", "同步帧出错!");
            return;
        }
    }

    // 3字节设备号
    for (int i = 0; i < 3; i++)
    {
        //目前设备号不做判断
        read_buf_index++;
    }

    // 2字节命令名称
    high = frameData.at(read_buf_index++);
    low = frameData.at(read_buf_index++);
    UINT16 CommandName = high << 8 | low;
    // 2字节命令长度
    high = frameData.at(read_buf_index++);
    low = frameData.at(read_buf_index++);
    UINT16 CommandLen = high << 8 | low;


    switch (CommandName)
    {
        case 0x0001:
            {
                //配置
                // 2字节开启导联，高地址为1
                UINT16 OpenChIndex = frameData.at(read_buf_index++) << 8 | frameData.at(read_buf_index++);
                // 1字节采样速率
                // 1=250;2=500;4=1000
                UINT8 FreqCommand = frameData.at(read_buf_index++);
                switch(FreqCommand)
                {
                    case 1:
                        m_freq = 250;
                        break;
                    case 2:
                        m_freq = 500;
                        break;
                    case 4:
                        m_freq = 1000;
                        break;
                }

                m_send_data_timer.setInterval(1000 / m_freq);
                m_serialPort.write(frameData);
                break;
            }
        case 0x0002:
            {
                //开启
                m_send_data_timer.start();
                break;
            }
        case 0x0003:
            {
                //停止
                // bufferVect[read_buf_index++] == 0x03;
                m_send_data_timer.stop();
                m_serialPort.write(frameData);
                break;
            }
        default:
            {
//                QMessageBox::critical(parent(), "error", "命令名称出错!");
                return;
                break;
            }
    }
}

quint8 checkParity(QByteArray data)
{
    quint8 parity = 0;
    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        parity ^= *iter;
    }
    return parity;
}

QVector<quint8> _Int224Bit(const int& data)
{
    QVector<quint8> _24BitInt;
    _24BitInt.resize(3);
    _24BitInt[0] = (data & 0x00FF0000) >> 16;
    _24BitInt[1] = (data & 0x0000FF00) >> 8;
    _24BitInt[2] = (data & 0x000000FF);

    return _24BitInt;
}
