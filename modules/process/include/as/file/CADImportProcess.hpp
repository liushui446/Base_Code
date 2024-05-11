#ifndef AS_CADIMPORTPROCESS_H
#define AS_CADIMPORTPROCESS_H

#include "as/CommonCore.hpp"
#include "as/CADCore.hpp"
#include "as/Common/UiCallback.h"

namespace as
{
	//文本类型
	enum class TEXT_TYPE
	{
		TEXT_ANSI = 0,
		TEXT_UTF8 = 1,
		TEXT_UTF8_BOM = 2,
		TEXT_UTF16_LE = 3,
		TEXT_UTF16_BE = 4,
		TEXT_UNKNOW = 5,
	};
	//CAD关键字
	enum class SectionKey
	{
		VERSION = 0,
		PCB = 1, 
		BOARD = 2, 
		PLACEMENT = 3
	};
	//表格列txt内容
	enum class ColunmName
	{
		Reference = 0,           //PCB元件位置号
		CompId = 1,				 //元件ID
		CompName = 2,			 //元件名
		BlockID = 3,			 //块ID
		ArrayID = 4,			 //拼板ID
		Skip = 5,				 //调试
		Position_x = 6,			 //坐标x
		Position_y = 7,			 //坐标y 
		Angle = 8,               //角度
		BodyPosition_x = 9,		 //框选坐标x
		BodyPosition_y = 10,	 //框选坐标y
		BodyWidth = 11,			 //框选宽
		BodyHeight = 12,		 //框选高
		SmtName = 13,			 //贴片机号
		SlotNum = 14,			 //站位号
		NozzleName = 15,		 //吸嘴号
		RoiWidth = 16,			 //框选宽
		RoiHeight = 17,			 //框选高
		PL = 18,			     //PL
		Comment = 19,			 //任意可选择
		FidShape = 20,			 //Mark点形状
		Fid1X = 21,				 //Mark点1坐标
		Fid1Y = 22,
		Fid2X = 23,				 //Mark点2坐标
		Fid2Y = 24,
		MachineNo = 25,			 //机器号
		PartName = 26,			 //元件名
		Description = 27,		 //描述
		Position_z = 28          //板子厚度
	};

	//cad旋转角度
	enum class Cad_Rotation
	{
		_0 = 0,
		_90 = 1, 
		_180 = 2,
		_270 = 3
	};
	//刻度单位
	enum class Unit
	{
		Inches = 0, 
		Mils = 1, 
		Centimeters = 2, 
		Micrometers = 3, 
		Micron = 4, 
		SCALE = 5
	};

	class AS_EXPORTS CADImportProcess
	{
	public:
		CADImportProcess::CADImportProcess();

		
	private:
		
	};
}

#endif