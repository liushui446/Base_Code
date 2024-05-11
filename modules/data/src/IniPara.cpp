#include <as/IniPara.hpp>
#include <as/io.hpp>
#include <codecvt>
#include "as/MacType.hpp"
namespace as
{
	string wstring2string(const std::wstring& ws)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		string result = converter.to_bytes(ws);
		return result;
	}

	IniParameter& IniParameter::GetInstance()
	{
		static IniParameter instance;
		return instance;
	}

	IniParameter::IniParameter()
	{
		IniFile readin;
		//获取当前路径并查找切割process
		std::wstring current_path = std::filesystem::current_path();
		string light_file_name = wstring2string(current_path);
		int pos = light_file_name.find("build");
		light_file_name.erase(pos);
		light_file_name += "IniFile\\CommonConfig.ini";
		readin.SetIniFilePath(light_file_name);

		BoardOrigin = atof(readin.ReadIniContent("ProduceParameter", "BoardOrigin", "392").c_str());
		IsOnline = atoi(readin.ReadIniContent("OperationMode", "IsOnline", "1").c_str());
		ALGORITHM_IMG = atoi(readin.ReadIniContent("OperationMode", "ALGORITHM_IMG", "0").c_str());
		Camera_Mode = atoi(readin.ReadIniContent("OperationMode", "Camera_Mode", "1").c_str());
		AssembleMode = atoi(readin.ReadIniContent("OperationMode", "AssembleMode", "0").c_str());
		ImageDetectionMode = atoi(readin.ReadIniContent("OperationMode", "ImageDetectionMode", "1").c_str());

		MacType::GetInstance().SetMacType(MacModel(atoi(readin.ReadIniContent("MachineType", "Type", "0").c_str())));
	}
}
