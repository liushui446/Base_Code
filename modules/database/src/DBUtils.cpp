#include"as/DBUtils.hpp"
#include <fstream>
#include <mutex>
#include <deque>
#include <map>
#include <vector>
#include <chrono>
#include <thread>

namespace as
{
	std::string DataBasePath::UserPath = "";
	std::string DataBasePath::RangePath = "";

	//不同数据库路径的获取
	void DataBasePath::GetDataBasePath()
	{
		std::string path;

		if ((path = getcwd(NULL, 0)) == "")
			path = "..//...//";
		else
		{
			int slow_ptr = 0, fast_ptr = path.find("\\");
			while (fast_ptr < path.size())
			{
				slow_ptr = fast_ptr;
				while (fast_ptr < path.size() && path[fast_ptr++] != '\\');

				std::string sub = path.substr(slow_ptr, fast_ptr - slow_ptr - 1);
				if (sub == "build")
					break;
			}
			path = path.substr(0, slow_ptr - 1);
		}
		/*test*/UserPath = string(getcwd(NULL, 0)) + "\\UserdataDB.db";
		//UserPath = path + "\\modules\\database\\src\\db\\UserdataDB.db";
		RangePath = path + "\\modules\\database\\src\\db\\RangeDB.db";
	}

	bool UserDBUtils::getConnection()
	{
		int nRes = sqlite3_open(DataBasePath::UserPath.c_str(), &pDB1);
		if (nRes != SQLITE_OK) {
			std::cout << "open user databases fail";
			return false;
		}
		return true;
	}

	void UserDBUtils::free()
	{
		if (pDB1 != NULL) {
			sqlite3_close(pDB1);
		}
	}

	sqlite3* UserDBUtils::getsqlite()
	{
		return pDB1;
	}

	UserDBUtils& UserDBUtils::Getinstance()
	{
		static UserDBUtils m_cUserDBUtils;
		return m_cUserDBUtils;
	}


	bool RangeDBUtils::getConnection()
	{
		int nRes = sqlite3_open(DataBasePath::RangePath.c_str(), &pDB1);
		if (nRes != SQLITE_OK) {
			std::cout << "open databases fail";
			return false;
		}
		return true;
	}

	void RangeDBUtils::free()
	{
		if (pDB1 != NULL) {
			sqlite3_close(pDB1);
		}
	}

	sqlite3* RangeDBUtils::getsqlite()
	{
		return pDB1;
	}

	RangeDBUtils& RangeDBUtils::Getinstance()
	{
		static RangeDBUtils m_cMechDBUtils;
		return m_cMechDBUtils;
	}
}