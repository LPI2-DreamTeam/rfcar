#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QMessageBox>
#include <QSysInfo>
#include <QBluetoothLocalDevice>
#include <QStandardPaths>
#include <QDir>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <QDateTime>
#include "SDP.h"
#include "Database.h"
#include "RFCommSocketServer.h"
#include "RFCommSocketClient.h"
#include "Thread.h"

using std::runtime_error;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void acceptThread();

    QString FormatText(QString dev_name, QString &text);

    void initiateDatabase(const std::string &bt_addr);

private slots:
    void processMsgReceived();

    void on_sendButton_clicked();

    void on_connectButton_clicked();

    int fill_comboBox();

    void ChangeStatusLabel(QString labelText, QColor color);

    void DisplayWarnMsgbox(QString text, QString infoText);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;

    std::mutex mtx;
    int status;
    bool connected = false;
    std::thread accept_th_var;

    RFCommSocketServer *serv;

    RFCommSocketClient *cli;

    Database *db;

    SDP *sdp;

    std::vector<sdp_record> sdp_rec;

    QString hostname;
    QString targetname;

    int serv_port;

protected:

    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
