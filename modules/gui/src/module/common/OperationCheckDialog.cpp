#include "OperationCheckDialog.h"
#include "ui_OperationCheckDialog.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMouseEvent>
#include <QString>

#include "as/Common/CommonProcess.h"

OperationCheckDialog::OperationCheckDialog(as::WidgetType widget_type, QString info, int key_amount, QWidget* parent = nullptr,bool isSplit) :
    ASQDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
	m_ui(new Ui::OperationCheckDialog), m_keyValue(KeyValue::None)
{
	m_ui->setupUi(this);
    img = new QImage;    
    // 标题行（图片待添加）
    if (widget_type == as::WidgetType::Error) {
        setWindowTitle(tr(as::CommonProcess::GetInstance().string_To_UTF8("错误").c_str()));
        img->load(tr("res/button/error.png"));
        m_ui->label_icon->setPixmap(QPixmap::fromImage(*img));
    }
    else if (widget_type == as::WidgetType::Warning) {
        setWindowTitle(tr(as::CommonProcess::GetInstance().string_To_UTF8("警告").c_str()));
        img->load(tr("res/button/warning.png"));
        m_ui->label_icon->setPixmap(QPixmap::fromImage(*img));
    }
    else if (widget_type == as::WidgetType::Tip) {
        setWindowTitle(tr(as::CommonProcess::GetInstance().string_To_UTF8("提示").c_str()));
        img->load(tr("res/button/info.png"));
        m_ui->label_icon->setPixmap(QPixmap::fromImage(*img));
    }
    else if (widget_type == as::WidgetType::Question) {
        setWindowTitle(tr(as::CommonProcess::GetInstance().string_To_UTF8("询问").c_str()));
        img->load(tr("res/button/question.png"));
        m_ui->label_icon->setPixmap(QPixmap::fromImage(*img));
    }

    vector<string> _info = as::CommonProcess::GetInstance().SplitString(info.toLocal8Bit().data(),isSplit);
    QString _text;
    for (int i = 0; i < _info.size(); i++)
    {
        _text += tr(_info.at(i).c_str());
    }
    // 标签文字
    m_ui->label->setText(_text);
    
    m_ui->label_icon->adjustSize();
    m_ui->label->adjustSize();

    m_ui->label->setFixedHeight(m_ui->label->height() > m_ui->label_icon->height() ? m_ui->label->height() : m_ui->label_icon->height());
    int min_width = (key_amount + 1) * m_ui->pushButton->width();
    m_ui->label->setFixedWidth(min_width < m_ui->label->width() ? m_ui->label->width() : min_width);

    int margin = 30;

    this->setFixedWidth(m_ui->label->width() + m_ui->label_icon->width() + 10 + margin * 2);
    this->setFixedHeight(m_ui->label->height() + m_ui->pushButton->height() + margin * 2);

    m_ui->label->move(m_ui->label_icon->width() + 20 + margin, 20);
    m_ui->label_icon->move(margin, 20 + (m_ui->label->height() - m_ui->label_icon->height()) / 2);

    // 界面按钮
    if (key_amount == 1)
    {
        m_ui->pushButton->move((this->width() - m_ui->pushButton->width()) / 2, m_ui->label->height() + margin + 10);
        m_ui->pushButton->setText(tr(as::CommonProcess::GetInstance().string_To_UTF8("确认").c_str()));
        m_ui->pushButton_2->hide();
        m_ui->pushButton_3->hide();
        m_ui->pushButton_4->hide();
    }
    else if (key_amount == 2)
    {
        int key_interval = 30;
        int left = (this->width() - m_ui->pushButton->width() * 2 - key_interval) / 2;
        m_ui->pushButton->move(left, m_ui->label->height() + margin + 10);
        m_ui->pushButton_4->move(left + key_interval + m_ui->pushButton->width(), m_ui->label->height() + margin + 10);
        m_ui->pushButton_4->setText(tr(as::CommonProcess::GetInstance().string_To_UTF8("否").c_str()));
        m_ui->pushButton_2->hide();
        m_ui->pushButton_3->hide();
    }
    else if (key_amount == 3)
    {
        int key_interval = 20;
        int left = (this->width() - m_ui->pushButton->width() * 3 - key_interval * 2) / 2;
        m_ui->pushButton->move(left, m_ui->label->height() + margin + 10);
        m_ui->pushButton_3->move(left + key_interval + m_ui->pushButton->width(), m_ui->label->height() + margin + 10);
        m_ui->pushButton_4->move(left + (key_interval + m_ui->pushButton->width()) * 2, m_ui->label->height() + margin + 10);

        m_ui->pushButton_2->hide();
    }
    else
    {
        int key_interval = 20;
        int left = (this->width() - m_ui->pushButton->width() * 4 - key_interval * 3) / 2;
        m_ui->pushButton->move(left, m_ui->label->height() + margin + 10);
        m_ui->pushButton_2->move(left + key_interval + m_ui->pushButton->width(), m_ui->label->height() + margin + 10);
        m_ui->pushButton_3->move(left + (key_interval + m_ui->pushButton->width()) * 2, m_ui->label->height() + margin + 10);
        m_ui->pushButton_4->move(left + (key_interval + m_ui->pushButton->width()) * 3, m_ui->label->height() + margin + 10);
    }

    // 信号函数
    connect(m_ui->pushButton, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, [this](bool checked)
    {
        Q_UNUSED(checked);
        
        m_keyValue = KeyValue::Yes;     
        this->close();
    });

    connect(m_ui->pushButton_2, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, [this](bool checked)
    {
        Q_UNUSED(checked);
        
        m_keyValue = KeyValue::YesToAll;   
        this->close();
    });

    connect(m_ui->pushButton_3, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, [this](bool checked)
    {
        Q_UNUSED(checked);
        
        m_keyValue = KeyValue::No;
        this->close();
     });

    connect(m_ui->pushButton_4, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, [this](bool checked)
	{
		Q_UNUSED(checked);
		m_keyValue = KeyValue::Cancel;
		this->close();
	});
}

OperationCheckDialog::~OperationCheckDialog()
{
    delete img;
    img = nullptr;
    delete m_ui;
}

OperationCheckDialog::KeyValue OperationCheckDialog::GetKeyValue()
{
    return m_keyValue;
}

//JudgeRangePart::JudgeRangePart(as::RangeTab rangedb)
//{
//    int define = 1;
//    double min = -100;
//    double max = 1000;
//    double rangedefault = 0;
//    auto rangedata = as::RangeData::GetInstance().getRangeData();
//    if (rangedata.find(rangedb) != rangedata.end())
//    {
//        define = as::RangeData::GetInstance().getRangeBase(rangedb).getDefine();
//        min = as::RangeData::GetInstance().getRangeBase(rangedb).getMinvalue();
//        max = as::RangeData::GetInstance().getRangeBase(rangedb).getMaxvalue();
//        rangedefault = as::RangeData::GetInstance().getRangeBase(rangedb).getDefaultvalue();
//    }
//
//    if (define == 2)
//    {
//        min = min * 0.001;
//        this->Rangemin = QString::number(min, 10, 3).toLocal8Bit().data();
//
//        max = max * 0.001;
//        this->Rangemax = QString::number(max, 10, 3).toLocal8Bit().data();
//
//        rangedefault = rangedefault * 0.001;
//        this->Rangedefault = QString::number(rangedefault, 10, 3).toLocal8Bit().data();
//
//    }
//
//    else if (define == 1)
//    {
//        this->Rangemin = QString::number(min, 10, 0).toLocal8Bit().data();
//
//        this->Rangemax = QString::number(max, 10, 0).toLocal8Bit().data();
//
//        if (rangedefault == 0)
//        {
//            this->Rangedefault = QString::number(rangedefault, 10, 3).toLocal8Bit().data();
//        }
//        else
//        {
//            this->Rangedefault = QString::number(rangedefault, 10, 0).toLocal8Bit().data();
//        }
//    }
//}

JudgeRangePart::JudgeRangePart(string rangemin, string rangemax)
{
    this->Rangemin = rangemin;
    this->Rangemax = rangemax;
}

JudgeRangePart::JudgeRangePart(string rangemin, string rangemax, string rangedefault)
{
    this->Rangemin = rangemin;
    this->Rangemax = rangemax;
    this->Rangedefault = rangedefault;
}

int JudgeRangePart::JudgeRangevalue(string dialogname, string LineeditName, string input)
{
    bool range = true;
    if (atof(input.c_str()) > stod(Rangemax) || atof(input.c_str()) < stod(Rangemin))
    {
        range = false;
    }
    else if (input == "")
    {
        range = false;
    }

    if (range == false)
    {
        // 不在取值取件范围内
        if (as::UiCallback().GetInstance().GetUiCallback(as::WidgetType::Warning, "[" + dialogname + ":" + LineeditName + "]\n" + (Rangemin)+" =<" + "[value" +
            "]=< " + (Rangemax)+"\n" + "缺省值: " + (Rangedefault), 2))
        {
            // 选择否
            return 1;
        }
        else
        {
            // 选择是
            return 2;
        }
    }
    else
    {
        // 在取值范围区间内
        return 0;
    }
}
