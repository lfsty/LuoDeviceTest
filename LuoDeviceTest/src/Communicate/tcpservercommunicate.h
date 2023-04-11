
#ifndef TCPSERVERCOMMUNICATE_H
#define TCPSERVERCOMMUNICATE_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>


class TCPServerCommunicate : public QObject
{
    Q_OBJECT
public:
    explicit TCPServerCommunicate(QObject* parent = nullptr);
    ~TCPServerCommunicate();
private:
    QTcpServer m_tcpserver;
    QList<QTcpSocket*> m_tcp_client_socket_list;

public:
    void MoveToThread(QThread* thread);
public slots:
    void OpenTCPServer(const QString& ip_address, const quint16& port);
    void CloseTCPServer();
    void ExportData(const QList<float>& framedata);
signals:
    void sig_TCPServerStatusChanged(bool enable);
};

#endif // TCPSERVERCOMMUNICATE_H
