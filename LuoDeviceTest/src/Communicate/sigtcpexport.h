#ifndef SIGTCPEXPORT_H
#define SIGTCPEXPORT_H

#include <QDialog>

namespace Ui
{
    class SigTCPExport;
}

class SigTCPExport : public QDialog
{
    Q_OBJECT

public:
    explicit SigTCPExport(QWidget* parent = nullptr);
    ~SigTCPExport();

private:
    Ui::SigTCPExport* ui;
public:
    void OnQuit();
public slots:
    void TCPServerStatusChanged(bool enable);
private slots:
    void on_m_pushButton_tcp_export_enable_clicked();
signals:
    void sig_open_TCPServer(const QString& ip_address, const quint16& port);
    void sig_close_TCPServer();
};

#endif // SIGTCPEXPORT_H
