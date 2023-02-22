#include "ytcpclient.h"

YTcpClient::YTcpClient::YTcpClient()
{
    m_ip = "127.0.0.1";
    m_port = 8080;

    initialize();
}

YTcpClient::YTcpClient::YTcpClient(QString strIP)
{
    m_ip = strIP;
    m_port = 8080;

    initialize();
}

YTcpClient::YTcpClient::YTcpClient(ushort nPort)
{
    m_ip = "127.0.0.1";
    m_port = nPort;

    initialize();
}

YTcpClient::YTcpClient::YTcpClient(QString strIP, ushort nPort)
{
    m_ip = strIP;
    m_port = nPort;

    initialize();
}

void YTcpClient::initialize()
{
    m_status = false;
    m_cnt = 0;

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &YTcpClient::timerTimeout);
    m_timer->start();

    m_client = new QTcpSocket();
    connect(m_client, &QTcpSocket::readyRead, this, &YTcpClient::receiveMessage);
}

bool YTcpClient::doConnect()
{
    // 非连接状态，连接服务器
    if (!m_status)
    {
        // 连接服务器
        m_client->connectToHost(m_ip, m_port);
        // 等待连接成功
        if (!m_client->waitForConnected(2000))
        {
            qDebug() << "Connect to host:" << m_ip << ",port:" << m_port <<" failed";
            return false;
        }
        m_status = true;
        m_cnt = 0;
        if (!m_timer->isActive())
        {
            m_timer->start();
        }
    }
    return true;
}

bool YTcpClient::doDisconnect()
{
    // 已连接状态，断开连接
    if (m_status)
    {
        bool bRet = m_client->disconnect();
        qDebug() << "Disconnect to host, status:" << bRet;
        if (bRet)
        {
//            m_client->abort();
            m_client->close();
            m_status = false;
            m_cnt = 0;
            m_timer->stop();
            return true;
        }
        else
        {
            qDebug() << "Client disconnected to host failed";
            return false;
        }
    }
    // 未连接状态
    else
    {
        qDebug() << "Client is not connected to host";
        return false;
    }
}

void YTcpClient::reConnect()
{
    // 已连接状态，断开重连
    if (m_status)
    {
        doDisconnect();
        doConnect();
    }
    // 未连接状态，直接重连
    else
    {
        doConnect();
    }
}

void YTcpClient::sendMessage(QString str)
{
    if (m_status)
    {
        m_client->write(str.toLocal8Bit());
        qDebug() << "Send message: " << str;
    }
    else
    {
        qDebug() << "ERROR: Send message client not connected to host ";
    }
}

void YTcpClient::receiveMessage()
{
    QByteArray buffer = m_client->readAll();
    QString strRecv = QString(buffer);
    qDebug() << "Receive message: " << strRecv;

    emit transmitMessage(strRecv);
}

void YTcpClient::timerTimeout()
{
    m_status = ((m_client->state() == QAbstractSocket::ConnectedState) ? true : false);
    if (m_status)
    {
        m_cnt = 0;
    }
    else
    {
        m_cnt++;
        doConnect();
        if (m_cnt > 10)
        {
            m_timer->stop();
            qDebug() << "ERROR: Client " << m_client->localPort() << " try to reconnect ten times failed";
        }
    }
}
