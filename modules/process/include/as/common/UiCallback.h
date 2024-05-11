#ifndef AS_UICALLBACK_H
#define AS_UICALLBACK_H

#include "as/CommonCore.hpp"
#include "as/MainWindowProcess.hpp"

namespace as
{
	enum WidgetType
	{
		Error = 0,
		Warning = 1,
		Tip = 2,
		Question = 3
	};

	class AS_EXPORTS UiCallback
	{
	public:
		UiCallback() {};

		static UiCallback& GetInstance();
		void SetUiCallback(std::function<int(WidgetType widget_type, std::string info, int key_amount, bool diff_thread, bool isSplit)> func);
		int GetUiCallback(WidgetType widget_type, std::string info, int key_amount = 2, bool diff_thread = false, bool isSplit = true);  // 返回值为按键编号
		

	private:
		//function变量定义
		std::function<int(WidgetType widget_type, std::string info, int key_amount, bool diff_thread, bool isSplit)> callback_func;
		
	};
}

#endif