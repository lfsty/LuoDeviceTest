#ifndef DEVICESETTING_H
#define DEVICESETTING_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "chartsetting.h"

//绘图
#include <QChart>
#include <QLegendMarker>
#include <QPointF>
#include <QSplineSeries>
#include <QTimer>
#include <QValueAxis>

//数据保存
#include <QDir>
#include <QFile>
#include <QDateTime>

#define BYTE quint8
#define UINT quint32
#define UINT16 quint16
#define UINT8 quint8
#define UINT64 quint64

#define TOTAL_CH_NUM 8

QT_BEGIN_NAMESPACE
namespace Ui
{
    class DeviceSetting;
}
QT_END_NAMESPACE

class DeviceSetting : public QMainWindow
{
    Q_OBJECT

public:
    DeviceSetting( QWidget* parent = nullptr );
    ~DeviceSetting();
private slots:
    void on_m_pushbutton_SettingDevice_clicked();

    void on_m_pushbutton_OpenDevice_clicked();

    void on_m_pushbutton_CloseDevice_clicked();

    void on_m_pushbutton_OpenCom_clicked();

    void recv_data();

    void update_paint();

    void on_draw_switch_triggered();

    void on_cmd_switch_triggered();

    void on_chart_setting_triggered();

    void chart_data_changed(ChartSettingData chart_data);

    void on_m_comboBox_ChannelSelect_currentIndexChanged(int index);

    void on_radioButton_clicked(bool checked);

private:
    QSerialPort m_serialPort;
    QByteArray  m_opendevice_message;
    QByteArray  m_closedevice_message;
    //数据接收缓冲区
    QByteArray buffer;
    //采样率
    quint32 freq       = 1000;
    double  m_now_time = 0;

    bool m_is_cmd_output_on = true;
    bool m_is_paint_on      = true;
    bool m_is_device_open   = false;

    quint8 checkParity( QByteArray data );
    void   setSerial();
    void   sendMessage( QByteArray hexMessage );
    void   printMessage( QString prefix, QByteArray hexMessage );
    void   parseFrame( const QByteArray& frameData );

private:
    //绘图
    QTimer           m_updateTimer;
    QChart*          m_chart         = nullptr;
    QSplineSeries*   m_spline_series = nullptr;
    QValueAxis*      m_axisY         = nullptr;
    QValueAxis*      m_axisX         = nullptr;
    QList<QPointF>   m_point_data;
    void             addNewPoint( QPointF new_point );
    void             initChart();
    ChartSetting*    m_chartsetting = nullptr;
    ChartSettingData m_chartdata;
    int m_disp_ch_index = 0;
    //数据保存
    QFile m_qfile_array[TOTAL_CH_NUM];
    bool m_is_save_to_file = false;

private:
    Ui::DeviceSetting* ui;
};
#endif // DEVICESETTING_H
