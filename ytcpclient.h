#ifndef YTCPCLIENT_H
#define YTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QString>
#include <QDebug>

class YTcpClient : public QObject
{
    Q_OBJECT

public:
    YTcpClient();
    YTcpClient(QString);
    YTcpClient(ushort);
    YTcpClient(QString, ushort);

public:
    bool doConnect();
    bool doDisconnect();
    void reConnect();
    void sendMessage(QString);

private:
    void initialize();

signals:
    void transmitMessage(QString);

private slots:
    void receiveMessage();

    void timerTimeout();

private:
    // 需要连接的主机IP
    QString m_ip;
    // 需要连接的主机端口
    ushort m_port;
    // client连接socker实例
    QTcpSocket* m_client;
    // 定时器，监控tcp连接状态
    QTimer* m_timer;
    // tcp连接状态，连接中true；断开连接false
    bool m_status;
    // tcp断联计数
    int m_cnt;

};

#endif // YTCPCLIENT_H
