#include "HomePageDialog.h"
#include "ui_HomePageDialog.h"
#include "src/module/common/OperationCheckDialog.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QFuture>
#include <Qtimer>
#include <QStandardItemModel>
#include <QtWidgets>


//#include "signal_transmitter.h"
//#include "as/IniPara.hpp"

HomePageDialog::HomePageDialog(QWidget *parent)
    : ASQDialog(parent),
	m_ui(new Ui::HomePageDialog)
{
	m_ui->setupUi(this);

    InitUI();
    InitConnect();
}

HomePageDialog::~HomePageDialog()
{
	delete m_ui;
}

void HomePageDialog::InitUI()
{


}

void HomePageDialog::InitConnect()
{
    // ==== 位置类型 下拉选择 更新标记位置 ====
    
}

