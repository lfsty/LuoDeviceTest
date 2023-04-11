#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Chart/chartsetting.h"
#include <QTimer>
#include "Communicate/comcommunicate.h"
#include "ConstData.h"
#include <QMessageBox>
#include "Filter/filterwork.h"
#include "FileSave/filesave.h"
#include "Filter/filtersetting.h"

#include "Communicate/sigtcpexport.h"
#include "Communicate/tcpservercommunicate.h"
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
    void SetUpDevice();

private slots:
    void on_chart_setting_triggered();

    void on_m_pushbutton_OpenCom_clicked();

    void on_m_pushbutton_OpenDevice_clicked();

    void on_m_pushbutton_CloseDevice_clicked();

    void on_m_radioButton_record_clicked(bool checked);

    void printMessage(QString prefix, QByteArray hexMessage);

    void on_cmd_switch_triggered(bool checked);

    void on_m_comboBox_ChannelSelect_currentIndexChanged(int index);

    void on_draw_switch_triggered(bool checked);

    void on_m_pushButton_filter_clicked();

    void closeEvent(QCloseEvent* event);

    void on_m_pushButton_errorNum_clear_clicked();

    void on_m_pushButton_sig_export_clicked();

private:
    // 绘图设置对话框
    ChartSetting    m_Dlg_chartsetting;
//    ChartSettingData m_chartdata;

    // 串口信息刷新
    QTimer m_timer_update_serialport_info;

    // 串口通信
    ComCommunicate m_com_communicate;
    QThread m_thread_communicate;

    // 帧数统计
    int m_frame_count = 0;
    QTimer m_frame_timer;

    // 滤波
    filterWork m_filter_work;
    QThread m_thread_filter;
    FilterSetting m_Dlg_filtersetting;

    // 文件保存
    FileSave m_file_save;
    QThread m_thread_filesave;

    // tcp信号输出
    SigTCPExport m_Dlg_tcpexport;
    QThread m_thread_tcpserver_communicate;
    TCPServerCommunicate m_tcpserver_communcate;

signals:
    void sig_Set_serialport(SerialPortData);
    void sig_Open_serialport();
    void sig_Close_serialport();
    void sig_Set_deivce(QByteArray);
    void sig_Open_device();
    void sig_Close_device();
    void sig_Filter_enabled(bool);

    void sig_Start_savedata();
    void sig_Stop_savedata();
    void sig_Set_filter_sampling(SamplingFreq);

private:
    Ui::MainWindow* ui;
    // 报错数据统计
    quint64 m_parity_error_num = 0;
    quint64 m_sync_error_num = 0;
    quint64 m_serialNum_error = 0;
};
#endif // MAINWINDOW_H
