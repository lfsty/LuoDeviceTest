#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Chart/chartsetting.h"
#include <QTimer>
#include "Communicate/communicate.h"
#include "ConstData.h"
#include <QMessageBox>
#include "Filter/filterwork.h"
#include "FileSave/filesave.h"

#ifdef Q_OS_WIN
//串口信息获取
#include <initguid.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <Devpkey.h>
#pragma comment(lib, "setupapi.lib")

struct ComInfo
{
    int index = 0;
    QString m_com_name;
    QString m_com_desc;
    char m_container_id[17] = {0};
};
#endif


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void UpdateSerialPortInfo();

private slots:
    void on_chart_setting_triggered();

    void on_m_pushbutton_OpenCom_clicked();

    void on_m_pushbutton_SettingDevice_clicked();

    void on_m_pushbutton_OpenDevice_clicked();

    void on_m_pushbutton_CloseDevice_clicked();

    void on_m_radioButton_filter_clicked(bool checked);

    void on_m_radioButton_record_clicked(bool checked);

    void printMessage(QString prefix, QByteArray hexMessage);

    void on_cmd_switch_triggered(bool checked);

    void on_m_comboBox_ChannelSelect_currentIndexChanged(int index);

private:
    // 绘图设置对话框
    ChartSetting    m_Dlg_chartsetting;
    ChartSettingData m_chartdata;

    // 串口信息刷新
    QTimer m_timer_update_serialport_info;

    // 串口通信
    communicate m_communicate;
    QThread m_thread_communicate;

    // 帧数统计
    int m_frame_count = 0;
    QTimer m_frame_timer;

    // 滤波
    filterWork m_filter_work;
    QThread m_thread_filter;

    // 文件保存
    FileSave m_file_save;
    QThread m_thread_filesave;

signals:
    void sig_Set_serialport(SerialPortData);
    void sig_Open_serialport();
    void sig_Close_serialport();
    void sig_Set_deivce(QByteArray);
    void sig_Open_device();
    void sig_Close_device();

    void sig_Start_savedata();
    void sig_Stop_savedata();
    void sig_Set_filter_sampling(SamplingFreq);

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
