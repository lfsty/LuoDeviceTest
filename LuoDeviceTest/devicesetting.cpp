#include "devicesetting.h"
#include "ui_devicesetting.h"
#include <QDebug>

DeviceSetting::DeviceSetting(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DeviceSetting)
{
    ui->setupUi(this);
    connect(&m_serialPort,&QSerialPort::readyRead,this,&DeviceSetting::recv_data);

    QStringList SerialPortInfo;
    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        SerialPortInfo<<info.portName();
    }
    ui->m_comboBox_ComNameSelect->addItems(SerialPortInfo);

    ui->m_comboBox_BautSelect->addItem("460800");

    for(int i =1;i<=8;i++){
        ui->m_comboBox_ChannelSelect->addItem(QString("导联：%1").arg(i));
    }

    ui->m_comboBox_freq->addItem("250");
    ui->m_comboBox_freq->addItem("500");
    ui->m_comboBox_freq->addItem("1000");
    ui->m_comboBox_freq->setCurrentText("1000");

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

    initChart();

    connect(&m_updateTimer,&QTimer::timeout,this,&DeviceSetting::update_paint);
    m_updateTimer.setInterval(50);

}

DeviceSetting::~DeviceSetting()
{
    delete ui;
    if(m_chart != nullptr)
    {
        m_chart->deleteLater();
    }

    if(m_spline_series != nullptr)
    {
        m_spline_series->deleteLater();
    }
    if(m_axisY != nullptr)
    {
        m_axisY->deleteLater();
    }
    if(m_axisX != nullptr)
    {
        m_axisX->deleteLater();
    }
}

void DeviceSetting::setSerial()
{
    qint32 _baut = ui->m_comboBox_BautSelect->currentText().toInt();
    QString _portName = ui->m_comboBox_ComNameSelect->currentText();
    QSerialPort::DataBits _dataBits = QSerialPort::Data8;
    QSerialPort::FlowControl _flowControl = QSerialPort::NoFlowControl;
    QSerialPort::StopBits _stopBits = QSerialPort::OneStop;
    QSerialPort::Parity _parity = QSerialPort::NoParity;

    m_serialPort.setPortName(_portName);
    m_serialPort.setBaudRate(_baut);
    m_serialPort.setDataBits(_dataBits);
    m_serialPort.setFlowControl(_flowControl);
    m_serialPort.setStopBits(_stopBits);
    m_serialPort.setParity(_parity);
}

void DeviceSetting::sendMessage(QByteArray hexMessage)
{
    if(m_serialPort.isOpen()){
        m_serialPort.write(hexMessage);
    }else{
        qDebug()<<"消息未发送，设备未打开";
    }
}


void DeviceSetting::printMessage(QString prefix, QByteArray hexMessage)
{
    if(m_is_cmd_output_on){
        QString printString = QString("%1:").arg(prefix);
        for(auto iter = hexMessage.begin();iter != hexMessage.end();iter++){
            printString += QString::asprintf("%02X ",(unsigned char)*iter);
        }
        qDebug()<<printString.toUtf8().data();
    }
}

void DeviceSetting::parseFrame(const QByteArray& frameData)
{
    //奇偶校验
    BYTE parity = 0;
    int FrameLen = frameData.size();
    for(int i = 0; i < FrameLen - 1; i++) {
        parity ^= frameData.at(i);
    }
    if (parity != BYTE(frameData.at(FrameLen - 1))) {
        QMessageBox::critical(this,"error","奇偶校验错误!"+QString::asprintf("%02X",BYTE(frameData.at(FrameLen - 1))));
    }

    UINT read_buf_index = 0;

    //3字节同步帧检验
    for (int i = 0; i < 3; i++) {
        if (frameData.at(read_buf_index++) != 0x5A) {
            QMessageBox::critical(this,"error","同步帧出错!");
        }
    }

    //3字节设备号
    for (int i = 0; i < 3; i++) {
        //目前设备号不做判断
        //bufferVect[read_buf_index++];
        read_buf_index++;
    }

    //2字节命令名称
    UINT16 CommandName = frameData.at(read_buf_index++) << 8 | frameData.at(read_buf_index++);
    //2字节命令长度
    UINT16 CommandLen = frameData.at(read_buf_index++) << 8 | frameData.at(read_buf_index++);
    //目前只有00,01,02
    switch (CommandName) {
    case 0x0001: {
        //配置
        //2字节开启导联，高地址为1
        UINT16 OpenChIndex = frameData.at(read_buf_index++) << 8 | frameData.at(read_buf_index++);
        //1字节采样速率
        //1=250;2=500;4=1000
        UINT8 FreqCommand = frameData.at(read_buf_index++);
        break;
    }
    case 0x0002: {
        //数据
        //2字节序列号
        static UINT16 FrameNumIndex = frameData.at(read_buf_index) << 8 | frameData.at(read_buf_index + 1);
        static UINT64 errornum = 0;
        UINT16 recvIndex = frameData.at(read_buf_index++) << 8 | frameData.at(read_buf_index++);

        if (FrameNumIndex != recvIndex) {
            FrameNumIndex = recvIndex;
            errornum++;
        }
        FrameNumIndex++;

        if (FrameNumIndex == 0) {
            FrameNumIndex++;
        }

        static auto _24Bit2Int = [](const BYTE data[3])->int {
            int num = (data[0] << 16) | (data[1] << 8) | (data[2]);
            //判断正负
            if ((data[0] & 0x80) == 0x80) {
                num |= 0xFF000000;
            }
            return num;
        };
        //3字节有符号整形解析
        UINT8 ChNum = (CommandLen - 2) / 3;
        for (int i = 0; i < ChNum; i++) {
            BYTE data_24bit[3];
            data_24bit[0] = frameData.at(read_buf_index++);
            data_24bit[1] = frameData.at(read_buf_index++);
            data_24bit[2] = frameData.at(read_buf_index++);
            float ch_data = _24Bit2Int(data_24bit);

            float d_float = ch_data / 16777216 * 4500 / 24 * 1000;
            addNewPoint(QPointF(m_now_time,d_float));
            m_now_time += 1.0/freq;

        }
        //数据
        break;
    }
    case 0x0003: {
        //停止
        //bufferVect[read_buf_index++] == 0x03;
        break;
    }
    default: {
        QMessageBox::critical(this,"error","命令名称出错!");
        return;
        break;
    }
    }
}



void DeviceSetting::initChart()
{
    if(m_chart == nullptr)
    {
        m_chart = new QChart();
        // 将Chart添加到ChartView
        ui->m_chartview->setChart(m_chart);
        ui->m_chartview->setRenderHint(QPainter::Antialiasing);
        m_chart->legend()->setVisible(false);
        // 设置图表主题色
//            ui->graphicsView->chart()->setTheme(QChart::ChartTheme(0));
    }
    if(m_spline_series == nullptr)
    {
        m_spline_series = new QSplineSeries(m_chart);
        m_chart->addSeries(m_spline_series);
    }
    if(m_axisY == nullptr)
    {
        m_axisY = new QValueAxis(m_chart);
        m_axisY->setRange(-50,50);
        m_chart->addAxis(m_axisY, Qt::AlignLeft);
        m_spline_series->attachAxis(m_axisY);
    }
    if(m_axisX == nullptr)
    {
        m_axisX = new QValueAxis(m_chart);
        m_axisX->setRange(0, CHART_X_DISP_S);
        m_chart->addAxis(m_axisX, Qt::AlignBottom);
        m_spline_series->attachAxis(m_axisX);
    }
}

quint8 DeviceSetting::checkParity(QByteArray data)
{
    quint8 parity = 0;
    for(auto iter = data.begin();iter!= data.end();iter++){
        parity ^= *iter;
    }
    return parity;
}


void DeviceSetting::on_m_pushbutton_SettingDevice_clicked()
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

    quint8 open_ch = 0x01<<ui->m_comboBox_ChannelSelect->currentIndex();
    setdevice_message.push_back(open_ch);

    freq = ui->m_comboBox_freq->currentText().toUInt();
    switch(freq){
    case 250:
        setdevice_message.push_back(0x01);
        break;
    case 500:
        setdevice_message.push_back(0x02);
        break;
    case 1000:
        setdevice_message.push_back(0x04);
        break;
    default:
        QMessageBox::critical(this,"error","采样率设置错误！");
        break;
    }
    setdevice_message.push_back(checkParity(setdevice_message));

    printMessage("设置设备,发送",setdevice_message);
    sendMessage(setdevice_message);
}


void DeviceSetting::on_m_pushbutton_OpenDevice_clicked()
{
    printMessage("开启设备,发送",m_opendevice_message);
    sendMessage(m_opendevice_message);
    ui->m_pushbutton_SettingDevice->setEnabled(false);
    m_updateTimer.start();
    ui->m_comboBox_ChannelSelect->setEnabled(false);
    ui->m_comboBox_freq->setEnabled(false);
    m_is_device_open = true;
}


void DeviceSetting::on_m_pushbutton_CloseDevice_clicked()
{
    printMessage("关闭设备,发送",m_closedevice_message);
    sendMessage(m_closedevice_message);
    buffer.clear();
    ui->m_pushbutton_SettingDevice->setEnabled(true);
    m_now_time = 0;
    m_axisX->setRange(0, CHART_X_DISP_S);
    m_point_data.clear();
    m_updateTimer.stop();
    ui->m_comboBox_ChannelSelect->setEnabled(true);
    ui->m_comboBox_freq->setEnabled(true);
    m_is_device_open = false;
}

void DeviceSetting::on_m_pushbutton_OpenCom_clicked()
{
    if(!m_serialPort.isOpen()){
        setSerial();
        m_serialPort.open(QIODevice::ReadWrite);
        if(!m_serialPort.isOpen())
        {
            QMessageBox::critical(this,"error","串口打开错误!");
        }else{
            ui->m_comboBox_BautSelect->setEnabled(false);
            ui->m_comboBox_ComNameSelect->setEnabled(false);
            ui->m_pushbutton_OpenCom->setText("关闭串口");
        }
    }else{
        m_serialPort.close();
        ui->m_comboBox_BautSelect->setEnabled(true);
        ui->m_comboBox_ComNameSelect->setEnabled(true);
        ui->m_pushbutton_OpenCom->setText("打开串口");
    }
}

void DeviceSetting::recv_data()
{
    QByteArray data = m_serialPort.readAll();

    buffer.append(data);

    while(!buffer.isEmpty()){
        if(buffer.at(0) != 0x5A){
            qDebug()<<QString("寻找同步帧："+QString::asprintf("%02X",quint8(buffer.at(0)))).toUtf8().data();
            buffer.remove(0,1);
        }else{
            break;
        }
    }


    if(buffer.size()<10){
        return;
    }else{
        quint16 commamd_len = buffer.at(8) << 8 | buffer.at(9);
        quint64 frame_len = 10+commamd_len+1;
        if(buffer.size() < frame_len){
            return;
        } else{
            while(buffer.size() >= frame_len){
                //处理一帧数据
                QByteArray frameData = buffer.mid(0,frame_len);
                parseFrame(frameData);
                printMessage("收到数据",frameData);
                buffer.remove(0,frame_len);
            }
        }
    }
}
void DeviceSetting::addNewPoint(QPointF new_point)
{
    if(m_point_data.size()>freq*CHART_X_DISP_S){
        m_point_data.removeFirst();
    }
    m_point_data.append(new_point);
}

void DeviceSetting::update_paint()
{
    if(m_point_data.back().x() > CHART_X_DISP_S){
        m_axisX->setMin(m_point_data.front().x());
        m_axisX->setMax(m_point_data.back().x());
    }
    m_spline_series->replace(m_point_data);
}


void DeviceSetting::on_m_pushbutton_cmd_output_clicked()
{
    m_is_cmd_output_on = !m_is_cmd_output_on;
    if(m_is_cmd_output_on){
        ui->m_pushbutton_cmd_output->setText("控制台:开");
    }else{
        ui->m_pushbutton_cmd_output->setText("控制台:关");
    }
}


void DeviceSetting::on_m_pushbutton_paint_clicked()
{
    m_is_paint_on = !m_is_paint_on;
    if(m_is_paint_on){
        ui->m_pushbutton_paint->setText("绘图:开");
        if(m_is_device_open){
            m_updateTimer.start();
        }
    }else{
        ui->m_pushbutton_paint->setText("绘图:关");
        m_updateTimer.stop();
    }
}

