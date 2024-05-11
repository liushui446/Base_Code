#ifndef AS_COMDBUTILS_HPP
#define AS_COMDBUTILS_HPP
#include "as/sqlite3.hpp"
#include <iostream>
#include <string>
#include <direct.h>
#include "as/core/def.h"
#include <memory>

using std::shared_ptr;
using std::string;

namespace as
{
	//数据库路径类
	class AS_EXPORTS DataBasePath
	{
	public:
		static void GetDataBasePath();
		static std::string UserPath;
		static std::string RangePath;
	};

	//权限数据库
	class AS_EXPORTS UserDBUtils
	{
	public:
		bool getConnection();
		void free();
		sqlite3* getsqlite();
		static UserDBUtils& Getinstance();
	private:
		sqlite3* pDB1 = NULL;//声明一个数据库
	};

	//Range数据库
	class AS_EXPORTS RangeDBUtils
	{
	public:
		bool getConnection();
		void free();
		sqlite3* getsqlite();
		static RangeDBUtils& Getinstance();
	private:
		sqlite3* pDB1 = NULL;//声明一个数据库
	};
}

#endif