/**
 * @file mainwindow.h
 * @author Pedro Duarte
 * @date 20-06-2019
 *
 * @brief Interface for UI main window
 */

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
    /**
     * @brief: Default constructor.
     * @param parent: QWidget parent (default: none)
     *
     * Exceptions caller must catch:
     * - 1: sdp_connect() fails to connect to local device
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief: Default destructor.
     */
    ~MainWindow();
    /**
     * @brief: Function of the thread used to accept a connection.
     */
    void accept_thread_fcn();
    /**
     * @brief: Function of the thread used to send a message.
     */
    void send_thread_fcn();
    /**
     * @brief: Function of the thread used to receive a message.
     */
    void send_socket_thread_fcn(string s);
    /**
     * @brief: Function of the thread used to receive a message.
     */
    void recv_thread_fcn(); 
    /**
     * @brief: Function of the thread that processes the received message and updates the UI and database
     */
    void recv_thread_updateUI_fcn();
    /**
     * @brief: Function that returns a QString with the formatted text that will be displayed on the chat box.
     * @return QString
     * @param dev_name: the name of the device of where the message originates
     * @param text: text to format
     */
    QString FormatText(QString dev_name, QString &text);
    /**
     * @brief: Initiates the chat box messages database by creating a file with the target bluetooth address, or opening the file if it already exists.
     */
    void initiateDatabase(const string &bt_addr);

private slots:
    /**
     * @brief: Function that returns a QString with the formatted text that will be displayed on the chat box.
     */
    void on_sendButton_clicked();
    /**
     * @brief: Function that returns a QString with the formatted text that will be displayed on the chat box.
     */
    void on_connectButton_clicked();
    /**
     * @brief: Function that fills the combo box with the devices who have the desired service.
     * @return int
     */
    int fill_comboBox();
    /**
     * @brief: Function changes the text and color of the status label.
     * @param the text for the label and the color of the label
     */
    void ChangeStatusLabel(QString labelText, QColor color);
    /**
     * @brief: Function that displays a warning message box with a title and information.
     * @param the text for the warning title, and the text with the warning information.
     */
    void DisplayWarnMsgbox(QString text, QString infoText);
    /**
     * @brief: Function that enables the send button only when something is written on the text line.
     */
    void on_lineEdit_textChanged();
    /**
     * @brief: Function that sends the text written on the text line when return is pressed, only if it isn't empty.
     */
    void on_lineEdit_returnPressed();

private:
    /**
     * @brief: MainWindow pointer that points to the ui
     */
    Ui::MainWindow *ui;
    /**
     * @brief: indicates if an incoming connection has been accepted
     */
    int status;
    /**
     * @brief: indicates if there is already a connection
     */
    bool connected = false;
    /**
     * @brief: accept thread - accepts a connection
     */
    std::thread *accept_th;
    /**
     * @brief: send thread - writes data to comm output buffer
     */
    std::thread *send_th;
    /**
     * @brief: recv thread - reads data from comm input buffer
     */
    std::thread *recv_th;
    /**
     * @brief: socket ptr to point to correct socket for send/recv
     */
    Socket *sock;
    /**
     * @brief: comm input buffer - used to store incoming data
     */
    std::queue<string> recv_buff;
    /**
     * @brief: comm output buffer - used to store outgoing data
     */
    std::queue<string> send_buff;
    /**
     * @brief: Mutex for input buffer - protects access to shared buffer
     */
    std::mutex imut;
    /**
     * @brief: Mutex for output buffer - protects access to shared buffer
     */
    std::mutex omut;
    /**
     * @brief: Mutex for accepting a connection - protects access to shared variable
     */
    std::mutex accept_mut;
    /**
     * @brief: Conditional variable used to notify when a connection is available
     */
    std::condition_variable accept_conn;
    /**
     * @brief: Conditional variable used to notify when data is ready to be sent
     */
    std::condition_variable send_data;
    /**
     * @brief: Conditional variable used to notify when data is ready to be read
     */
    std::condition_variable recv_data;
    /**
     * @brief: RFCommSocketServer ptr to point to the server
     */
    RFCommSocketServer *serv;
    /**
     * @brief: RFCommSocketClient ptr to point to the client
     */
    RFCommSocketClient *cli;
    /**
     * @brief: Database ptr to point to the database
     */
    Database *db;
    /**
     * @brief: SDP ptr to point to sdp server
     */
    SDP *sdp;
    /**
     * @brief: stores the information of the devices present in the combo box
     */
    std::vector<sdp_record> sdp_rec;
    /**
     * @brief: stores the local hostname
     */
    QString hostname;
    /**
     * @brief: stores the target bluetooth address
     */
    string targetaddr;
    /**
     * @brief: stores the port where the server is listening and the service is registered
     */
    int serv_port;

protected:
    /**
     * @brief: Function that filters ui objects and events and verifies if the combobox has been clicked.
     * @return false
     * @param object of the ui and the event that occurred
     */
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
