#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscrollbar.h"
#include "DataMsg.h"
#include "TestSrv.h"

#include <glog/logging.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::signal_send_log, this, &MainWindow::slot_recv_log);

    ltCall = new mind_os::util::LoopThread(5, [this](){
        static int i;
        tutorial::srv_call::TestSrv srv;
        srv.request.stamp = mind_os::util::now();
        srv.request.param = i++;
        if(!scTest.call(srv))
        {
            SendLog("Failed to call /test.");
        }
        else
        {
            QString txt = QString("call /test: ") + QString(srv.response.success ? "true" : "false") + ", result: " + QString::number(srv.response.result);
            SendLog(txt);
        }
    });
}

MainWindow::~MainWindow()
{
    if (ltCall != nullptr && ltCall->isRunning())
    {
        ltCall->stop(false);
        delete ltCall;
    }
    delete ui;
}

void MainWindow::SendLog(QString txt)
{
    emit signal_send_log(txt);
}

void MainWindow::slot_recv_log(QString txt)
{
    ui->logView->addItem(txt);
    ui->logView->scrollToBottom();
}

void MainWindow::on_btnConnectServer_clicked()
{
    if (!nh.isRunning())
    {
        SendLog("Server connecting...");
        mind_os::RemoteSetting setting;
        setting.ip = ui->lineEdit->text().toStdString();
        setting.port = 20001;
        setting.protocol = mind_os::RemoteSetting::TCP1V1;
        setting.interval = 1000;
        setting.retry = 1;
        nh.startAsClient(setting, [this](bool connected){
            if (connected)
            {
                SendLog("Server connected.");
                scTest = nh.serviceClient<tutorial::srv_call::TestSrv>("/test");
            }
            else
            {
                SendLog("Server disonnected.");
            }
        },
        [this](){
            SendLog("Timeout to connect server.");
        });
    }
}


void MainWindow::on_btnDisConnectServer_clicked()
{
    nh.stop();
}


void MainWindow::on_btnSub_clicked()
{
    subData = nh.subscribe<tutorial::pub_sub::DataMsg>("/data", [this](mind_os::ConstPtr<tutorial::pub_sub::DataMsg>& msg){
        QString txt =
            QString("Received topic data from server: ") + QString::number(msg->data) +
                ", lantency: " + QString::number((mind_os::util::now() > msg->stamp ? mind_os::util::now() - msg->stamp : 0)) + " us";
        SendLog(txt);
    });
}


void MainWindow::on_btnUnsub_clicked()
{
    subData.shutdown();
    subData2.shutdown();
}


void MainWindow::on_btnSub_2_clicked()
{
    subData2 = nh.subscribe<tutorial::pub_sub::DataMsg>("/data2", [this](mind_os::ConstPtr<tutorial::pub_sub::DataMsg>& msg){
            QString txt =
                QString("Received topic data2 from server: ") + QString::number(msg->data) +
                    ", lantency: " + QString::number((mind_os::util::now() > msg->stamp ? mind_os::util::now() - msg->stamp : 0)) + " us";
        SendLog(txt);
    });
}


void MainWindow::on_btnStartCall_clicked()
{
    if (!ltCall->isRunning())
    {
        ltCall->start();
    }
}


void MainWindow::on_btnStopCall_clicked()
{
    if (ltCall->isRunning())
    {
        ltCall->stop();
    }

}

