#ifndef DEVICESETTING_H
#define DEVICESETTING_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

//绘图
#include <QTimer>
#include <QChart>
#include <QSplineSeries>
#include <QValueAxis>
#include <QLegendMarker>
#include <QPointF>

#define BYTE quint8
#define UINT quint32
#define UINT16 quint16
#define UINT8 quint8
#define UINT64 quint64

#define CHART_X_DISP_S 5

QT_BEGIN_NAMESPACE
namespace Ui { class DeviceSetting; }
QT_END_NAMESPACE

class DeviceSetting : public QMainWindow
{
    Q_OBJECT

public:
    DeviceSetting(QWidget *parent = nullptr);
    ~DeviceSetting();

private slots:
    void on_m_pushbutton_SettingDevice_clicked();

    void on_m_pushbutton_OpenDevice_clicked();

    void on_m_pushbutton_CloseDevice_clicked();

    void on_m_pushbutton_OpenCom_clicked();

    void recv_data();

    void update_paint();

    void on_m_pushbutton_cmd_output_clicked();

    void on_m_pushbutton_paint_clicked();

private:
    QSerialPort m_serialPort;
    QByteArray m_opendevice_message;
    QByteArray m_closedevice_message;
    //数据接收缓冲区
    QByteArray buffer;
    //采样率
    quint32 freq = 1000;
    double m_now_time = 0;

    bool m_is_cmd_output_on = true;
    bool m_is_paint_on = true;
    bool m_is_device_open = false;

    quint8 checkParity(QByteArray data);
    void setSerial();
    void sendMessage(QByteArray hexMessage);
    void printMessage(QString prefix,QByteArray hexMessage);
    void parseFrame(const QByteArray& frameData);
private:
    //绘图
    QTimer m_updateTimer;
    QChart *m_chart = nullptr;
    QSplineSeries *m_spline_series = nullptr;
    QValueAxis *m_axisY = nullptr;
    QValueAxis *m_axisX = nullptr;
    QList<QPointF> m_point_data;
    void addNewPoint(QPointF new_point);
    void initChart();
private:
    Ui::DeviceSetting *ui;
};
#endif // DEVICESETTING_H
