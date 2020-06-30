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
#include <queue>
#include "SDP.h"
#include "Database.h"
#include "RFCommSocketServer.h"
#include "RFCommSocketClient.h"

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

    void accept_thread_fcn();

    void send_thread_fcn();

    void send_socket_thread_fcn(const string &s);

    void recv_thread_fcn();

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

    int status;
    bool connected = false;
    std::thread *accept_th; // accept thread: accepts a connection
    std::thread *send_th; // send thread: writes data to comm output buffer
    std::thread *recv_th; // recv thread: reads data from comm input buffer
    Socket *sock; // socket ptr to point to correct socket for send/recv
    std::queue<string> recv_buff; // comm input buffer: used to store incoming data
    std::queue<string> send_buff; // comm output buffer: used to store outgoing data
    std::mutex imut; // Mutex for input buffer: protects access to shared buffer
    std::mutex omut; // Mutex for output buffer: protects access to shared buffer
    std::mutex accept_mut;
    std::condition_variable accept_conn;
    std::condition_variable send_data;
    std::condition_variable read_data;

    RFCommSocketServer *serv;

    RFCommSocketClient *cli;

    Database *db;

    SDP *sdp;

    std::vector<sdp_record> sdp_rec;

    QString hostname;

    int serv_port;

protected:

    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
