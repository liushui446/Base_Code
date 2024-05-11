#ifndef AS_TAKELEAVEDIALOG_H
#define AS_TAKELEAVEDIALOG_H

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
	class TakeLeaveDialog;
}

class MainWindow;

using namespace as;

class TakeLeaveDialog : public ASQDialog
{
    Q_OBJECT
public:
	TakeLeaveDialog(QWidget* parent = nullptr);
	~TakeLeaveDialog();

	void InitUI();
	
	void InitConnect();

	void ShowPosition(QString adress);
	
signals:

private slots:

private:
    Ui::TakeLeaveDialog *m_ui;

	SubmitDialog* m_cSubmitDialog;
	
	MainWindow* m_cmain;
};

#endif
