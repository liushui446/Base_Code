#ifndef AS_LOADDIALOGPROCESS_H
#define AS_LOADDIALOGPROCESS_H

#include "as/CommonCore.hpp"
#include "as/ASContext.hpp"
#include "as/Common/UiCallback.h"

namespace as
{

	class AS_EXPORTS LoadDialogProcess
	{
	public:
		LoadDialogProcess::LoadDialogProcess();

		void SetUsername(string name);
		string GetUsername();
		void SetUserpwd(string pwd);
		void SetUsertype(UserType type);

		bool identifyUsername(string val);
		bool identifyUserpwd(string val);

		
	private:
		UserManage m_pUserManagedata;
		string m_curSignname;
		string m_pwd;
		UserType m_type;
	};
}

#endif