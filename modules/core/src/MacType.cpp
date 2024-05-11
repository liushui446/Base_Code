#include "as/MacType.hpp"

namespace as
{
	MacType::MacType()
	{
		Type = MacModel::SPI;
	}

	MacType::~MacType(){}

	//MacModel MacType::GetType()
	//{
	//	return Type;
	//}

	void MacType::SetMacType(MacModel type) {
		Type = type;
	}
	
	std::shared_ptr<as::MacType> as::MacType::pIns(new as::MacType);

	MacType& MacType::GetInstance()
	{
		return *(as::MacType::pIns);
	}

}