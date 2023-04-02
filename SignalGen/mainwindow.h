#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "communicate.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

#include "communicate.h"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private slots:

    void on_m_pushbutton_OpenCom_clicked();

private:
    void UpdateSerialPortInfo();
    QTimer m_timer_update_serialport_info;
    Communicate m_com;
    QThread m_com_thread;
private:
    Ui::MainWindow* ui;
signals:
    void sig_Open_serialport(SerialPortData);
    void sig_Close_serialport();
};
#endif // MAINWINDOW_H
