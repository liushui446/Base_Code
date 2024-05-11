#include "mainwindow.h"
#include "ui_mainwindow.h"

// Qt
#include <QTimer>
#include <QMenuBar>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QToolBar>
#include <QWheelEvent>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QHeaderView>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QApplication>
#include <QTableWidgetItem>


// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

//其他
//#include "as/CommonProcess.h"
#include "src/utils/include/AutoLoadQss.h"
#include <QtWidgets>

using namespace cv;

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);

	iniUI();
	iniConnect();
}

MainWindow::~MainWindow()
{
	delete m_ui;
}


void MainWindow::iniUI()
{
	m_isGetPartData = false;
	m_requestDataSize = 0;
	auto qssLoader = new as::AutoLoadQss(this);
	qssLoader->bindAppAndQss(this, "res/SpiStyleSheet.qss");

	m_CurDialogType = DialogType::Home_Page;
	m_cHomePageDialog = new HomePageDialog(this);
	//添加
	m_ui->verticalLayout_2->insertWidget(0, m_cHomePageDialog);

	m_ui->toolButton_page_3->setEnabled(false);

	m_socket = NULL;
}

void MainWindow::iniConnect()
{
	//connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::handleSocketConnected);


	connect(m_ui->toolButton_page_1, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
		{
			Q_UNUSED(checked);
			switch (m_CurDialogType)
			{
			case DialogType::Home_Page:
				return;
				break;
			case DialogType::Take_Leave:
				if (m_curauthority == UserType::Student)
				{
					m_cTakeLeaveDialog = nullptr;
					delete m_cTakeLeaveDialog;
				}
				else
				{
					m_cApproveLeaveDialog = nullptr;
					delete m_cApproveLeaveDialog;
				}
				break;
			case DialogType::Authority:
				//m_cSubmitDialog = nullptr;
				//delete m_cSubmitDialog;
				break;
			default:
				break;
			}
			// 从布局中移除窗口
			QLayoutItem* item = m_ui->verticalLayout_2->itemAt(0); // 获取第一个插入的窗口
			if (item) {
				QWidget* widgetToRemove = item->widget();
				if (widgetToRemove) {
					m_ui->verticalLayout_2->removeWidget(widgetToRemove);
					delete widgetToRemove; // 删除窗口对象
				}
			}
			m_CurDialogType = DialogType::Home_Page;
			m_cHomePageDialog = new HomePageDialog(this);
			//添加
			m_ui->verticalLayout_2->insertWidget(0, m_cHomePageDialog);
			
		});

	connect(m_ui->toolButton_page_2, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
		{
			Q_UNUSED(checked);
			
			switch (m_CurDialogType)
			{
			case DialogType::Home_Page:
				m_cHomePageDialog = nullptr;
				delete m_cHomePageDialog;
				break;
			case DialogType::Take_Leave:
				return;
				break;
			case DialogType::Authority:
				//m_cSubmitDialog = nullptr;
				//delete m_cSubmitDialog;
				break;
			default:
				break;
			}
			// 从布局中移除窗口
			QLayoutItem* item = m_ui->verticalLayout_2->itemAt(0); // 获取第一个插入的窗口
			if (item) {
				QWidget* widgetToRemove = item->widget();
				if (widgetToRemove) {
					m_ui->verticalLayout_2->removeWidget(widgetToRemove);
					delete widgetToRemove; // 删除窗口对象
				}
			}
			m_CurDialogType = DialogType::Take_Leave;
			if (m_curauthority == UserType::Student)
			{
				m_cTakeLeaveDialog = new TakeLeaveDialog(this);
				m_ui->verticalLayout_2->insertWidget(0, m_cTakeLeaveDialog);
			}
			else
			{
				m_cApproveLeaveDialog = new ApproveLeaveDialog(this);
				m_ui->verticalLayout_2->insertWidget(0, m_cApproveLeaveDialog);
			}
			
			
		});

	connect(m_ui->toolButton_page_3, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
		{
			Q_UNUSED(checked);
			switch (m_CurDialogType)
			{
			case DialogType::Home_Page:
				m_cHomePageDialog = nullptr;
				delete m_cHomePageDialog;
				break;
			case DialogType::Take_Leave:
				if (m_curauthority == UserType::Student)
				{
					m_cTakeLeaveDialog = nullptr;
					delete m_cTakeLeaveDialog;
				}
				else
				{
					m_cApproveLeaveDialog = nullptr;
					delete m_cApproveLeaveDialog;
				}
				break;
			case DialogType::Authority:
				return;
				break;
			default:
				break;
			}
			// 从布局中移除窗口
			QLayoutItem* item = m_ui->verticalLayout_2->itemAt(0); // 获取第一个插入的窗口
			if (item) {
				QWidget* widgetToRemove = item->widget();
				if (widgetToRemove) {
					m_ui->verticalLayout_2->removeWidget(widgetToRemove);
					delete widgetToRemove; // 删除窗口对象
				}
			}
			m_CurDialogType = DialogType::Authority;

		});


	connect(m_ui->comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int index)
		{
			if (index == 1)
			{
				m_socket->disconnectFromHost();
				this->close();
			}
		});
	
}

void MainWindow::SetSocket(QTcpSocket* socket)
{
	m_socket = socket;
	if (m_socket)
	{
		connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::handleGetRecieveData);
	}
	UserLeaveMessageData tem_data;
	string des = "sign in success";
	tem_data.m_username = m_curusername;
	handleSendOutData(tem_data, des);
	//QByteArray data = QString::fromLocal8Bit("sign in success").toUtf8();
	//m_socket->write(data);
}

void MainWindow::HandinMess(string mess)
{
	QByteArray data = QString::fromLocal8Bit(mess.c_str()).toUtf8();
	m_socket->write(data);
}

void MainWindow::SetUserNa(string mess)
{
	m_curusername = mess;
}

void MainWindow::SetAuthority(string mess, UserType auth)
{
	m_curusername = mess;
	m_curauthority = auth;
}

void MainWindow::ShowUserNa()
{
	string temp = "你好,";
	m_ui->label_user->setText(QString::fromLocal8Bit((temp + m_curusername).c_str()));

	if (m_curauthority == UserType::Student)
	{
		m_ui->toolButton_page_2->setText(QString::fromLocal8Bit("请假"));
	}
	else
	{
		m_ui->toolButton_page_2->setText(QString::fromLocal8Bit("审批"));
	}
}

void MainWindow::handleSendOutData(UserLeaveMessageData& data, string des)
{
	QByteArray byteData;
	QDataStream stream(&byteData, QIODevice::WriteOnly);
	data.m_username = m_curusername;
	stream << data;
	sendStruct senddata;
	if (m_curauthority == UserType::Student)
		senddata.Type = 0;
	else
		senddata.Type = 1;
	senddata.Description = QString::fromLocal8Bit(des.c_str());
	senddata.ByteData = byteData;

	if ((!m_socket) || m_socket->state() != QAbstractSocket::ConnectedState)
		return;

	QDataStream out(m_socket);
	out << senddata.size() << senddata;//先发送数据大小，在发送数据本身
	m_socket->flush();
}

void MainWindow::handleSendOutData()
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
	if (m_curauthority == UserType::Student)
		senddata.Type = 0;
	else
		senddata.Type = 1;
	senddata.Description = QString("this is approval");
	senddata.ByteData = byteData;

	if ((!m_socket) || m_socket->state() != QAbstractSocket::ConnectedState)
		return;

	QDataStream out(m_socket);
	out << senddata.size() << senddata;//先发送数据大小，在发送数据本身
	m_socket->flush();
}

void MainWindow::handleGetRecieveData()
{
	if ((!m_socket) || m_socket->state() != QAbstractSocket::ConnectedState)
		return;
	if (m_isGetPartData == false) {
		if (m_socket->bytesAvailable() < sizeof(int))//先接收数据的大小
			return;
		else
		{
			QDataStream in(m_socket);
			in >> m_requestDataSize;//数据大小写入变量
			m_isGetPartData = true;//设置标志，只接收到了数据大小，没接收到数据全部
		}
	}
	if (m_isGetPartData == true) {
		if (m_socket->bytesAvailable() < m_requestDataSize)//判断是否接收到了完整的数据
			return;
		else
		{
			QDataStream in(m_socket);
			sendStruct receiveData;
			in >> receiveData;//接收到了数据
			m_requestDataSize = 0;//清空大小
			m_isGetPartData = false;//清空标志
			
			//数据接收成功，放置在receiveData中，可以做其他处理
			//doSomething(receiveData);
			if (receiveData.Type == 0)
			{

			}
			else
			{
				QDataStream mess(receiveData.ByteData);
				UserLeaveMessageData temp_usermess;
				mess >> temp_usermess;
				vector<UserLeaveMessage> datalist;
				UserLeaveMessage tmp_mess;
				DataToMess(temp_usermess, tmp_mess);
				datalist.push_back(tmp_mess);
				//保存当前数据
				ASContext::GetInstance().GetUserManagedata()->InsertLeavemess(datalist);

				//操作
				if (m_CurDialogType == DialogType::Take_Leave && m_curauthority == UserType::Teacher)
				{
					m_cApproveLeaveDialog->RefLeaveDataListview();
				}
				else if(m_cHomePageDialog != nullptr){

				}
			}
			
			if (m_socket->bytesAvailable())//如果缓存区还存在数据，递归执行
				handleGetRecieveData();
		}
	}
}

//数据转换
void MainWindow::DataToMess(UserLeaveMessageData data, UserLeaveMessage& mess)
{
	mess.m_username = data.m_username;
	mess.m_type = data.m_type;
	mess.m_datetime = data.m_datetime;
	mess.m_reason = data.m_reason;
	mess.m_status = data.m_status;
	mess.m_adress = data.m_adress;
}

//数据转换
void MainWindow::MessToData(UserLeaveMessage data, UserLeaveMessageData& mess)
{
	mess.m_username = data.m_username;
	mess.m_type = data.m_type;
	mess.m_datetime = data.m_datetime;
	mess.m_reason = data.m_reason;
	mess.m_status = data.m_status;
	mess.m_adress = data.m_adress;
}

void MainWindow::queryLocationOfIP(const QString& strIp, const QString& strAk)
{
	//string ip = strIp.toStdString();
	//QString ssst = QString("112.14.29.174");
   // const QString& strUrl = QString("http://api.map.baidu.com/location/ip?ak=%1&ip=PeR9Utg7G9MNLltViPXhw0xhUtabELu8&coor=bd09ll").
		//arg(strIp);
	const QString& strUrl = QString("http://api.map.baidu.com/location/ip?ak=%2&coor=bd09ll&ip=%1").arg(strIp).arg(strAk);

	QNetworkRequest request;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setUrl(QUrl(strUrl));

	m_reply_1 = m_manager->get(request);
	connect(m_reply_1, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void MainWindow::querySpecialLocation(const QString& strAk)
{
	// 创建请求
	const QString& strUrl = QString("http://api.map.baidu.com/reverse_geocoding/v3/?ak=%1&output=json&coordtype=bd09ll&location=").arg(strAk);
	QUrl url(strUrl + m_lat + "," + m_lon);
	QNetworkRequest request(url);

	// 发送请求
	m_reply_1 = m_manager->get(request);

	// 接收响应
	QObject::connect(m_reply_1, &QNetworkReply::finished, [&]() {
		if (m_reply_1->error() == QNetworkReply::NoError) {
			// 读取响应数据
			QByteArray response = m_reply_1->readAll();
			QJsonParseError error;
			// 解析JSON
			QJsonDocument doc = QJsonDocument::fromJson(response, &error);
			if (error.error != QJsonParseError::NoError) {
				QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("JSON:数据结构有问题！"));
				return;
			}
			QJsonObject jsonObject = doc.object();
			int status = jsonObject.value("status").toInt();
			if (status != 0) {
				QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("状态码：%1").arg(status));
				return;
			}
			// 提取位置信息
			QJsonObject result = jsonObject["result"].toObject();
			QJsonObject addresscomponent = result["addressComponent"].toObject();
			QString formattedAddress = result["formatted_address"].toString();
			qDebug() << "country: " << jsonObject["result"].toObject()["addressComponent"].toObject()["country"].toString();
			qDebug() << "province: " << jsonObject["result"].toObject()["addressComponent"].toObject()["province"].toString();
			qDebug() << "city: " << jsonObject["result"].toObject()["addressComponent"].toObject()["city"].toString();
			qDebug() << "district: " << jsonObject["result"].toObject()["addressComponent"].toObject()["district"].toString();
			qDebug() << "town: " << jsonObject["result"].toObject()["addressComponent"].toObject()["town"].toString();
			qDebug() << "street: " << jsonObject["result"].toObject()["addressComponent"].toObject()["street"].toString();
			qDebug() << "Formatted Address: " << formattedAddress;
			m_qrOutAdress = addresscomponent["country"].toString()+",";
			m_qrOutAdress = m_qrOutAdress + addresscomponent["province"].toString() + addresscomponent["city"].toString() + ",";
			m_qrOutAdress = m_qrOutAdress + addresscomponent["district"].toString() +","+ addresscomponent["town"].toString() + ",";
			m_qrOutAdress = m_qrOutAdress + addresscomponent["street"].toString();
			QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("Positon Success!"));
			if (m_CurDialogType == DialogType::Take_Leave)
			{
				m_cTakeLeaveDialog->ShowPosition(m_qrOutAdress);
			}

			m_reply_1->deleteLater();
			m_manager->deleteLater();
		}
		else {
			//qDebug() << "Error: " << m_reply_1->errorString();
			QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("Error：%1").arg(m_reply_1->errorString()));
			// 释放资源
			m_reply_1->deleteLater();
		}
		});
}

void MainWindow::replyFinished()
{
	m_reply_1 = qobject_cast<QNetworkReply*>(sender());

	if (m_reply_1->error() != QNetworkReply::NoError) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请求出错:%1").arg(m_reply_1->errorString()));
	}
	const QByteArray& bytes = m_reply_1->readAll();
	QVariantMap varMap = parseLocationData(bytes);
	if (varMap.isEmpty())
	{
		return;
	}
	showLocation(varMap);
}

QVariantMap MainWindow::parseLocationData(const QByteArray& data)
{
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(data, &error);
	if (error.error != QJsonParseError::NoError) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("JSON:数据结构有问题！"));
		return QVariantMap();
	}
	QJsonObject obj = doc.object();
	int status = obj.value("status").toInt();
	if (status != 0) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("状态码：%1").arg(status));
		return QVariantMap();
	}
	QJsonObject contentObj = obj.value("content").toObject();
	/*QJsonObject contentArray = contentObj.value("address_detail").toObject();
	qDebug() << contentObj.value("address").toString();
	qDebug() << contentArray.value("province").toString();*/
	return contentObj.toVariantMap();
}


void MainWindow::showLocation(QVariantMap varMap)
{
	const QString& addrStr = varMap.value("address").toString();
	//string ad = addrStr.toStdString();
	QVariantMap ptMap = varMap.value("point").toMap();
	QString longitudeStr = ptMap.value("x").toString(); //经度
	QString latitudeStr = ptMap.value("y").toString(); //纬度
	const QString& txt = QStringLiteral("城市：%1 \n经度：%2   纬度：%3").arg(addrStr).arg(longitudeStr).arg(latitudeStr);
	m_lat = latitudeStr;
	m_lon = longitudeStr;
	qDebug() << txt;
	ptMap.clear();
	varMap.clear();

	querySpecialLocation(QString::fromLocal8Bit(ASContext::GetInstance().GetUserManagedata()->GetAkCode().c_str()));
	//m_manager->deleteLater();
}

void MainWindow::getOnlineIPs() {
	for (QNetworkInterface inter : QNetworkInterface::allInterfaces()) {
		// 跳过本地回环接口
		if (inter.flags().testFlag(QNetworkInterface::IsLoopBack)) {
			continue;
		}
		// 跳过不活跃的接口
		if (!inter.flags().testFlag(QNetworkInterface::IsUp)) {
			continue;
		}
		// 遍历每个接口的地址
		for (QNetworkAddressEntry entry : inter.addressEntries()) {
			// 只获取IPv4地址
			if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
				qDebug() << "Online IP:" << entry.ip().toString();
			}
		}
	}
}

void MainWindow::GetOutNetIp()
{
	//QNetworkAccessManager m_manager;
	QUrl url("http://ifconfig.me/ip");
	QNetworkRequest request(url);

	m_reply_1 = m_manager->get(request);
	QObject::connect(m_reply_1, &QNetworkReply::finished, [&]() {
		if (m_reply_1->error() == QNetworkReply::NoError) {
			m_ipnet = m_reply_1->readAll();
			std::cout << "External IP: " << m_ipnet.toStdString() << std::endl;
		}
		else {
			QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("Error：%1").arg(m_reply_1->errorString()));
			return;
		}

		queryLocationOfIP(m_ipnet,QString::fromLocal8Bit(ASContext::GetInstance().GetUserManagedata()->GetAkCode().c_str()));
		});
}

void MainWindow::SelfPosition()
{
	//客户端定位
	m_manager = new QNetworkAccessManager(this);
	GetOutNetIp();
}

