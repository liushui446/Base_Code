#ifndef AS_USERIMPL_HPP
#define AS_USERIMPL_HPP

#include "as/UserDao.hpp"
#include "as/DBUtils.hpp"
#include "as/ASContext.hpp"
namespace as
{
	class AS_EXPORTS  UserImpl : public UserDAO
	{
	public:
		UserImpl();

		void GetItem();
		bool UpdateItem();
		void DeleteItem();
		void AddItem(vector<User> data);
		/*void GetAuthorityItem();
		void GetDefAuthority();*/
		//bool UpdateAuthorityItem(std::map<std::pair<as::LevelCore::OperationMode, WindowsType>, std::string> authority);
	protected:
	};
}
#endif