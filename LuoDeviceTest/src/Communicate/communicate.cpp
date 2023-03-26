#include "communicate.h"
#include <QDebug>

communicate::communicate(QObject* parent)
    : QObject{parent}
{
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

    connect(&m_serialPort, &QSerialPort::readyRead, this, &communicate::OnSerialPortRecvData);
}

void communicate::MoveToThead(QThread* thread)
{
    moveToThread(thread);
    m_serialPort.moveToThread(thread);
}

void communicate::SetSerialPortData(SerialPortData serialport_data)
{
    m_serialPort.setPortName(serialport_data.PortName);
    m_serialPort.setBaudRate(serialport_data.Baut);
    m_serialPort.setDataBits(serialport_data.DataBits);
    m_serialPort.setFlowControl(serialport_data.FlowControl);
    m_serialPort.setStopBits(serialport_data.StopBits);
    m_serialPort.setParity(serialport_data.Parity);
}

void communicate::OpenSerialPort()
{
    m_serialPort.open(QIODevice::ReadWrite);
    if (m_serialPort.isOpen())
    {
        emit sig_serialport_open();
    }
}

void communicate::CloseSerialPort()
{
    m_serialPort.close();
    if (!m_serialPort.isOpen())
    {
        emit sig_serialport_close();
    }
}

void communicate::SetDeivce(QByteArray data)
{
    m_setdevice_message = data;
    m_serialPort.write(data);
}

void communicate::OpenDevice()
{
    m_serialPort.write(m_opendevice_message);
}

void communicate::CloseDeivce()
{
    m_serialPort.write(m_closedevice_message);
}

void communicate::OnSerialPortRecvData()
{
    QByteArray data = m_serialPort.readAll();

//    if(m_is_raw_data_save)
//    {
//        m_qfile_raw.write(data);
//    }

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
                emit sig_sync_error();
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
                emit sig_print_message("收到数据", frameData);
                parseFrame(frameData);
                buffer.remove(0, frame_len);
            }
        }
    }
}

void communicate::parseFrame(const QByteArray& frameData)
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
        emit sig_parity_error();
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
    //目前只有00,01,02
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

                if(frameData == m_setdevice_message)
                {
                    emit sig_device_set();
                }
                break;
            }
        case 0x0002:
            {
                //数据
                // 2字节序列号
                if(m_is_device_open == false)
                {
                    m_is_device_open = true;
                    emit sig_device_open();
                }
                static UINT16 FrameNumIndex = frameData.at(read_buf_index) << 8 | frameData.at(read_buf_index + 1);
                high = frameData.at(read_buf_index++);
                low = frameData.at(read_buf_index++);
                UINT16 recvIndex = high << 8 | low;

                if (FrameNumIndex != recvIndex)
                {
                    FrameNumIndex = recvIndex;
                    emit sig_serialNum_error();
                }
                FrameNumIndex++;

                if (FrameNumIndex == 0)
                {
                    FrameNumIndex++;
                }

                QVector<float> _frame_data;
                // 3字节有符号整形解析
                for (int i = 0; i < TOTAL_CH_NUM; i++)
                {
                    BYTE data_24bit[3];
                    data_24bit[0] = frameData.at(read_buf_index++);
                    data_24bit[1] = frameData.at(read_buf_index++);
                    data_24bit[2] = frameData.at(read_buf_index++);

                    float ch_data = _24Bit2Int(data_24bit);
                    //float d_float = ch_data / 16777216 * 4500 / 24 * 1000;
                    float d_float = ch_data / 8388608 * 4500 / 24 * 1000;
                    _frame_data.push_back(d_float);
                }
                emit sig_recv_frame();
                emit sig_recv_ch_data(_frame_data);
                //数据
//                m_frame_count++;
                break;
            }
        case 0x0003:
            {
                //停止
                // bufferVect[read_buf_index++] == 0x03;
                emit sig_device_close();
                m_is_device_open = false;
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

int _24Bit2Int(const quint8 data[])
{
    int num = (data[0] << 16) | (data[1] << 8) | (data[2]);
    //判断正负
    if ((data[0] & 0x80) == 0x80)
    {
        num |= 0xFF000000;
    }
    return num;
}
