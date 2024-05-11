#include "as/ASContext.hpp"

namespace as
{
	ASContext::ASContext()
	{
		m_ptrUserManagedata = make_shared<UserManage>();
	}

	ASContext& ASContext::GetInstance()
	{
		static ASContext as_context;
		return as_context;
	}


	shared_ptr<UserManage> ASContext::GetUserManagedata()
	{
		return m_ptrUserManagedata;
	}
}