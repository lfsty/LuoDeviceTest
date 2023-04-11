
#include "tcpservercommunicate.h"

TCPServerCommunicate::TCPServerCommunicate(QObject* parent)
    : QObject{parent}
{
    //TCServer初始化
    // 监听
    connect(&m_tcpserver, &QTcpServer::newConnection, this, [ = ]()
    {
        QTcpSocket* client_socket = m_tcpserver.nextPendingConnection();
        m_tcp_client_socket_list.append(client_socket);
        //Disconnect
        connect(client_socket, &QTcpSocket::disconnected, this, [ = ]()
        {
            m_tcp_client_socket_list.removeOne(client_socket);
            client_socket->deleteLater();
        });
    });
}

TCPServerCommunicate::~TCPServerCommunicate()
{
    CloseTCPServer();
}

void TCPServerCommunicate::MoveToThread(QThread* thread)
{
    this->moveToThread(thread);
    m_tcpserver.moveToThread(thread);
}

void TCPServerCommunicate::OpenTCPServer(const QString& ip_address, const quint16& port)
{
    assert(!m_tcpserver.isListening());

    QHostAddress server_address = ip_address == "0.0.0.0" ? QHostAddress::AnyIPv4 : QHostAddress(ip_address);
    m_tcpserver.listen(server_address, port);

    emit sig_TCPServerStatusChanged(m_tcpserver.isListening());
}

void TCPServerCommunicate::CloseTCPServer()
{
    if(m_tcpserver.isListening())
    {
        for(auto socket : m_tcp_client_socket_list)
        {
            //断开与客户端连接
            if(socket != nullptr)
            {
                socket->disconnectFromHost();
                if(socket->state() != QAbstractSocket::UnconnectedState)
                {
                    socket->abort();
                }
                socket->deleteLater();
            }
        }
        m_tcp_client_socket_list.clear();

        m_tcpserver.close();

        emit sig_TCPServerStatusChanged(m_tcpserver.isListening());
    }
}

void TCPServerCommunicate::ExportData(const QList<float>& framedata)
{
    if(!m_tcp_client_socket_list.isEmpty())
    {
        static QByteArray _framedata;
        _framedata.resize(sizeof(float)*framedata.size());
        memcpy(_framedata.data(), framedata.data(), sizeof(float)*framedata.size());

        for(QTcpSocket* socket : m_tcp_client_socket_list)
        {
            if(socket != nullptr)
            {
                socket->write(_framedata);
            }
        }
    }
}

