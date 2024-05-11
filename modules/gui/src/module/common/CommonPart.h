#ifndef _COMMONPART_H
#define _COMMONPART_H
#include <QDialog>
#include <QCloseEvent>
#include <QCheckBox>
#include <QTableWidget>
#include <QHBoxLayout>
#include <as/CommonCore.hpp>



class ASQDialog :public QDialog
{
public:

	ASQDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) :QDialog(parent, f)
	{

	}

	void show();

	int exec();

	virtual void closeEvent(QCloseEvent* event);

	QCheckBox* CreateCheckBox(QTableWidget* table, int row, int column);

};

class sendStruct
{
public:
    explicit sendStruct(int Type, QString Description, QByteArray ByteData = QByteArray(0));
    int Type;
    QString Description;//发送内容的描述
    QByteArray ByteData;//具体发送或者接受的内容，可以将所有基本类型int，char，vector，map等或者自定义的结构体通过
                        //QDataStream序列化到ByteData中，接收端同样的方法从QDataStream中解析出来原数据
    sendStruct() { Type = 0; Description = ""; ByteData = QByteArray(0); }
    int size()
    {
        int size = 0;
        size = sizeof(int) + Description.size() * 2 + 4 + ByteData.size() + 4;
        //序列化后QString大小为原有大小乘以2加4，QByteArry序列化后大小为原始大小加4，QString为Unicode编码每个字符占两个字节,
        //QString和QByteArry序列化过程中，首先序列化了本身大小的整形数据(qint32)到序列中，然后才是具体数据。
        return size;
    }
    int size() const
    {
        int size = 0;
        size = sizeof(int) + Description.size() * 2 + 4 + ByteData.size() + 4;
        return size;
    }
    sendStruct& operator=(const sendStruct& other)
    {
        Type = other.Type;
        Description = other.Description;
        ByteData = QByteArray(other.ByteData);
        return *this;
    }
#ifndef QT_NO_DATASTREAM
    friend QDataStream& operator <<(QDataStream& out, const sendStruct& senstruct)
    {
        out << senstruct.Type
            << senstruct.Description
            << senstruct.ByteData;
        return out;
    }
    friend QDataStream& operator >>(QDataStream& in, sendStruct& senstruct)
    {
        in >> senstruct.Type
            >> senstruct.Description
            >> senstruct.ByteData;
        return in;
    }
#endif
};

class UserLeaveMessageData
{
public:
	explicit UserLeaveMessageData(string m_username, int m_type, string m_datetime, string m_reason, as::leaveStatus m_status, string m_adress);

	std::string m_username;
	int m_type;  //(0:事假 1:病假)
	std::string m_datetime;
	std::string m_reason;
	as::leaveStatus m_status;
	std::string m_adress;

	UserLeaveMessageData()
		: m_status(as::leaveStatus::Approvaling)
	{
		m_username = "";
		m_type = 0;
		m_datetime = "";
		m_reason = "";
		m_adress = "";
	}

	UserLeaveMessageData& operator=(UserLeaveMessageData& ref)
	{
		this->m_username = ref.m_username;
		this->m_type = ref.m_type;
		this->m_datetime = ref.m_datetime;
		this->m_reason = ref.m_reason;
		this->m_status = ref.m_status;
		this->m_adress = ref.m_adress;
		return *this;
	}

#ifndef QT_NO_DATASTREAM

	friend QDataStream& operator<<(QDataStream& out, const UserLeaveMessageData& s)
	{
		out << QString::fromLocal8Bit(s.m_username.c_str())
			<< s.m_type
			<< QString::fromLocal8Bit(s.m_datetime.c_str())
			<< QString::fromLocal8Bit(s.m_reason.c_str())
			<< (int)s.m_status
			<< QString::fromLocal8Bit(s.m_adress.c_str());
		return out;
	}

	friend QDataStream& operator>>(QDataStream& in, UserLeaveMessageData& s) {

		QString data_1;
		QString data_2;
		QString data_3;
		int data_4;
		QString data_5;

		in >> data_1
			>> s.m_type
			>> data_2
			>> data_3
			>> data_4
			>> data_5;
		s.m_username = data_1.toStdString();
		s.m_datetime = data_2.toStdString();
		s.m_reason = data_3.toStdString();
		s.m_status = static_cast<as::leaveStatus>(data_4);
		s.m_adress = data_5.toStdString();
		return in;
	}

#endif
};


#endif // !_COMMONPART_H

