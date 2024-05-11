#ifndef AS_APPROVEDIALOG_H
#define AS_APPROVEDIALOG_H

#include "common/CommonPart.h"
#include "as/CommonCore.hpp"
#include "as/common/UiCallback.h"

#include "SubmitDialog.h"

//
#include "as/common/CommonProcess.h"

//Qt
#include <QLineEdit>

namespace Ui
{
	class ApproveLeaveDialog;
}

class MainWindow;

using namespace as;

class ApproveLeaveDialog : public ASQDialog
{
    Q_OBJECT
public:
	ApproveLeaveDialog(QWidget* parent = nullptr);
	~ApproveLeaveDialog();

	void InitUI();
	
	void InitConnect();

	void RefLeaveDataListview();
	
signals:

private slots:

private:
    Ui::ApproveLeaveDialog*m_ui;

	SubmitDialog* m_cSubmitDialog;
	
	MainWindow* m_cmain;

	string m_cur_selname;
};

#endif
