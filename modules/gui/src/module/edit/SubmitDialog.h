#ifndef AS_SUBMITDIALOG_H
#define AS_SUBMITDIALOG_H

#include "common/CommonPart.h"
#include "as/CommonCore.hpp"
#include "as/common/UiCallback.h"

//
#include "as/common/CommonProcess.h"

//Qt
#include <QLineEdit>

namespace Ui
{
	class SubmitDialog;
}
using namespace as;

class SubmitDialog : public ASQDialog
{
    Q_OBJECT
public:
	SubmitDialog(UserLeaveMessageData mess, QWidget* parent = nullptr);
	~SubmitDialog();

	void InitUI();
	
	void InitConnect();
	
signals:

private slots:

private:
    Ui::SubmitDialog*m_ui;
	string m_mess = "";
};

#endif
