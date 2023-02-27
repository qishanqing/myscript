#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mind_os/mind_os.h>

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
    void on_btnConnectServer_clicked();

    void on_btnDisConnectServer_clicked();

    void on_btnSub_clicked();

    void on_btnUnsub_clicked();

    void on_btnSub_2_clicked();

    void on_btnStartCall_clicked();

    void on_btnStopCall_clicked();

    void slot_recv_log(QString txt);

private:
    Ui::MainWindow *ui;
    mind_os::RemoteNodeHandle nh;
    mind_os::Subscriber subData, subData2;
    mind_os::ServiceClient scTest;
    mind_os::util::LoopThread* ltCall;

public:
    void SendLog(QString txt);

signals:
    void signal_send_log(QString txt);
};
#endif // MAINWINDOW_H
