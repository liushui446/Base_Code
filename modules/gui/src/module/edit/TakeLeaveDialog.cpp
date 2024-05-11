#include "TakeLeaveDialog.h"
#include "ui_TakeLeaveDialog.h"
#include "src/module/common/OperationCheckDialog.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QFuture>
#include <Qtimer>
#include <QStandardItemModel>
#include <QtWidgets>
#include "mainwindow.h"

//#include "signal_transmitter.h"
//#include "as/IniPara.hpp"

TakeLeaveDialog::TakeLeaveDialog(QWidget *parent)
    : ASQDialog(parent),
	m_ui(new Ui::TakeLeaveDialog)
{
	m_ui->setupUi(this);
    m_cmain = (MainWindow*)parentWidget();
    InitUI();
    InitConnect();
}

TakeLeaveDialog::~TakeLeaveDialog()
{
	delete m_ui;
}

void TakeLeaveDialog::InitUI()
{


}

void TakeLeaveDialog::InitConnect()
{
    connect(m_ui->toolButton_look, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
        {
            Q_UNUSED(checked);
            UserLeaveMessageData temp_mess;
            temp_mess.m_type = 1;
            temp_mess.m_datetime = "2024/03/25";
            temp_mess.m_reason = "leg broken";
            temp_mess.m_status = leaveStatus::Approvaling;
            m_cSubmitDialog = new SubmitDialog(temp_mess,this);
            if (m_cSubmitDialog->exec() == QDialog::Accepted)
            {
                string des = "this is mess";
                m_cmain->handleSendOutData(temp_mess, des);
                
                OperationCheckDialog tip_dialog(as::WidgetType::Tip, tr(as::CommonProcess::GetInstance().string_To_UTF8("提交成功!").c_str()), 1, this);
                tip_dialog.exec();
            }
        });

    connect(m_ui->toolButton_position, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
        {
            Q_UNUSED(checked);

            m_cmain->SelfPosition();

        });
}

void TakeLeaveDialog::ShowPosition(QString adress)
{
    m_ui->lineEdit_3->clear();
    m_ui->lineEdit_3->setText(adress);
}

