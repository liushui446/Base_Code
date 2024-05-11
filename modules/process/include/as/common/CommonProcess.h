#ifndef AS_COMMONPROCESS_H
#define AS_COMMONPROCESS_H

#include "as/CommonCore.hpp"

namespace as
{
	enum class LanguegeType
	{
		CHS = 0,
		ENG,
		KOR
	};

	class AS_EXPORTS CommonProcess
	{
	public:
		CommonProcess();
		~CommonProcess() {};

		static CommonProcess& GetInstance();
		string string_To_UTF8(const string& str);
		bool isChineseChar(char c);
		vector<string> SplitString(string str, bool isSplit=true);
		//判断值范围
		bool JudgeValueRange(string valueOfMin, string valueOfMax, string input);

		//工具接口
		string intToHex(int n);
		char hexUnit(unsigned char unit);

		void SetSignName(string data);
		string GetSignName();

	private:
		mutex mtx;
		std::function<void(int)> m_GUIFunc;
		string m_strUserName;
		string m_strDataSavePath;//数据保存路径
	};

}

#endif