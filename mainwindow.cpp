#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpClient = new YTcpClient(8090);
    connect(tcpClient, &YTcpClient::transmitMessage, this, &MainWindow::tcpClientReadData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcpClientReadData(QString str)
{
    qDebug() << "main:" << str;
    ui->textEdit_context->append("\nReceive:\n" + str);
}


void MainWindow::on_pushButton_connect_clicked()
{
    bool bRet = tcpClient->doConnect();
    if (bRet)
    {
        ui->pushButton_connect->setText("Connected");
    }
    else if (ui->pushButton_connect->text() == "Connected")
    {
        bRet = tcpClient->doDisconnect();
        ui->pushButton_connect->setText("Connect");
    }
}


void MainWindow::on_pushButton_send_clicked()
{
    QString str = ui->textEdit_input->toPlainText();
    ui->textEdit_context->append("\nSend:\n" + str);
    tcpClient->sendMessage(str);
}


void MainWindow::on_pushButton_clearRecv_clicked()
{
    ui->textEdit_context->clear();
}


void MainWindow::on_pushButton_clearSend_clicked()
{
    ui->textEdit_input->clear();
}

