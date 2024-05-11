#pragma once

#include <vector>
#include <memory>

#include "as/core/def.h"

namespace as
{
	enum class MacModel
	{
		AOI2D = 0,		//2D
		AOI3D = 1,		//3D
		SPI   = 2       //SPI
	};


	class AS_EXPORTS MacType
	{
	public:
		~MacType();
		MacModel GetType() { return Type; };
		void SetMacType(MacModel type);


		static MacType& GetInstance();

	private:
		MacType();
		MacModel Type;
		static std::shared_ptr<MacType> pIns;
	};

	static MacModel MacGetType()
	{
		MacModel type = as::MacType::GetInstance().GetType();
		return type;
	};
}
