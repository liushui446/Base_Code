#include "as/UserImpl.hpp"
#include <as/sqlite3.hpp>

namespace as
{
	UserImpl::UserImpl()
	{
	}

	void UserImpl::GetItem()
	{
		char* cErrMsg = NULL, ** dbResult = nullptr;
		string sql_1;
		string sql_2;
		string sql_3;
		int nRow = 0, nColumn = 0;

		sql_1 = fmt::format("select * from UserPassword");
		sql_2 = fmt::format("select * from PositionPw");
		try
		{
			int res = sqlite3_get_table(as::UserDBUtils::Getinstance().getsqlite(), sql_1.c_str(), &dbResult, &nRow, &nColumn, &cErrMsg);
			if (res != SQLITE_OK)
			{
				std::cout << "UserPassword select fail" << (cErrMsg ? cErrMsg : "0 count") << endl;
				as::UserDBUtils::Getinstance().free();
			}
			
		}
		catch (...)
		{
			as::UserDBUtils::Getinstance().free();
			throw nullptr;
		}

		int index = 0;
		ASContext* level = &ASContext::GetInstance();
		for (int row = 0; row < nRow; row++)
		{
			index = (row + 1) * nColumn;
			User tmp_user;
			if (string(dbResult[index+1]) == "Teacher")
			{
				tmp_user.setUserType(UserType::Teacher);
				tmp_user.setUsername(string(dbResult[index]));
				tmp_user.setPassword(string(dbResult[index + 2]));
				level->GetUserManagedata()->InsertUserParam(tmp_user.getUserType(), string(dbResult[index]), tmp_user);
			}
			else if (string(dbResult[index+1]) == "Student")
			{
				tmp_user.setUserType(UserType::Student);
				tmp_user.setUsername(string(dbResult[index]));
				tmp_user.setPassword(string(dbResult[index + 2]));
				level->GetUserManagedata()->InsertUserParam(tmp_user.getUserType(), string(dbResult[index]), tmp_user);
			}
		}

		try
		{
			int res = sqlite3_get_table(as::UserDBUtils::Getinstance().getsqlite(), sql_2.c_str(), &dbResult, &nRow, &nColumn, &cErrMsg);
			if (res != SQLITE_OK)
			{
				std::cout << "PositionPw select fail" << (cErrMsg ? cErrMsg : "0 count") << endl;
				as::UserDBUtils::Getinstance().free();
			}

		}
		catch (...)
		{
			as::UserDBUtils::Getinstance().free();
			throw nullptr;
		}
		//string code = string(dbResult[1]);
		if (dbResult == nullptr )
		{
			sqlite3_free_table(dbResult);
			return;
		}
		level->GetInstance().GetUserManagedata()->SetAkCode(string(dbResult[1]));
		
	}

	
	bool UserImpl::UpdateItem()
	{
		ASContext* level = &ASContext::GetInstance();
		auto data = level->GetUserManagedata()->GetuserDataRes();
		string type;
		char* cErrMsg = NULL;
		string sql;
		int res = SQLITE_OK;
		try
		{
			for (auto it = data.find(UserType::Teacher)->second.begin(); it != data.find(UserType::Teacher)->second.end(); it++)
			{
				type = "Teacher";
				sql = fmt::format("update UserPassword set USER_TYPE ='{1}', USER_PASSWORD = '{2}' where USER_Name = '{0}';", \
					it->second.getUsername(), type, it->second.getPassword());
				
				res = sqlite3_exec(UserDBUtils::Getinstance().getsqlite(), sql.c_str(), 0, 0, &cErrMsg);
				if (res != SQLITE_OK)
				{
					std::cout << it->second.getUsername() << "--" << "User Update Fail" << (cErrMsg ? cErrMsg : "0 count") << endl;
					as::UserDBUtils::Getinstance().free();
					return false;
				}
			}
			for (auto it = data.find(UserType::Student)->second.begin(); it != data.find(UserType::Student)->second.end(); it++)
			{
				type = "Student";
				sql = fmt::format("update UserPassword set USER_TYPE ='{1}', USER_PASSWORD = '{2}' where USER_Name = '{0}';", \
					it->second.getUsername(), type, it->second.getPassword());
				
				res = sqlite3_exec(UserDBUtils::Getinstance().getsqlite(), sql.c_str(), 0, 0, &cErrMsg);
				if (res != SQLITE_OK)
				{
					std::cout << it->second.getUsername() << "--" << "User Update Fail" << (cErrMsg ? cErrMsg : "0 count") << endl;
					as::UserDBUtils::Getinstance().free();
					return false;
				}
			}
		}
		catch (...)
		{
			as::UserDBUtils::Getinstance().free();
			return false;
		}
		
		as::UserDBUtils::Getinstance().free();
		return true;
	}

	void UserImpl::DeleteItem()
	{

	}

	void UserImpl::AddItem(vector<User> data)
	{
		char* cErrMsg = NULL;
		string sql;
		string type;
		int res = SQLITE_OK;

		try
		{
			for (auto &val : data)
			{
				if (val.getUserType() == UserType::Teacher)
					type = "Teacher";
				else
					type = "Student";
				
				sql = fmt::format("insert into UserPassword (USER_Name, USER_TYPE, USER_PASSWORD) values('{}','{}','{}');", \
					val.getUsername(), type, val.getPassword());
				int res = sqlite3_exec(UserDBUtils::Getinstance().getsqlite(), sql.c_str(), 0, 0, &cErrMsg);
				if (res != SQLITE_OK) {
					std::cout << val.getUsername() << "--" << "User Create Fail" << (cErrMsg ? cErrMsg : "0 count") << endl;
					UserDBUtils::Getinstance().free();
					return;
				}
			}
		}
		catch (...)
		{
			as::UserDBUtils::Getinstance().free();
			return;
		}
		as::UserDBUtils::Getinstance().free();
	}

}