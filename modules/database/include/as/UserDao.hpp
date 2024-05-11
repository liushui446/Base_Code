#ifndef AS_USERDAO_HPP
#define AS_USERDAO_HPP

#include "as/ASContext.hpp"
#include "score.hpp"
namespace as
{
	class AS_EXPORTS UserDAO
	{
	public:
		virtual ~UserDAO() = default;
		virtual void GetItem() = 0;
		virtual bool UpdateItem() = 0;
		virtual void DeleteItem() = 0;
		virtual void AddItem(vector<User> data) = 0;
		/*virtual void GetAuthorityItem() = 0;
		virtual void GetDefAuthority() = 0;*/
		//virtual bool UpdateAuthorityItem(std::map<std::pair<as::LevelCore::OperationMode, WindowsType>, std::string> authority) = 0;
	};
}
#endif