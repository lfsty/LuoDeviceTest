#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_filter_work(TOTAL_CH_NUM)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_cmd_switch_triggered(true);

    {
        // 图标设置对话框初始化
        m_Dlg_chartsetting.SetCurrentChartData(ui->m_chart->GetChartSettingData());
        connect(&m_Dlg_chartsetting, &ChartSetting::SettingChanged, ui->m_chart, &ChartWork::ChartSettingChange);
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
        m_com_communicate.MoveToThead(&m_thread_communicate);
        connect(this, &MainWindow::sig_Set_serialport, &m_com_communicate, &ComCommunicate::SetSerialPortData);
        connect(this, &MainWindow::sig_Open_serialport, &m_com_communicate, &ComCommunicate::OpenSerialPort);
        connect(this, &MainWindow::sig_Close_serialport, &m_com_communicate, &ComCommunicate::CloseSerialPort);
        //串口打开事件
        connect(&m_com_communicate, &ComCommunicate::sig_serialport_open, this, [ = ]()
        {
            ui->m_comboBox_BautSelect->setEnabled(false);
            ui->m_comboBox_ComNameSelect->setEnabled(false);
            ui->m_pushbutton_OpenCom->setText("关闭串口");

            ui->m_radioButton_record->setEnabled(true);
            ui->m_comboBox_ChannelSelect->setEnabled(true);
            ui->m_comboBox_freq->setEnabled(true);
            ui->m_lcdNumber_frame_count->setEnabled(true);
            ui->m_pushButton_filter->setEnabled(true);
//            ui->m_pushbutton_OpenDevice->setEnabled(false);
            ui->m_pushbutton_CloseDevice->setEnabled(true);
            SetUpDevice();
        });
        //串口关闭事件
        connect(&m_com_communicate, &ComCommunicate::sig_serialport_close, this, [ = ]()
        {
            ui->m_comboBox_BautSelect->setEnabled(true);
            ui->m_comboBox_ComNameSelect->setEnabled(true);
            ui->m_pushbutton_OpenCom->setText("打开串口");

            ui->m_radioButton_record->setEnabled(false);
            ui->m_comboBox_ChannelSelect->setEnabled(false);
            ui->m_comboBox_freq->setEnabled(false);
            ui->m_lcdNumber_frame_count->setEnabled(false);
            ui->m_pushButton_filter->setEnabled(false);
            ui->m_pushbutton_OpenDevice->setEnabled(false);
            ui->m_pushbutton_CloseDevice->setEnabled(false);
        });
        connect(this, &MainWindow::sig_Set_deivce, &m_com_communicate, &ComCommunicate::SetDeivce);
        connect(ui->m_comboBox_freq, &QComboBox::currentTextChanged, this, [ = ](const QString & arg)
        {
            SetUpDevice();
        });
        connect(&m_com_communicate, &ComCommunicate::sig_device_set, this, [ = ]()
        {
            //配置成功事件
            ui->m_pushbutton_OpenDevice->setEnabled(true);
        });
        connect(this, &MainWindow::sig_Open_device, &m_com_communicate, &ComCommunicate::OpenDevice);
        connect(&m_com_communicate, &ComCommunicate::sig_device_open, this, [ = ]()
        {
            ui->m_comboBox_freq->setEnabled(false);
            ui->m_pushbutton_OpenDevice->setEnabled(false);
            ui->m_pushbutton_OpenCom->setEnabled(false);
        });
        connect(this, &MainWindow::sig_Close_device, &m_com_communicate, &ComCommunicate::CloseDeivce);
        connect(&m_com_communicate, &ComCommunicate::sig_device_close, this, [ = ]()
        {
            ui->m_comboBox_freq->setEnabled(true);
            ui->m_pushbutton_OpenDevice->setEnabled(true);
            ui->m_pushbutton_OpenCom->setEnabled(true);
        });
        connect(&m_com_communicate, &ComCommunicate::sig_device_close, ui->m_chart, &ChartWork::ClearPlot);
        // 校验位错误
        connect(&m_com_communicate, &ComCommunicate::sig_parity_error, this, [ = ]()
        {
            m_parity_error_num++;
            ui->m_label_errorNum_parity->setText("奇偶校验错误数：" + QString::number(m_parity_error_num));
        });
        // 序列号错误
        connect(&m_com_communicate, &ComCommunicate::sig_serialNum_error, this, [ = ]()
        {
            m_sync_error_num++;
            ui->m_label_errorNum_sync->setText("同步帧错误数：" + QString::number(m_sync_error_num));
        });
        //同步位错误
        connect(&m_com_communicate, &ComCommunicate::sig_sync_error, this, [ = ]()
        {
            m_serialNum_error++;
            ui->m_label_errorNum_serial->setText("序列号错误数：" + QString::number(m_serialNum_error));
        });
        connect(&m_com_communicate, &ComCommunicate::sig_recv_frame, this, [ = ]()
        {
            m_frame_count++;
        });
        m_thread_communicate.start();
    }

    {
        // 滤波设置
        m_filter_work.moveToThread(&m_thread_filter);
        connect(this, &MainWindow::sig_Set_filter_sampling, &m_filter_work, &filterWork::SetSamplingFreq);
        connect(&m_com_communicate, &ComCommunicate::sig_recv_ch_data, &m_filter_work, &filterWork::DoFilter);

        connect(&m_Dlg_filtersetting, &FilterSetting::sig_FilterEnabled, &m_filter_work, &filterWork::OnFilterEnabled);
        connect(&m_Dlg_filtersetting, &FilterSetting::sig_Filter_lowpass_enabled, &m_filter_work, &filterWork::OnFilterLowPassEnalbed);
        connect(&m_Dlg_filtersetting, &FilterSetting::sig_Filter_highpass_enabled, &m_filter_work, &filterWork::OnFilterHighPassEnabled);
        connect(&m_Dlg_filtersetting, &FilterSetting::sig_Filter_notch_enabled, &m_filter_work, &filterWork::OnFilterNotchEnabled);
        connect(&m_Dlg_filtersetting, &FilterSetting::sig_Filter_highpass_impen_enabled, &m_filter_work, &filterWork::OnFilterHighPassImpenEnabled);

        connect(&m_Dlg_filtersetting, &FilterSetting::sig_Filter_Changed, &m_filter_work, &filterWork::OnFilterChanged);

        m_Dlg_filtersetting.init();

        //绘图
        connect(&m_filter_work, &filterWork::sig_Filter_output, ui->m_chart, &ChartWork::AddPoints);
//        connect(this, &MainWindow::sig_Filter_enabled, &m_filter_work, &filterWork::SetFilterEnabled);
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
        ui->m_label_total_ch_yRange->setVisible(false);
        connect(ui->m_chart, &ChartWork::sig_Total_Ch_yRange_change, this, [ = ](int yRange)
        {
            ui->m_label_total_ch_yRange->setText("显示范围：" + QString::number(yRange));
        });
    }

    {
        //初始化按钮设置
        ui->m_radioButton_record->setEnabled(false);
        ui->m_comboBox_ChannelSelect->setEnabled(false);
        ui->m_comboBox_freq->setEnabled(false);
        ui->m_lcdNumber_frame_count->setEnabled(false);
        ui->m_pushButton_filter->setEnabled(false);
        ui->m_pushbutton_OpenDevice->setEnabled(false);
        ui->m_pushbutton_CloseDevice->setEnabled(false);
    }

    {
        m_tcpserver_communcate.MoveToThread(&m_thread_tcpserver_communicate);
        //tcp 信号输出初始化
        m_Dlg_tcpexport.setModal(false);
        //信号数据传递
        connect(&m_com_communicate, &ComCommunicate::sig_recv_ch_data, &m_tcpserver_communcate, &TCPServerCommunicate::ExportData);
        connect(&m_Dlg_tcpexport, &SigTCPExport::sig_open_TCPServer, &m_tcpserver_communcate, &TCPServerCommunicate::OpenTCPServer);
        connect(&m_Dlg_tcpexport, &SigTCPExport::sig_close_TCPServer, &m_tcpserver_communcate, &TCPServerCommunicate::CloseTCPServer);
        connect(&m_tcpserver_communcate, &TCPServerCommunicate::sig_TCPServerStatusChanged, &m_Dlg_tcpexport, &SigTCPExport::TCPServerStatusChanged);
        m_thread_tcpserver_communicate.start();
    }


//    m_Dlg_filtersetting.show();
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

    m_thread_tcpserver_communicate.quit();
    m_thread_tcpserver_communicate.wait();
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

    QVector<QString> _new_serialport_item;
    for(int i = 0; i < com_info_list.size(); i++)
    {
        _new_serialport_item.push_back(com_info_list.at(i).m_com_name + ":" + com_info_list.at(i).m_com_desc);
    }


    bool _is_serialInfo_changed = false;
    if(_new_serialport_item.size() != ui->m_comboBox_ComNameSelect->count())
    {
        _is_serialInfo_changed = true;
    }
    else
    {
        for(int i = 0; i < _new_serialport_item.size(); i++)
        {
            if(_new_serialport_item[i] != ui->m_comboBox_ComNameSelect->itemText(i))
            {
                _is_serialInfo_changed = true;
                break;
            }
        }
    }


    if(_is_serialInfo_changed)
    {
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

void MainWindow::SetUpDevice()
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


void MainWindow::on_chart_setting_triggered()
{
    m_Dlg_chartsetting.SetCurrentChartData(ui->m_chart->GetChartSettingData());
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

void MainWindow::on_m_pushbutton_OpenDevice_clicked()
{
    emit sig_Open_device();
}


void MainWindow::on_m_pushbutton_CloseDevice_clicked()
{
    emit sig_Close_device();
}

void MainWindow::on_m_radioButton_record_clicked(bool checked)
{
    if(checked)
    {
        emit sig_Start_savedata(ui->m_comboBox_freq->currentText().toUInt());
        connect(&m_com_communicate, &ComCommunicate::sig_recv_ch_data_raw, &m_file_save, &FileSave::SaveData);
    }
    else
    {
        emit sig_Stop_savedata();
        disconnect(&m_com_communicate, &ComCommunicate::sig_recv_ch_data_raw, &m_file_save, &FileSave::SaveData);
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
        connect(&m_com_communicate, &ComCommunicate::sig_print_message, this, &MainWindow::printMessage);
    }
    else
    {
        ui->cmd_switch->setText("控制台:关");
        disconnect(&m_com_communicate, &ComCommunicate::sig_print_message, this, &MainWindow::printMessage);
    }
}

void MainWindow::on_m_comboBox_ChannelSelect_currentIndexChanged(int index)
{
    ui->m_chart->SetChIndex(index);
    if(index < TOTAL_CH_NUM)
    {
        m_Dlg_chartsetting.SetAllChannelEnabled(false);
        ui->m_label_total_ch_yRange->setVisible(false);
    }
    else
    {
        //显示所有导联
        m_Dlg_chartsetting.SetAllChannelEnabled(true);
        ui->m_label_total_ch_yRange->setVisible(true);
    }
}


void MainWindow::on_draw_switch_triggered(bool checked)
{
    ui->cmd_switch->setChecked(!checked);
    if(checked)
    {
        ui->m_chart->StartPlot();
    }
    else
    {
        ui->m_chart->StopPlot();
    }
}


void MainWindow::on_m_pushButton_filter_clicked()
{
    m_Dlg_filtersetting.show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_Dlg_filtersetting.close();
    m_Dlg_chartsetting.close();
    m_Dlg_tcpexport.OnQuit();
    m_Dlg_tcpexport.close();
}

void MainWindow::on_m_pushButton_errorNum_clear_clicked()
{
    m_parity_error_num = 0;
    m_sync_error_num = 0;
    m_serialNum_error = 0;
    ui->m_label_errorNum_parity->setText("奇偶校验错误数：" + QString::number(m_parity_error_num));
    ui->m_label_errorNum_sync->setText("同步帧错误数：" + QString::number(m_sync_error_num));
    ui->m_label_errorNum_serial->setText("序列号错误数：" + QString::number(m_serialNum_error));
}


void MainWindow::on_m_pushButton_sig_export_clicked()
{
    m_Dlg_tcpexport.show();
}

