#include "as/PadCore.hpp"

namespace as
{

	User::User()
	{
		username = "";
		password = "";
		userType = as::UserType::Student;
	}

	User& User::operator=(User& ref)
	{
		this->username = ref.getUsername();
		this->password = ref.getPassword();
		this->userType = ref.getUserType();
		return *this;
	}

	string User::getUsername()
	{
		return username;
	}
	void User::setUsername(string name)
	{
		username = name;
	}

	string User::getPassword()
	{
		return password;
	}
	void User::setPassword(string pwd)
	{
		password = pwd;
	}

	UserType User::getUserType()
	{
		return userType;
	}
	void User::setUserType(UserType type)
	{
		userType = type;
	}

	string User::MD5Convert(string origin_password)
	{
		return MD5().encode(origin_password);
	}

	UserManage::UserManage()
	{
		m_strAkCode = "";
	}

	map<UserType, map<string, User>>& UserManage::GetuserDataRes()
	{
		return userDatabase;
	}

	bool UserManage::FindUserType(UserType type)
	{
		if (userDatabase.find(type) != userDatabase.end())
		{
			return true;
		}
		return false;
	}

	bool UserManage::FindUsername(UserType type, string name)
	{
		if (FindUserType(type))
		{
			if (userDatabase.at(type).find(name) != userDatabase.at(type).end())
			{
				return true;
			}
		}
		return false;
	}

	map<string, User>& UserManage::GetUserTypeParam(UserType type)
	{
		if (FindUserType(type))
		{
			return userDatabase.at(type);
		}
		return map<string, User>();
	}

	User UserManage::GetUserParam(UserType type, string name)
	{
		if (FindUsername(type, name))
		{
			return userDatabase.at(type).at(name);
		}
	}

	void UserManage::InsertUserParam(UserType type, string name, User param)
	{
		if (FindUserType(type))
		{
			if (!FindUsername(type, name))
			{
				userDatabase.at(type).insert(pair<string, User>(name, param));
			}
		}
		else
		{
			map<string, User> tmp_data;
			tmp_data.insert(pair<string, User>(name, param));
			userDatabase.insert(pair<UserType, map<string, User>>(type, tmp_data));
		}
	}

	vector<UserLeaveMessage>& UserManage::GetLeavemess()
	{
		return m_vleavemess;
	}

	void UserManage::InsertLeavemess(vector<UserLeaveMessage> vec)
	{
		for (auto& data : vec)
		{
			m_vleavemess.push_back(data);
		}
	}

	void UserManage::SetAkCode(string ak)
	{
		m_strAkCode = ak;
	}
	string UserManage::GetAkCode()
	{
		return m_strAkCode;
	}

}