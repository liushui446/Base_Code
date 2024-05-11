#ifndef _AS_IO_HPP_
#define _AS_IO_HPP_

#include "as/core/def.h"
#include <string>
#include <map>
#include <vector>

namespace as
{
    AS_EXPORTS void IOTest();

	class AS_EXPORTS IniFile
	{
	public:
		void SetIniFilePath(const std::string& filepath);
		std::string ReadIniContent(const std::string& section, const std::string& item, std::string value);
		bool WriteIniContent(const std::string& section, const std::string& item, const std::string& value);

		/*
		写配置文件：清空当前文件重新写入文件数据
		*/
		bool WriteLightIniContent(std::map<std::string, std::map<std::string, std::vector<std::string>>> iniContent, bool isSearch);
	
    private:
		void TrimLeft(std::string& str);
		void TrimRight(std::string& str);
		void TrimBoundary(std::string& str);

		std::string m_FilePath;
	};
}

#endif