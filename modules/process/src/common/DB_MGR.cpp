#include "as/Common/DB_MGR.hpp"
#include "as/ASContext.hpp"
//#include "as/RangeCore.hpp"

#include <as/UserDao.hpp>
#include "as/UserImpl.hpp"

#include <as/Common/UiCallback.h>
//#include <as/Rangelmpl.hpp>


namespace as
{
	//shared_ptr<Factory> fac_base = make_shared<Factory_sqlite>();

    as::DB_MGR& DB_MGR::GetInstance()
    {
        static DB_MGR instance;
        return instance;
    }

	//权限数据库权限数据和密码获取
	bool DB_MGR::GetUserData()
	{
		try
		{
			as::UserDBUtils::Getinstance().getConnection();
			shared_ptr<as::UserDAO> Userlite(new as::UserImpl());
			Userlite->GetItem();
			return true;
		}
		catch (...)
		{
			return nullptr;
		}
	}

	//更新用户密码
	bool DB_MGR::UpdateUserPassword()
	{
		try
		{
			as::DataBasePath::GetDataBasePath();
			as::UserDBUtils::Getinstance().getConnection();
			shared_ptr<as::UserDAO> Userlite(new as::UserImpl());
			Userlite->UpdateItem();
			return true;
		}
		catch (...)
		{
			return nullptr;
		}
	}

	bool DB_MGR::CreateUserPassword(vector<User> data)
	{
		try
		{
			as::DataBasePath::GetDataBasePath();
			as::UserDBUtils::Getinstance().getConnection();
			shared_ptr<as::UserDAO> Userlite(new as::UserImpl());
			Userlite->AddItem(data);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	
	/*bool DB_MGR::UpdateAuthority(std::map<std::pair<as::LevelCore::OperationMode, WindowsType>, std::string> authority)
	{
		try
		{
			as::DataBasePath::GetDataBasePath();
			as::UserDBUtils::Getinstance().getConnection();
			shared_ptr<as::UserDAO> Userlite(new as::UserImpl());
			Userlite->UpdateAuthorityItem(authority);
			return true;
		}
		catch (...)
		{
			return nullptr;
		}
	}*/

	

}