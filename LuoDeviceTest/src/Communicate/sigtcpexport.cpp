#include "sigtcpexport.h"
#include "ui_sigtcpexport.h"
#include <QHostAddress>
#include <QNetworkInterface>
SigTCPExport::SigTCPExport(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SigTCPExport)
{
    ui->setupUi(this);

    //tcp设置
    ui->m_combobox_server_ip->setEditable(true);
    //获取本机ip地址
    ui->m_combobox_server_ip->addItem("0.0.0.0");
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for(const QHostAddress& ipaddress : ipAddressesList)
    {
        if(ipaddress.toIPv4Address())
        {
            ui->m_combobox_server_ip->addItem(ipaddress.toString());
        }
    }

    TCPServerStatusChanged(false);
}

SigTCPExport::~SigTCPExport()
{
    delete ui;
}

void SigTCPExport::OnQuit()
{
    emit sig_close_TCPServer();
}

void SigTCPExport::TCPServerStatusChanged(bool enable)
{
    if(enable)
    {
        ui->m_combobox_server_ip->setEnabled(false);
        ui->m_spinBox_port->setEnabled(false);
        ui->m_pushButton_tcp_export_enable->setText("Open");
        ui->m_pushButton_tcp_export_enable->setStyleSheet("background-color: rgb(85, 255, 0);");
    }
    else
    {
        ui->m_combobox_server_ip->setEnabled(true);
        ui->m_spinBox_port->setEnabled(true);
        ui->m_pushButton_tcp_export_enable->setText("Closed");
        ui->m_pushButton_tcp_export_enable->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
}


void SigTCPExport::on_m_pushButton_tcp_export_enable_clicked()
{
    if(ui->m_pushButton_tcp_export_enable->text() == "Closed")
    {
        QString _ip_address = ui->m_combobox_server_ip->currentText();
        quint16 _port = ui->m_spinBox_port->value();
        emit sig_open_TCPServer(_ip_address, _port);
    }
    else if(ui->m_pushButton_tcp_export_enable->text() == "Open")
    {
        emit sig_close_TCPServer();
    }
}
