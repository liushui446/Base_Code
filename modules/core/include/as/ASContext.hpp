#ifndef AS_CONTEXT_HPP
#define AS_CONTEXT_HPP

#include "as/core/def.h"
#include "as/PadCore.hpp"
namespace as
{
	class ASContext;
	class FileSerialize;
}

namespace boost
{
	/*namespace serialization
	{
		template <class Archive>
		void serialize(Archive& ar, aoi::AOIContext& val, const unsigned int version);
	}*/
}

namespace as
{
	class AS_EXPORTS ASContext
	{
	public:
		/*template <class Archive>
		friend void boost::serialization::serialize(Archive& ar, AOIContext& val, const unsigned int version);*/

		ASContext();
		static ASContext& GetInstance();

		shared_ptr<UserManage> GetUserManagedata();
	private:
		
		shared_ptr<UserManage> m_ptrUserManagedata;        //Êý¾Ý
	};
}


#endif