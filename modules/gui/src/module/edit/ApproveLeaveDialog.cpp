#include "ApproveLeaveDialog.h"
#include "ui_ApproveLeaveDialog.h"
#include "src/module/common/OperationCheckDialog.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QFuture>
#include <Qtimer>
#include <QStandardItemModel>
#include <QtWidgets>
#include <QCheckBox>
#include "mainwindow.h"

#include "src/module/common/CustomDataStreamOperators.h"

//#include "signal_transmitter.h"
//#include "as/IniPara.hpp"

ApproveLeaveDialog::ApproveLeaveDialog(QWidget *parent)
    : ASQDialog(parent),
	m_ui(new Ui::ApproveLeaveDialog)
{
	m_ui->setupUi(this);
    m_cmain = (MainWindow*)parentWidget();
    InitUI();
    InitConnect();
}

ApproveLeaveDialog::~ApproveLeaveDialog()
{
	delete m_ui;
}

void ApproveLeaveDialog::InitUI()
{
    QStringList paddata_headrs = { /*QString::fromLocal8Bit("序号") ,*/QString::fromLocal8Bit("姓名"), QString::fromLocal8Bit("类型"), \
        QString::fromLocal8Bit("原因"), QString::fromLocal8Bit("时间"), QString::fromLocal8Bit("状态") };

    //设置表格列数
    m_ui->tableWidget->setColumnCount(paddata_headrs.size());
    m_ui->tableWidget->setHorizontalHeaderLabels(paddata_headrs);
    m_ui->tableWidget->verticalHeader()->setVisible(true);
    m_ui->tableWidget->horizontalHeader()->setVisible(true);
    m_ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    m_ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_ui->tableWidget->setColumnWidth(0, 140);
    m_ui->tableWidget->setColumnWidth(1, 60);
    m_ui->tableWidget->setColumnWidth(2, 370);
    m_ui->tableWidget->setColumnWidth(3, 140);
    m_ui->tableWidget->setColumnWidth(4, 90);
    /*m_ui->tableWidget->setColumnWidth(5, 70);
    m_ui->tableWidget->setColumnWidth(6, 70);*/

    RefLeaveDataListview();
}

void ApproveLeaveDialog::InitConnect()
{
    connect(m_ui->tableWidget, static_cast<void (QTableWidget::*)(int, int)>(&QTableWidget::cellClicked), this, [this](int row, int column)
        {
             m_cur_selname = m_ui->tableWidget->item(row, 0)->text().toLocal8Bit();
             m_ui->tableWidget->selectRow(row);
        });

    connect(m_ui->toolButton_make1, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
        {
            Q_UNUSED(checked);
            int row = m_ui->tableWidget->currentRow();
            if (row < 0)
            {
                OperationCheckDialog tip_dialog(as::WidgetType::Tip, tr(as::CommonProcess::GetInstance().string_To_UTF8("未选中行!").c_str()), 1, this);
                tip_dialog.exec();
                return;
            }
            ASContext::GetInstance().GetUserManagedata()->GetLeavemess()[row].m_status = leaveStatus::Comfirm;        

            RefLeaveDataListview();
            
        });

    connect(m_ui->toolButton_make_2, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
        {
            Q_UNUSED(checked);
            int row = m_ui->tableWidget->currentRow();
            if (row < 0)
            {
                OperationCheckDialog tip_dialog(as::WidgetType::Tip, tr(as::CommonProcess::GetInstance().string_To_UTF8("未选中行!").c_str()), 1, this);
                tip_dialog.exec();
                return;
            }
            ASContext::GetInstance().GetUserManagedata()->GetLeavemess()[row].m_status = leaveStatus::NO_Pass;

            RefLeaveDataListview();
        });

    connect(m_ui->toolButton_make_3, static_cast<void (QToolButton::*)(bool)>(&QToolButton::clicked), this, [this](bool checked)
        {
            Q_UNUSED(checked);

            m_cmain->handleSendOutData();
            OperationCheckDialog tip_dialog(as::WidgetType::Tip, tr(as::CommonProcess::GetInstance().string_To_UTF8("提交成功!").c_str()), 1, this);
            tip_dialog.exec();
        });
}

void ApproveLeaveDialog::RefLeaveDataListview()
{
    m_ui->tableWidget->clearContents();
    m_ui->tableWidget->setRowCount(0);
    
    vector<UserLeaveMessage> leave_datalist = ASContext::GetInstance().GetUserManagedata()->GetLeavemess();
    if (leave_datalist.size() == 0)
    {
        return;
    }
    m_ui->tableWidget->setRowCount(leave_datalist.size());
    for (int index = 0; index < leave_datalist.size();index++)
    {
        string leave_type;
        string m_status;
        /*QCheckBox* check_5;
        QCheckBox* check_6;
        check_5 = CreateCheckBox(m_ui->tableWidget, index, 5);
        check_6 = CreateCheckBox(m_ui->tableWidget, index, 6);*/
        if (leave_datalist[index].m_type)
            leave_type = "病假";
        else
            leave_type = "事假";

        if (leave_datalist[index].m_status == leaveStatus::Approvaling)
        {
            /*check_5->setChecked(Qt::Checked);
            check_6->setChecked(Qt::Unchecked);*/
            m_status = "待审批";
        }
        else if (leave_datalist[index].m_status == leaveStatus::Comfirm)
        {
            /*check_5->setEnabled(false);
            check_6->setEnabled(false);*/
            m_status = "已批准";
        }  
        else if (leave_datalist[index].m_status == leaveStatus::NO_Pass)
        {
            /*check_5->setEnabled(false);
            check_6->setEnabled(false);*/
            m_status = "已退回";
        }
        
        m_ui->tableWidget->setItem(index, 0, new QTableWidgetItem(tr(as::CommonProcess::GetInstance().string_To_UTF8(leave_datalist[index].m_username).c_str())));
        m_ui->tableWidget->item(index, 0)->setTextAlignment(Qt::AlignCenter);
        m_ui->tableWidget->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_ui->tableWidget->setItem(index, 1, new QTableWidgetItem(QString::fromLocal8Bit(leave_type.c_str())));
        m_ui->tableWidget->item(index, 1)->setTextAlignment(Qt::AlignCenter);
        m_ui->tableWidget->item(index, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_ui->tableWidget->setItem(index, 2, new QTableWidgetItem(QString::fromLocal8Bit(leave_datalist[index].m_reason.c_str())));
        m_ui->tableWidget->item(index, 2)->setTextAlignment(Qt::AlignCenter);
        m_ui->tableWidget->item(index, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_ui->tableWidget->setItem(index, 3, new QTableWidgetItem(QString::fromLocal8Bit(leave_datalist[index].m_datetime.c_str())));
        m_ui->tableWidget->item(index, 3)->setTextAlignment(Qt::AlignCenter);
        m_ui->tableWidget->item(index, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_ui->tableWidget->setItem(index, 4, new QTableWidgetItem(QString::fromLocal8Bit(m_status.c_str())));
        m_ui->tableWidget->item(index, 4)->setTextAlignment(Qt::AlignCenter);
        m_ui->tableWidget->item(index, 4)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        
    }
}

