#ifndef AS_HOMEPAGEDIALOG_H
#define AS_HOMEPAGEDIALOG_H

#include "common/CommonPart.h"
#include "as/CommonCore.hpp"
#include "as/common/UiCallback.h"

//
#include "as/common/CommonProcess.h"

//Qt
#include <QLineEdit>

namespace Ui
{
	class HomePageDialog;
}
using namespace as;

class HomePageDialog : public ASQDialog
{
    Q_OBJECT
public:
	HomePageDialog(QWidget* parent = nullptr);
	~HomePageDialog();

	void InitUI();
	
	void InitConnect();
	
signals:

private slots:

private:
    Ui::HomePageDialog*m_ui;
	//BenchMarkProcess m_cBenchMarkProcess;

};

#endif
