#include "mainwindow.h"
#include "ui_mainwindow.h"

static const string service_name = "BTCHAT";
static const string service_dsc = "Bluetooth Chat";
static const string service_prov = "MIEEICOM";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->installEventFilter(this);

    try{
        sdp = new SDP();
        serv_port = sdp->findAvailablePort();
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
    }

    try{
        serv = new RFCommSocketServer(serv_port);
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
    }

    try{
        sdp->RegisterService(serv_port, service_name, service_dsc, service_prov);
        ui->portLabel->setText("Port: " + QString::number(serv_port));
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
    }

    hostname = QSysInfo::machineHostName();
    ui->deviceLabel->setText("Device: " + hostname);

    QBluetoothLocalDevice loc_dev;
    QBluetoothAddress bt_addr = loc_dev.address();
    ui->bt_addressLabel->setText("Bluetooth address: " + bt_addr.toString());


    /* Create a new client socket */
    cli = new RFCommSocketClient();
    /* Initialize comm socket as client */
    this->sock = this->cli;

    /* Create threads */
    //accept_th = new std::thread(&MainWindow::accept_thread_fcn, this);
    send_th = new std::thread(&MainWindow::send_thread_fcn, this);
    recv_th = new std::thread(&MainWindow::recv_thread_fcn, this);

    /* Run threads in background */
    //accept_th->detach();
    send_th->detach();
    recv_th->detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initiateDatabase(const std::string &bt_addr)
{
    QString path;
    path = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "BTChatHist", QStandardPaths::LocateDirectory);
    if(path.isEmpty()){
        path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BTChatHist";
        QDir dir(path);
        dir.mkpath(".");
    }
    path += "/" + QString::fromStdString(bt_addr) + ".dat";

    try{
        db = new Database(path.toStdString());
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
        return;
    }
    string hist;
    while(1){
        hist = db->history();
        if(hist.empty()) break;
        ui->chatBox->appendPlainText(QString::fromStdString(hist));
    }
}

void MainWindow::accept_thread_fcn()
{
    /*try{ //to do: apagar esta parte de cima
        serv->Accept();
        connected = true;
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
    }

    std::unique_lock<std::mutex> lck(accept_mut);

    while(1){
        lck.lock();
        status = Select()?;

        if(!status){
            connected = true;
            ui->connectButton->setEnabled(false);
            initiateDatabase("00:00:00:00:00:00"); //to do: obter o endereço bluetooth do target
            sock = serv;
            break;
        }

        //lck.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }*/
}

void MainWindow::send_thread_fcn()
{
    string s;
    while(1) // thread runs forever
    {
        std::unique_lock<std::mutex> u_lk(omut); // protect shared resource: output buffer

        send_data.wait(u_lk,
                       [this]{ return !send_buff.empty(); } ); // wait for send condition to be activated, with some data to process
        s = send_buff.front(); /* Retrieve string from output buffer */
        send_buff.pop(); /* Remove element from queue */
        u_lk.unlock(); /* Unlock buffer */

        /* Create a thread to send through socket */
        std::thread t(&MainWindow::send_socket_thread_fcn, this, s);

        /* Wait for thread to end, before processing more data */
        t.join();
    }

}

void MainWindow::send_socket_thread_fcn(const string &s)
{
    try{
        sock->Send(s);
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
    }
}

void MainWindow::recv_thread_fcn()
{

}

void MainWindow::on_connectButton_clicked()
{
    sdp_record temp_rec;

    if(ui->comboBox->count() == 0){
        MainWindow::DisplayWarnMsgbox("Error:","No device was selected!");
        return;
    }

    int temp_index = ui->comboBox->currentIndex();
    temp_rec.addr = sdp_rec[temp_index].addr;
    temp_rec.dev_name = sdp_rec[temp_index].dev_name;
    temp_rec.port = sdp_rec[temp_index].port;

    ChangeStatusLabel("Connecting", "Blue");
    QMessageBox pairmsgBox;
    pairmsgBox.setText("Pairing with " + QString::fromStdString(temp_rec.dev_name) + "!");
    pairmsgBox.exec();

    //std::unique_lock<std::mutex> lck(accept_mut);

    try{
        // Bind to port
        //status = cli->Connect(temp_rec.addr, temp_rec.port);
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Failed connection:", e.what());
        return;
    }

    if(!status){
        connected = true;
        ui->chatBox->clear();
        initiateDatabase(temp_rec.addr);
        ChangeStatusLabel("Connected", "Green");
        ui->connectButton->setEnabled(false);
        return;
    }
    ChangeStatusLabel("Unconnected", "Red");
}

QString MainWindow::FormatText(QString dev_name, QString &text)
{
    QString msg = "<" + dev_name + " - ";
    msg += QTime::currentTime().toString();
    msg += "> ";
    msg += text;
    return msg;
}

void MainWindow::on_sendButton_clicked()
{
    QString sendText = ui->lineEdit->text();
    ui->lineEdit->clear();
    QString sendTestForm = FormatText(hostname, sendText);

    if(connected){
        std::lock_guard<std::mutex> u_lk(omut); // protect shared resource: output buffer
        send_buff.push(sendTestForm.toStdString()); /* Store string in output buffer */
        send_data.notify_one(); /* Notify Send thread that it can send data */
        ui->chatBox->appendPlainText(sendTestForm);
        db->write(sendTestForm.toStdString());
    }
    else DisplayWarnMsgbox("Error:", "You are not connected!");
}

void MainWindow::processMsgReceived()
{
    QString msg;
    try{
        msg = QString::fromStdString(serv->Recv());
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Failed to receive message:", e.what());
        return;
    }

    ui->chatBox->appendPlainText(msg);
    db->write(msg.toStdString());
}

void MainWindow::ChangeStatusLabel(QString labelText, QColor labelColor)
{
    ui->statusLabel->setText(labelText);
    QPalette palette = ui->statusLabel->palette();
    palette.setColor(QPalette::WindowText, labelColor);
    ui->statusLabel->setPalette(palette);
}

void MainWindow::DisplayWarnMsgbox(QString text, QString infoText)
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Warning);
    msgbox.setText(text);
    msgbox.setInformativeText(infoText);
    msgbox.exec();
}

int MainWindow::fill_comboBox()
{
    ui->comboBox->clear();

    try{
        sdp->SearchService(service_name, sdp_rec);
    }
    catch(runtime_error &e){
        MainWindow::DisplayWarnMsgbox("Runtime error:", e.what());
    }

    for (std::vector<sdp_record>::iterator it = sdp_rec.begin(); it != sdp_rec.end(); ++it)
    {
        QString dev_name = QString::fromStdString(it->dev_name);
        ui->comboBox->addItem(dev_name);

        QCoreApplication::processEvents();
    }

    if(ui->comboBox->count() == 0)
    {
        MainWindow::DisplayWarnMsgbox("No devices found!","Please check if your bluetooth is enabled.");
        return 0;
    }
    return 1;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->comboBox)
    {
        if (event->type() == QEvent::MouseButtonPress)
            MainWindow::fill_comboBox();

        return false;
    }
    return false;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(ui->lineEdit->text().isEmpty()){
        ui->sendButton->setEnabled(false);
    }
    else{
        ui->sendButton->setEnabled(true);
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty()) on_sendButton_clicked();
}
