#ifndef SPI_INIPARA_HPP
#define SPI_INIPARA_HPP
#include "as/CommonCore.hpp"

namespace as
{
	class AS_EXPORTS IniParameter
	{
	public:
		static IniParameter& GetInstance();	
		double BoardOrigin;
		bool IsOnline;
		bool ALGORITHM_IMG;
		bool Camera_Mode;	
		bool Ismaglev;
		bool AssembleMode;
		bool ImageDetectionMode;

	private:
		IniParameter();
		~IniParameter() = default;
		IniParameter(const IniParameter&) = default;
		IniParameter& operator=(const IniParameter&) = default;
	};

#define IniParm IniParameter::GetInstance()
}



#endif // !_SMARTSM_INIPARAMETER