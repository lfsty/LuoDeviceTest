#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_filter_work(TOTAL_CH_NUM)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_cmd_switch_triggered(true);
    on_m_radioButton_filter_clicked(false);

    {
        // 图标设置对话框初始化
        m_Dlg_chartsetting.SetCurrentChartData(m_chartdata);
        connect(&m_Dlg_chartsetting, &ChartSetting::SettingChanged, this, [ = ](ChartSettingData chart_data)
        {
            m_chartdata = chart_data;

//            m_chartdata.XRange = chart_data.XRange;
//            m_axisY->setRange(m_chartdata.YMin, m_chartdata.YMax);

//            if(chart_data.AutoAdjust == false)
//            {
//                m_chartdata = chart_data;
//                if(m_point_data.isEmpty())
//                {
//                    m_axisX->setMax(m_chartdata.XRange);
//                }
//                else
//                {
//                    m_axisX->setMax(m_point_data.front().x() + m_chartdata.XRange);
//                }
//            }
//            else
//            {
//                m_chartdata.AutoAdjust = chart_data.AutoAdjust;
//            }
        });
    }

    {
        // 自动刷新串口信息
        UpdateSerialPortInfo();
        m_timer_update_serialport_info.setInterval(1000);
        connect(&m_timer_update_serialport_info, &QTimer::timeout, this, [ = ]()
        {
            UpdateSerialPortInfo();
        });
        m_timer_update_serialport_info.start();
    }

    {
        //通信设置
        m_communicate.MoveToThead(&m_thread_communicate);
        connect(this, &MainWindow::sig_Set_serialport, &m_communicate, &communicate::SetSerialPortData);
        connect(this, &MainWindow::sig_Open_serialport, &m_communicate, &communicate::OpenSerialPort);
        connect(this, &MainWindow::sig_Close_serialport, &m_communicate, &communicate::CloseSerialPort);
        connect(&m_communicate, &communicate::sig_serialport_open, this, [ = ]()
        {
            ui->m_comboBox_BautSelect->setEnabled(false);
            ui->m_comboBox_ComNameSelect->setEnabled(false);
            ui->m_pushbutton_OpenCom->setText("关闭串口");
        });
        connect(&m_communicate, &communicate::sig_serialport_close, this, [ = ]()
        {
            ui->m_comboBox_BautSelect->setEnabled(true);
            ui->m_comboBox_ComNameSelect->setEnabled(true);
            ui->m_pushbutton_OpenCom->setText("打开串口");
        });
        connect(this, &MainWindow::sig_Set_deivce, &m_communicate, &communicate::SetDeivce);
        connect(&m_communicate, &communicate::sig_device_set, this, [ = ]()
        {

        });
        connect(this, &MainWindow::sig_Open_device, &m_communicate, &communicate::OpenDevice);
        connect(&m_communicate, &communicate::sig_device_open, this, [ = ]()
        {

        });
        connect(this, &MainWindow::sig_Close_device, &m_communicate, &communicate::CloseDeivce);
        connect(&m_communicate, &communicate::sig_device_close, this, [ = ]()
        {

        });
        // 校验位错误
        connect(&m_communicate, &communicate::sig_parity_error, this, [ = ]()
        {
            static quint64 _parity_error_num = 0;
            _parity_error_num++;
            ui->m_label_errorNum_parity->setText("奇偶校验错误数：" + QString::number(_parity_error_num));
        });
        // 序列号错误
        connect(&m_communicate, &communicate::sig_serialNum_error, this, [ = ]()
        {
            static quint64 _sync_error_num = 0;
            _sync_error_num++;
            ui->m_label_errorNum_sync->setText("同步帧错误数：" + QString::number(_sync_error_num));
        });
        //同步位错误
        connect(&m_communicate, &communicate::sig_sync_error, this, [ = ]()
        {
            static quint64 _serialNum_error = 0;
            _serialNum_error++;
            ui->m_label_errorNum_serial->setText("序列号错误数：" + QString::number(_serialNum_error));
        });
        connect(&m_communicate, &communicate::sig_recv_frame, this, [ = ]()
        {
            m_frame_count++;
        });
        m_thread_communicate.start();
    }

    {
        // 滤波设置
        m_filter_work.moveToThread(&m_thread_filter);
        connect(this, &MainWindow::sig_Set_filter_sampling, &m_filter_work, &filterWork::SetSamplingFreq);
        m_thread_filter.start();
    }

    {
        // 文件保存
        m_file_save.moveToThread(&m_thread_filesave);
        connect(this, &MainWindow::sig_Start_savedata, &m_file_save, &FileSave::StartRecord);
        connect(this, &MainWindow::sig_Stop_savedata, &m_file_save, &FileSave::StopRecord);
        m_thread_filesave.start();
    }

    {
        //帧数统计
        m_frame_timer.setInterval(1000);
        connect(&m_frame_timer, &QTimer::timeout, this, [ = ]()
        {
            ui->m_lcdNumber_frame_count->display(m_frame_count);
            m_frame_count = 0;
        });
        m_frame_timer.start();
    }

    {
        //绘图设置
        connect(this, &MainWindow::sig_Set_filter_sampling, ui->m_chart, &ChartWork::SetSamplingFrqe);
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    m_thread_communicate.quit();
    m_thread_communicate.wait();

    m_thread_filter.quit();
    m_thread_filter.wait();

    m_thread_filesave.quit();
    m_thread_filesave.wait();
}

void MainWindow::UpdateSerialPortInfo()
{
    QString _current_serialPort_name = ui->m_comboBox_ComNameSelect->currentText();

#ifdef Q_OS_WIN
    QList<ComInfo> com_info_list;
    HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
    SP_DEVINFO_DATA spDevInfoData = { 0 };

    //获取存在的串口信息
    hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        return;
    }
    spDevInfoData.cbSize = sizeof(spDevInfoData);
    for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
    {
        wchar_t desc[MAX_PATH] = { 0 };
        QString tmp_desc;
        char containerID[16] = { 0 };
        ComInfo tmp_com_info;

        DEVPROPTYPE PropertyType = 0;

        //获取串口描述
        if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)desc, MAX_PATH - 1, NULL))
        {
            tmp_desc = QString::fromWCharArray(desc);
        }
        else if(SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)desc, MAX_PATH - 1, NULL))
        {
            tmp_desc = QString::fromWCharArray(desc);
        }
        else
        {
            continue;
        }

        //container_id
        if (SetupDiGetDevicePropertyW(hDevInfo, &spDevInfoData, &DEVPKEY_Device_ContainerId, &PropertyType, (PBYTE)containerID, 16, NULL, NULL))
        {
            memcpy(tmp_com_info.m_container_id, containerID, 16);
        }
        else
        {
            continue;
        }
        int tmp_COM_index = tmp_desc.indexOf("COM");

        tmp_com_info.m_com_desc = tmp_desc;
        tmp_com_info.m_com_name = tmp_desc.mid(tmp_COM_index, 4);
        tmp_com_info.index = tmp_com_info.m_com_name.mid(3, 1).toInt();
        com_info_list.push_back(tmp_com_info);
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
    hDevInfo = INVALID_HANDLE_VALUE;
    //获取串口额外名称
    hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES);
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        return;
    }
    for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
    {
        char desc[MAX_PATH] = { 0 };
        char containerID[17] = { 0 };
        char classname[1024] = { 0 };
        DEVPROPTYPE PropertyType = 0;

        //获取containerID
        if (!SetupDiGetDevicePropertyW(hDevInfo, &spDevInfoData, &DEVPKEY_Device_ContainerId, &PropertyType, (PBYTE)containerID, 17, NULL, NULL))
        {
            continue;
        }
        //获取classname
        if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &spDevInfoData, SPDRP_CLASS, NULL, (PBYTE)classname, 1024, NULL))
        {
            continue;
        }

        for(int i = 0; i < com_info_list.size(); i++)
        {
            if(strlen(containerID) != 0
                    && strcmp(classname, "Bluetooth") == 0
                    && strcmp(com_info_list.at(i).m_container_id, containerID) == 0)
            {
                //获取串口描述
                if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)desc, MAX_PATH - 1, NULL))
                {
                    com_info_list[i].m_com_desc = QString(desc).simplified();
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);

    std::sort(com_info_list.begin(), com_info_list.end(), [](const ComInfo & com_info1, const ComInfo & com_info2)
    {
        return com_info1.index < com_info2.index;
    });

    ui->m_comboBox_ComNameSelect->clear();

    for(int i = 0; i < com_info_list.size(); i++)
    {
        ui->m_comboBox_ComNameSelect->addItem(com_info_list.at(i).m_com_name + ":" + com_info_list.at(i).m_com_desc);
    }

    for(int i = 0; i < ui->m_comboBox_ComNameSelect->count(); i++)
    {
        if(_current_serialPort_name == ui->m_comboBox_ComNameSelect->itemText(i))
        {
            ui->m_comboBox_ComNameSelect->setCurrentIndex(i);
            break;
        }
    }

#else
    QStringList SerialPortInfo;
    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts())
    {
        SerialPortInfo << info.portName();
    }
    ui->m_comboBox_ComNameSelect->addItems(SerialPortInfo);
#endif
}


void MainWindow::on_chart_setting_triggered()
{
    m_Dlg_chartsetting.SetCurrentChartData(m_chartdata);
    m_Dlg_chartsetting.show();
}


void MainWindow::on_m_pushbutton_OpenCom_clicked()
{
    if(ui->m_pushbutton_OpenCom->text() == "打开串口")
    {
        SerialPortData _serialport_data;

        _serialport_data.Baut = ui->m_comboBox_BautSelect->currentText().toInt();
        _serialport_data.PortName = ui->m_comboBox_ComNameSelect->currentText().mid(0, 4);
        _serialport_data.DataBits = QSerialPort::Data8;
        _serialport_data.FlowControl = QSerialPort::NoFlowControl;
        _serialport_data.StopBits = QSerialPort::OneStop;
        _serialport_data.Parity = QSerialPort::NoParity;

        emit sig_Set_serialport(_serialport_data);
        emit sig_Open_serialport();
    }
    else
    {
        emit sig_Close_serialport();
    }
}


void MainWindow::on_m_pushbutton_SettingDevice_clicked()
{
    QByteArray setdevice_message;

    //配置
    setdevice_message.push_back(0x5A);
    setdevice_message.push_back(0x5A);
    setdevice_message.push_back(0x5A);
    setdevice_message.push_back(0x01);
    setdevice_message.push_back(0x02);
    setdevice_message.push_back(0x03);
    setdevice_message.push_back(char(0x00));
    setdevice_message.push_back(0x01);
    setdevice_message.push_back(char(0x00));
    setdevice_message.push_back(0x03);
    setdevice_message.push_back(char(0x00));

//    quint8 open_ch = 0x01 << ui->m_comboBox_ChannelSelect->currentIndex();
//    setdevice_message.push_back(open_ch);
    setdevice_message.push_back(0xFF);

    quint32 freq = ui->m_comboBox_freq->currentText().toUInt();
    switch (freq)
    {
        case 250:
            setdevice_message.push_back(0x01);
            emit sig_Set_filter_sampling(Freq250);
            break;
        case 500:
            setdevice_message.push_back(0x02);
            emit sig_Set_filter_sampling(Freq500);
            break;
        case 1000:
            setdevice_message.push_back(0x04);
            emit sig_Set_filter_sampling(Freq1000);
            break;
        default:
            QMessageBox::critical(this, "error", "采样率设置错误！");
            break;
    }

    setdevice_message.push_back(checkParity(setdevice_message));

    emit sig_Set_deivce(setdevice_message);
}


void MainWindow::on_m_pushbutton_OpenDevice_clicked()
{
    emit sig_Open_device();
}


void MainWindow::on_m_pushbutton_CloseDevice_clicked()
{
    emit sig_Close_device();
}


void MainWindow::on_m_radioButton_filter_clicked(bool checked)
{
    if(checked)
    {
        //开滤波器
        //disconnect 绘图
        disconnect(&m_communicate, &communicate::sig_recv_ch_data, ui->m_chart, &ChartWork::AddPoints);
        connect(&m_communicate, &communicate::sig_recv_ch_data, &m_filter_work, &filterWork::DoFilter);
        //绘图
        connect(&m_filter_work, &filterWork::sig_Filter_output, ui->m_chart, &ChartWork::AddPoints);
    }
    else
    {
        //不开滤波器
        disconnect(&m_communicate, &communicate::sig_recv_ch_data, &m_filter_work, &filterWork::DoFilter);
        //connect 绘图
        connect(&m_communicate, &communicate::sig_recv_ch_data, ui->m_chart, &ChartWork::AddPoints);
    }
}


void MainWindow::on_m_radioButton_record_clicked(bool checked)
{
    if(checked)
    {
        emit sig_Start_savedata();
        connect(&m_communicate, &communicate::sig_recv_ch_data, &m_file_save, &FileSave::SaveData);
    }
    else
    {
        emit sig_Stop_savedata();
        disconnect(&m_communicate, &communicate::sig_recv_ch_data, &m_file_save, &FileSave::SaveData);
    }
}

void MainWindow::printMessage(QString prefix, QByteArray hexMessage)
{
    QString printString = QString("%1:").arg(prefix);
    for (auto iter = hexMessage.begin(); iter != hexMessage.end(); iter++)
    {
        printString += QString::asprintf("%02X ", (unsigned char) * iter);
    }
    qDebug() << printString.toUtf8().data();
}

void MainWindow::on_cmd_switch_triggered(bool checked)
{

    ui->cmd_switch->setChecked(!checked);
    if(checked)
    {
        ui->cmd_switch->setText("控制台:开");
        connect(&m_communicate, &communicate::sig_print_message, this, &MainWindow::printMessage);
    }
    else
    {
        ui->cmd_switch->setText("控制台:关");
        disconnect(&m_communicate, &communicate::sig_print_message, this, &MainWindow::printMessage);
    }
}

void MainWindow::on_m_comboBox_ChannelSelect_currentIndexChanged(int index)
{
    if(index < TOTAL_CH_NUM)
    {
        ui->m_chart->SetChIndex(index);
    }
}

