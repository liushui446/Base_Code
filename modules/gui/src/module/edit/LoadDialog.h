#ifndef AS_LoadDIALOG_H
#define AS_LoadDIALOG_H

#include "common/CommonPart.h"
#include "as/CommonCore.hpp"
#include "as/common/UiCallback.h"

//process
#include "as/common/CommonProcess.h"
#include "as/edit/LoadDialogProcess.hpp"

//Qt
#include <QLineEdit>
#include "qtcpsocket.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostInfo>

namespace Ui
{
	class LoadDialog;
}
using namespace as;

class LoadDialog : public ASQDialog
{
    Q_OBJECT
public:
	LoadDialog(QWidget* parent = nullptr);
	~LoadDialog();

	void InitUI();
	
	void InitConnect();

	QTcpSocket*& GetSocket();

	string GetUsername();

	UserType GetUserType();

	//void getPublicIp();
signals:

private slots:

private:
    Ui::LoadDialog *m_ui;
	
	LoadDialogProcess m_LoadDialogProcess;

	as::UserType m_eSelectMode;
	QTcpSocket* m_socket;

};

#endif
