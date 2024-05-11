#include "as/Common/UiCallback.h"


namespace as
{
	UiCallback& UiCallback::GetInstance()
	{
		static UiCallback instance;
		return instance;
	}

	void UiCallback::SetUiCallback(std::function<int(WidgetType widget_type, std::string info, int key_amount, bool diff_thread, bool isSplit)> func)
	{
		callback_func = func;
	}

	int UiCallback::GetUiCallback(WidgetType widget_type, std::string info, int key_amount, bool diff_thread, bool isSplit) {
		std::string log_info;
		if (widget_type == WidgetType::Error)
		{
			log_info = "[错误对话框]  " + info;
		}
		else if (widget_type == WidgetType::Warning)
		{
			log_info = "[警告对话框]  " + info;
		}
		else if (widget_type == WidgetType::Tip)
		{
			log_info = "[提示对话框]  " + info;
		}
		else if (widget_type == WidgetType::Question)
		{
			log_info = "[询问对话框]  " + info;
		}
		return callback_func(widget_type, info, key_amount, diff_thread, isSplit);
	}

	/***********元件模版管理会话框元件类型传输********/
	//保存算法界面的数据
	/*void UiCallback::SendSaveCompData(bool save_type, string ng_id)
	{
		m_SetSaveCompDataFunc(save_type, ng_id);
	}*/

	/*****元件模版管理会话框元件类型回调**************/
	//保存算法界面的数据响应
	/*void UiCallback::SetSaveCompDataFunc(std::function<void(bool save_type, string ng_id)> func)
	{
		m_SetSaveCompDataFunc = func;
	}*/

}