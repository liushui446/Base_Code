#include "as/edit/LoadDialogProcess.hpp"

namespace as
{
	LoadDialogProcess::LoadDialogProcess()
	{
		m_pUserManagedata = *ASContext::GetInstance().GetUserManagedata();
		m_curSignname = "";
		m_pwd = "";
		m_type = UserType::Teacher;
	}

	void LoadDialogProcess::SetUsername(string name)
	{
		m_curSignname = name;
	}
	string LoadDialogProcess::GetUsername()
	{
		return m_curSignname;
	}

	void LoadDialogProcess::SetUserpwd(string pwd)
	{
		m_pwd = pwd;
	}

	void LoadDialogProcess::SetUsertype(UserType type)
	{
		m_type = type;
	}

	bool LoadDialogProcess::identifyUsername(string val)
	{
		if (m_pUserManagedata.FindUsername(m_type, val))
		{
			m_curSignname = val;
			return true;
		}
		return false;
	}

	bool LoadDialogProcess::identifyUserpwd(string val)
	{
		MD5 tmp_md5;
		string tmp_pwd = m_pUserManagedata.GetUserParam(m_type, m_curSignname).getPassword();
		if (tmp_pwd == tmp_md5.encode(val))
		{
			return true;
		}
		return false;
	}


}