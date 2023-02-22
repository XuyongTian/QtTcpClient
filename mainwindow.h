#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ytcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_clearRecv_clicked();

    void on_pushButton_clearSend_clicked();

private:
    Ui::MainWindow *ui;

private:
    YTcpClient* tcpClient;

public slots:
    void tcpClientReadData(QString);
};
#endif // MAINWINDOW_H
