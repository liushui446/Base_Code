#ifndef AS_DB_MGR_HPP
#define AS_DB_MGR_HPP

#include "as/Common/CommonProcess.h"
#include "as/ASContext.hpp"

namespace as 
{
	class AS_EXPORTS DB_MGR
	{
	public:
		DB_MGR() {};
		~DB_MGR() {};
		static DB_MGR& GetInstance();


		bool GetUserData();

		//bool UpdateAuthority(std::map<std::pair<as::LevelCore::OperationMode, WindowsType>, std::string> authority);

		bool UpdateUserPassword();

		bool CreateUserPassword(vector<User> data);

	};
}

#endif