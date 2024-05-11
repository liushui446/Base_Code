#ifndef AS_TCPSERVERDIALOG_H
#define AS_TCPSERVERDIALOG_H

#include "common/CommonPart.h"
#include "as/CommonCore.hpp"
#include "as/common/UiCallback.h"

#include "SubmitDialog.h"

//
#include "as/common/CommonProcess.h"

//Qt
#include <QLineEdit>

//Tcp
#include "qtcpserver.h"
#include "qtcpsocket.h"

namespace Ui
{
	class TcpServerDialog;
}
using namespace as;

class TcpServerDialog : public ASQDialog
{
    Q_OBJECT
public:
	TcpServerDialog(QWidget* parent = nullptr);
	~TcpServerDialog();

	void InitUI();
	void InitConnect();
	
	void hasConnect();
	void handleSendOutData(const sendStruct& data);
	void handleSendOutData();
	void handleGetRecieveData();
	void DataToMess(UserLeaveMessageData data, UserLeaveMessage& mess);
	void MessToData(UserLeaveMessage data, UserLeaveMessageData& mess);
signals:

private slots:
	

private:
    Ui::TcpServerDialog* m_ui;

	QTcpServer* m_server;
	QTcpSocket* m_socket;

	//SubmitDialog* m_cSubmitDialog;
	
	bool m_ishead;
	QString m_fileName;
	int m_fileSize;			//接收文件的总大小
	int m_recvSize;			//当前接收文件的大小
	QByteArray m_filebuf;		//当前接收的文件数据

	bool m_isGetPartData;
	int m_requestDataSize;

	// 添加一个成员变量，用于存储客户端套接字指针及其标识信息
	map<string, QTcpSocket*> m_clientSockets;
	vector<QTcpSocket*> m_tcpclientlist;
};

#endif
