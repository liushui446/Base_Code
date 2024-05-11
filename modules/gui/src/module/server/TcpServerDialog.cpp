#include "TcpServerDialog.h"
#include "ui_TcpServerDialog.h"
#include "src/module/common/OperationCheckDialog.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QFuture>
#include <Qtimer>
#include <QStandardItemModel>
#include <QtWidgets>

#include "src/module/common/CustomDataStreamOperators.h"


TcpServerDialog::TcpServerDialog(QWidget *parent)
    : ASQDialog(parent),
	m_ui(new Ui::TcpServerDialog)
{
	m_ui->setupUi(this);

    InitUI();
    InitConnect();
}

TcpServerDialog::~TcpServerDialog()
{
    m_server->close();
    m_server->deleteLater();
	delete m_ui;
}

void TcpServerDialog::InitUI()
{
    m_socket = nullptr;
    m_isGetPartData = false;
    m_requestDataSize = 0;
    m_server = new QTcpServer(this);
    //监听1122端口的ip
    m_server->listen(QHostAddress::Any, 1122);

}

void TcpServerDialog::InitConnect()
{
    //如果有用户连接触发槽函数
    connect(m_server, &QTcpServer::newConnection, this, &TcpServerDialog::hasConnect);
    
    connect(m_ui->toolButton_close, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
        {
            Q_UNUSED(checked);
            this->close();
        });
}

void TcpServerDialog::hasConnect()
{
    while (m_server->hasPendingConnections() > 0)//判断当前连接了多少人
    {
        //用socket和我们的客户端连接，一个客户端对应一个套接字socket
        m_socket = m_server->nextPendingConnection();
        m_tcpclientlist.push_back(m_socket);
        //服务器界面上输出客户端信息
        m_ui->textEdit_show->append(QString::fromLocal8Bit("[%1:%2]:新用户连接").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort()));
        
        //如果客户端发送信息过来了，触发匿名函数
        if (m_socket)
        {
            connect(m_socket, &QTcpSocket::readyRead, [=]() {
                //QByteArray buf = m_socket->readAll();
                handleGetRecieveData();
                });
        }
    }
}

void TcpServerDialog::handleSendOutData(const sendStruct& data)
{
    if ((!m_socket) || m_socket->state() != QAbstractSocket::ConnectedState)
        return;
    if (data.Type == 1)
    {
        if (m_clientSockets.find("teacher_1") != m_clientSockets.end())
            m_socket = m_clientSockets.at("teacher_1");
        else
            return;
    }
    else
    {
        if (m_clientSockets.find("student_1") != m_clientSockets.end())
            m_socket = m_clientSockets.at("student_1");
        else
            return;
    }
    QDataStream out(m_socket);
    out << data.size() << data;//先发送了数据大小，在发送数据
    m_socket->flush();
}

void TcpServerDialog::handleSendOutData()
{
    QByteArray byteData;
    QDataStream stream(&byteData, QIODevice::WriteOnly);
    vector<UserLeaveMessage> mess_list = ASContext::GetInstance().GetUserManagedata()->GetLeavemess();
    if (mess_list.size() == 0)
    {
        return;
    }
    //数据转换
    vector<UserLeaveMessageData> tmp_data;
    for (auto& data1 : mess_list)
    {
        UserLeaveMessageData data_;
        MessToData(data1, data_);
        tmp_data.push_back(data_);
    }
    for (auto& data1 : tmp_data)
    {
        stream << data1;
    }
    sendStruct senddata;
    senddata.Type = 1;
    senddata.Description = QString("this is leavemess");
    senddata.ByteData = byteData;

    if ((!m_socket) || m_socket->state() != QAbstractSocket::ConnectedState)
        return;

    QDataStream out(m_socket);
    out << senddata.size() << senddata;//先发送数据大小，在发送数据本身
    m_socket->flush();
}

void TcpServerDialog::handleGetRecieveData()
{
    //利用for循环循环列表中的每一个连接进来的客户端，判断是哪一个客户端发的数据
    for (int i = 0; i < m_tcpclientlist.size(); i++)
    {
        m_socket = m_tcpclientlist.at(i);
        //QByteArray buffer = m_socket->readAll();
        if (m_socket->bytesAvailable() != 0) //如果检测到接收到的数据长度不为0，则代表是这个客户端发送的数据
            break;
    }
    
    if ((!m_socket) || m_socket->state() != QAbstractSocket::ConnectedState)
        return;
    if (m_isGetPartData == false) {
        if (m_socket->bytesAvailable() < sizeof(int))//先要得到数据的大小
            return;
        else
        {
            QDataStream in(m_socket);
            in >> m_requestDataSize;//数据大小写入这个变量中
            m_isGetPartData = true;//只获得了数据的大小，数据内容还未获得
        }
    }
    if (m_isGetPartData == true) {
        if (m_socket->bytesAvailable() < m_requestDataSize)//判断是否数据接收完整了，不完整就返回等待下一次判断
            return;
        else
        {
            QDataStream in(m_socket);
            sendStruct receiveData;
            in >> receiveData;//接收到了发送端的数据
            m_requestDataSize = 0;//清空大小
            m_isGetPartData = false;//清空标志
            
            //登录
            if (receiveData.Description == QString::fromLocal8Bit("sign in success"))
            {
                QDataStream mess(receiveData.ByteData);
                UserLeaveMessageData temp_usermess;
                mess >> temp_usermess;
                m_clientSockets.insert(pair<string, QTcpSocket*>(temp_usermess.m_username, m_socket));

                m_ui->textEdit_show->append(QString::fromLocal8Bit(temp_usermess.m_username.c_str())+ ":" + receiveData.Description);

                //发送初始数据
                if (temp_usermess.m_username == "teacher_1")
                {
                    handleSendOutData();
                }
            }
            else 
            {
                //发送审批
                if (receiveData.Type == 0)
                {
                    receiveData.Type = 1;
                    handleSendOutData(receiveData);
                    m_ui->textEdit_show->append(receiveData.Description);
                    QDataStream mess(receiveData.ByteData);
                    UserLeaveMessageData temp_usermess;
                    mess >> temp_usermess;
                    vector<UserLeaveMessage> datalist;
                    UserLeaveMessage tmp_mess;
                    DataToMess(temp_usermess, tmp_mess);
                    datalist.push_back(tmp_mess);
                    //保存当前数据
                    ASContext::GetInstance().GetUserManagedata()->InsertLeavemess(datalist);
                }
                else
                {
                    //接收提交的审批
                    m_ui->textEdit_show->append(receiveData.Description);
                    QDataStream mess(receiveData.ByteData);
                    UserLeaveMessageData temp_usermess;
                    mess >> temp_usermess;
                    vector<UserLeaveMessage> datalist;
                    UserLeaveMessage tmp_mess;
                    DataToMess(temp_usermess, tmp_mess);
                    datalist.push_back(tmp_mess);
                    //保存当前数据
                    ASContext::GetInstance().GetUserManagedata()->InsertLeavemess(datalist);
                }
            }
            
            if (m_socket->bytesAvailable())//如果缓存区还存在数据，继续执行
                handleGetRecieveData();
        }
    }
}

//数据转换
void TcpServerDialog::DataToMess(UserLeaveMessageData data, UserLeaveMessage& mess)
{
    mess.m_username = data.m_username;
    mess.m_type = data.m_type;
    mess.m_datetime = data.m_datetime;
    mess.m_reason = data.m_reason;
    mess.m_status = data.m_status;
    mess.m_adress = data.m_adress;
}

//数据转换
void TcpServerDialog::MessToData(UserLeaveMessage data, UserLeaveMessageData& mess)
{
    mess.m_username = data.m_username;
    mess.m_type = data.m_type;
    mess.m_datetime = data.m_datetime;
    mess.m_reason = data.m_reason;
    mess.m_status = data.m_status;
    mess.m_adress = data.m_adress;
}
