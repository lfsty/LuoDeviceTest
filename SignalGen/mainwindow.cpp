#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
        m_com.MoveToThead(&m_com_thread);
        //串口设置
        connect(this, &MainWindow::sig_Open_serialport, &m_com, &Communicate::OpenCom);
        connect(this, &MainWindow::sig_Close_serialport, &m_com, &Communicate::CloseCom);

        connect(&m_com, &Communicate::sig_serialport_open, this, [ = ]()
        {
            ui->m_pushbutton_OpenCom->setText("关闭串口");
        });
        connect(&m_com, &Communicate::sig_serialport_close, this, [ = ]()
        {
            ui->m_pushbutton_OpenCom->setText("打开串口");
        });
        m_com_thread.start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    m_com_thread.quit();
    m_com_thread.wait();
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


void MainWindow::on_m_pushbutton_OpenCom_clicked()
{
    if(ui->m_pushbutton_OpenCom->text() == "打开串口")
    {
        SerialPortData _serialport_data;

        _serialport_data.Baut = SERIALPORT_BAUT;
        _serialport_data.PortName = ui->m_comboBox_ComNameSelect->currentText().mid(0, 4);
        _serialport_data.DataBits = QSerialPort::Data8;
        _serialport_data.FlowControl = QSerialPort::NoFlowControl;
        _serialport_data.StopBits = QSerialPort::OneStop;
        _serialport_data.Parity = QSerialPort::NoParity;

        emit sig_Open_serialport(_serialport_data);
    }
    else
    {
        emit sig_Close_serialport();
    }
}

