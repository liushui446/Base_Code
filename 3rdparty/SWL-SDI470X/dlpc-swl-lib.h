/*------------------------------------------------------------------------------
 * Copyright (c) 2022 Senwaylight Incorporated - http://www.senwaylight.com/
 *------------------------------------------------------------------------------
 *
 * 深微光电DLPC光控制SDK ,提供纯C接口，支持C/C++
 * API底层支持通过USB或串口连接设备，控制系统。保持上层API一致性，通过SN或串口名来决定底层
 * 
 * 2.0.0  大版本更新，兼容MCU2.0.X版本功能
 * 2.0.3: 提供USB或串口升级MCU的API。要求MCU的基础版本>=2.0.0
 * 2.0.4  优化USB接口的烧录速度和访问稳定性
 * 2.0.5  优化linux下的USB接口访问独占释放问题
 * 2.2.1  修复串口备份错误,增加对MCU2.2.1的快速设置接口支持
 * 2.2.2  修复一些load，reload，增加跨平台固件基础库
 * 2.3.0  相对稳定版本
 * 2.3.1  修复固件制作splash源图片是1bit的BMP、PNG图片解析异常。
 * 2.3.2  修复USB插拔更新通知等异常
 * 2.3.3  修复固件制作路径不存在的异常问题
 * 2.3.4  创建固件功能支持修改子版本号
 * 2.3.6  优化制作固件功能
 * 2.3.8  修复API中生成3010 1D pattern错误, 增加检测模式的曝光参数API
 * 2.4.0  修复API中生成pattern长短轴反转和GUI不一致 优化linux下固件备份  Splash模式延迟优化
 */
#ifndef DLPC_SWL_LIB_H
#define DLPC_SWL_LIB_H 1

#define SDKVERSION {2,4,0}

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"


#ifdef WIN32
#define SWL_ThreadV __declspec(thread)
#ifdef SWL_EXPORT
#define SWL_API __declspec(dllexport)
#else
#define SWL_API __declspec(dllimport)
#endif

#else
//linux do not
#define SWL_API 
#define SWL_ThreadV __thread
#endif

	typedef enum
	{
		PAT_BITDEPTH_ONE = 1,
		PAT_BITDEPTH_EIGHT = 8
	} PAT_BitDepth_e;

	typedef enum
	{
		PAT_DIRECTION_HORIZONTAL,
		PAT_DIRECTION_VERTICAL
	} PAT_Direction_e;

	typedef enum
	{
		PAT_ILLUMINATION_NONE = 0,
		PAT_ILLUMINATION_RED = 1,
		PAT_ILLUMINATION_GREEN = 2,
		PAT_ILLUMINATION_BLUE = 4,
		PAT_ILLUMINATION_RGB = 7
	} PAT_IlluminationSelect_e;

	typedef enum {
		DMD_DLP2010,
		DMD_DLP3010,
		DMD_DLP4710
	} DMD_e;

	typedef enum {
		External,Internal,Splash
	} PatternMode;

	typedef enum {
		OneBitMono,OneBitRGB,EightBitMono, EightBitRGB
	} BitDepth;


	typedef struct
	{
		/**
		 * Number of bytes in the pixel array
		 */
		uint32_t PixelArrayCount;

		/**
		 * The 1-D pixel data array.
		 *
		 * For 1-bit patterns, each byte should 0 or 1.
		 * For 8-bit patterns, each byte can be 0-255.
		 * For horizontal patterns, the number of bytes should be equal to the DMD_HEIGHT
		 * For vertical patterns, the number of bytes should be equal to the DMD_WIDTH
		 * If the number of bytes is less than the expected value, the missing pixel
		 * data will be filled with zeros.
		 * If the number of bytes is greater than the expected value, the excess bytes
		 * are ignored.
		 */
		uint8_t* PixelArray;
	} PAT_PatternData_s;

	typedef struct
	{
		PAT_BitDepth_e     BitDepth;
		PAT_Direction_e    Direction;
		uint32_t                        PatternCount;
		PAT_PatternData_s* PatternArray;
	} PAT_PatternSet_s;

	/// <summary>
	/// Pattern set order entry 
	/// </summary>
	typedef struct
	{
		uint8_t                               PatternSetIndex;  //original pattern set index
		uint8_t                               NumDisplayPatterns;// Do not exceed the number of pats in the original pattern set
		PAT_IlluminationSelect_e			  IlluminationSelect;//R1,G2,B4,RGB7
		bool                                  InvertPatterns;   // Invert Pattern bit
		uint32_t                              IlluminationTimeInMicroseconds;
		uint32_t                              PreIlluminationDarkTimeInMicroseconds;
		uint32_t                              PostIlluminationDarkTimeInMicroseconds;
	} PAT_PatternOrderTableEntry_s;

#pragma pack(push)
#pragma pack(1)


	/// <summary>
	/// 静态时序表信息
	/// </summary>
	typedef void *SDIHandler,*SDIOTableHandler,*SDIPatternHandler;


	/// <summary>
	/// Flash数据类型
	/// </summary>
	typedef enum
	{
		ENTIRE_FLASH = 0x0,                        /* 全部 Flash */
		PATTERN_ORDERS = 0x02,                     /* 静态时序表集 */
		SPLASH_DATA = 0x08,                        /* 静态2D图片 */
		PATTERN_DATA = 0x0E,                       /* 内部1D图案和默认时序表 */
		
	} FlashDataType;

#define BIT(X) (1<<X)


//#ifndef _INCLUDED_CYUSBSERIAL_H_
//	/// <summary>
//	/// 错误码
//	/// </summary>
//	typedef enum _uint32_t {
//
//		CY_SUCCESS = 0,                         /*API returned successfully without any errors.*/
//		CY_ERROR_ACCESS_DENIED,                 /*Access of the API is denied for the application */
//		CY_ERROR_DRIVER_INIT_FAILED,            /*Driver initialisation failed*/
//		CY_ERROR_DEVICE_INFO_FETCH_FAILED,      /*Device information fetch failed */
//		CY_ERROR_DRIVER_OPEN_FAILED,            /*Failed to open a device in the library */
//		CY_ERROR_INVALID_PARAMETER,             /*One or more parameters sent to the API was invalid*/
//		CY_ERROR_REQUEST_FAILED,                /*Request sent to USB Serial device failed */
//		CY_ERROR_DOWNLOAD_FAILED,               /*Firmware download to the device failed */
//		CY_ERROR_FIRMWARE_INVALID_SIGNATURE,    /*Invalid Firmware signature in firmware file*/
//		CY_ERROR_INVALID_FIRMWARE,              /*Invalid firmware */
//		CY_ERROR_DEVICE_NOT_FOUND,              /*Device disconnected */
//		CY_ERROR_IO_TIMEOUT,                    /*Timed out while processing a user request*/
//		CY_ERROR_PIPE_HALTED,                   /*Pipe halted while trying to transfer data*/
//		CY_ERROR_BUFFER_OVERFLOW,               /*OverFlow of buffer while trying to read/write data */
//		CY_ERROR_INVALID_HANDLE,                /*Device handle is invalid */
//		CY_ERROR_ALLOCATION_FAILED,             /*Error in Allocation of the resource inside the library*/
//		CY_ERROR_I2C_DEVICE_BUSY,               /*I2C device busy*/
//		CY_ERROR_I2C_NAK_ERROR,                 /*I2C device NAK*/
//		CY_ERROR_I2C_ARBITRATION_ERROR,         /*I2C bus arbitration error*/
//		CY_ERROR_I2C_BUS_ERROR,                 /*I2C bus error*/
//		CY_ERROR_I2C_BUS_BUSY,                  /*I2C bus is busy*/
//		CY_ERROR_I2C_STOP_BIT_SET,              /*I2C master has sent a stop bit during a transaction*/
//		CY_ERROR_STATUS_MONITOR_EXIST           /*API Failed because the SPI/UART status monitor thread already exists*/
//
//
//	} CY_RETURN_STATUS;
//
//#endif

	typedef enum
	{
		TP_SOLID_FIELD = 0x0,                               /**< Solid Field */
		TP_HORIZONTAL_RAMP = 0x1,                           /**< Horizontal Ramp */
		TP_VERTICAL_RAMP = 0x2,                             /**< Vertical Ramp */
		TP_HORIZONTAL_LINES = 0x3,                          /**< Horizontal Lines */
		TP_DIAGONAL_LINES = 0x4,                            /**< Diagonal Lines */
		TP_VERTICAL_LINES = 0x5,                            /**< Vertical Lines */
		TP_GRID = 0x6,                                      /**< Grid */
		TP_CHECKERBOARD = 0x7,                              /**< Checkerboard */
		TP_COLORBARS = 0x8,                                 /**< Colorbars */
	} TestPattern;


	typedef struct {
		uint16_t width;//默认宽度
		uint16_t height;//支持的图像高度
	}EvmResolution;
	/// <summary>
	/// EVM信息结构体
	/// </summary>
	typedef struct SEvmInfo{
		/// <summary>
		/// DMD信息
		/// </summary>
		char dmd[20];

		//DLPC
		char dlpc[20];

		//DLPA名称
		char dlpa[20];

		//EVMsoft
		char evmsoft[20];

		//固件
		char firmware[20];

		//产品名称： SWL-SDI4701.
		char product[20];

		//支持的分辨率
		EvmResolution resolution;

	} EvmInfo;

	//LED信息
	typedef struct SLedInfo {
		bool RedEnable ;   //开关R灯,受限于系统状态。某些情况设置无效
		bool GreenEnable ; //开关G灯,受限于系统状态。某些情况设置无效
		bool BlueEnable;   //开关B灯,受限于系统状态。某些情况设置无效

		
		uint16_t RedCurrent ;// R- LED Currrent (PWM) min:91=1423ma;
		uint16_t GreenCurrent ;// G- LED Currrent (PWM) min:91=1423ma;
		uint16_t BlueCurrent ;// B- LED Currrent (PWM) min:91=1423ma;

	}LedInfo;

	/// <summary>
	/// 触发设置
	/// </summary>
	typedef struct {
		uint8_t TriggerIn;  //是否开启triggerIN，是否响应外部触发
		uint8_t DlpcTrigger;//是否PWM高帧率触发模式
		uint16_t TriggerInFilter;//触发过滤时间（us）。

		uint8_t TriggerOut;  //是否开启triggerOut输出，影响外部电平
		uint8_t TriggerLog;// 是否输出串口触发日志，开启后，输出AB0400,AB0411
		int16_t TriggerOutDelay;//触发输出延迟时间(us)，可以是微小的负数，实现提前。最大值<+32767。

		uint8_t TriggerTableNo; //时序表号，0默认时序，1-62为自定义时序表号

		uint8_t Flag;// 强制要求校验包头BB；1：开启，0：默认关闭。 建议保持默认关闭状态，否则可能
		int16_t LoadDelay;//pattern set load，扩展延迟，用于控制高帧率触发的扩展延迟。会降低帧率。默认为0或者-1

		uint16_t RCurrent;  //RGB3灯开机后的默认电流，设置为0或0xffff后，则忽略此设置。该设置区别于DLP启动初始电流。
		uint16_t GCurrent;
		uint16_t BCurrent;


	} SDITriggerSet;

	/// <summary>
	/// 触发状态
	/// </summary>
	typedef struct {
		uint32_t TriggerInTimes;   //触发次数
		uint8_t runing;          //触发进行状态
		uint8_t totalSets;       //时序条目数=patset数
		uint16_t totalPats;      //时序总pat数
		uint8_t outSets;     //获取pat rdy次数
		uint16_t outPats;    //获取输出的pat数
		uint16_t TriggerRebootTimes;//触发异常重启次数;
		uint16_t counter;//内部dlpc trigger 计数器
	} SDITriggerStat;

	/// <summary>
	/// 设备信息
	/// </summary>
	typedef struct {
		char sn[20]; // SN
		uint8_t type;// type:1 为新设备，0或FF为旧设备
	} Devinfo;



	/// <summary>
	/// 内部曝光时序项0
	/// </summary>
	typedef struct 
	{
		uint8_t  PatternSetIndex;   //PatSet 索引号
		uint8_t  NumDisplayPatterns; //显示pattern数，要求设置为和实际patset中存储的pat数一致。否则可能导致高帧率触发异常。
		uint8_t  IlluminationSelect;//1R,2G,4B,7RGB
		uint32_t PatternInvert0;   //翻转前32个比特位。0或255
		uint32_t PatternInvert1;   //翻转后32个比特位。0或255
		uint32_t IlluminationTime;   //曝光时间 (us)
		uint32_t PreIlluminationDarkTime;//曝光前暗场时间 (us)
		uint32_t PostIlluminationDarkTime;//曝光后暗场时间 (us)
	} PatternOrderTableEntry;


	typedef struct
	{
		uint8_t     BitDepth;
		uint8_t     Direction;
		uint32_t                        PatternCount;
		char* PatternPath[256];
	} PatternSetInfo;


	typedef enum 
	{
		ST_ONE_BIT_MONO = 0x0,                              /**< 1 Bit Mono */
		ST_ONE_BIT_RGB = 0x1,                               /**< 1 Bit RGB */
		ST_EIGHT_BIT_MONO = 0x2,                            /**< 8 Bit Mono */
		ST_EIGHT_BIT_RGB = 0x3,                             /**< 8 Bit RGB */
	} SequenceType;
	
	/// <summary>
	/// 用位表示RGB 3灯控制状态。 //1R,2G,4B,7RGB
	/// </summary>
	typedef struct{
		
		bool RedIlluminator : 1;
		bool GreenIlluminator : 1;
		bool BlueIlluminator : 1;
		uint8_t : 5;
	} Illuminator;

	/// <summary>
	/// 外部模式和Splash模式的Pattern设置
	/// </summary>
	typedef struct
	{
		uint8_t SequenceType;//0=1 Bit Mono,1=1 Bit RGB,2=8 Bit Mono,3=8 Bit RGB，Splash模式不支持RGB。
		uint8_t NumberOfPatterns;//Splash:1-16(1-bit patterns),1-2(8-bit patterns);    External: 1-3(8-bit patterns),  1-24(1-bit patterns)
		Illuminator Illuminator;//3灯控制，Splash模式不支持RGB。
		uint32_t IlluminationTime;
		uint32_t PreIlluminationDarkTime;
		uint32_t PostIlluminationDarkTime;
	} PatternConfiguration;



	//时序表项的
	typedef struct {
		uint32_t Size;  //实际长度
		uint32_t Offset;//上一个add的offset+size。必须为4的整数倍，否则填充空隙1-3个0xFF。最后1个offset+size<12288
	} OrderTableAddress;

	#define MAXSIZE 12288
	typedef struct {
		char ID[4];					 //4个固定标记字符：BATC
		uint32_t Num;                //时序表数量+1，[1-63]，第0个系统内置配置。
		OrderTableAddress Tables[63];//第0个为系统默认用，第1-62个为客户配置的时序表索引
		char Main[1024 - 512];        //系统默认配置的空间，请保持原样或由SWL提供。实际使用的大小由Tables[0].size指定。多余的填充的为0xff。
		char OrderDatas[MAXSIZE - 1024];//62个时序表数据总长度不能超过这个值。MAXSIZE<=12288
		/**
		因此第一个时序配置的起始地址，Tables[1].Offset=1024;
		OrderDatas内部的块的划分，可以由客户自己规划最多62个时序表的存储，如按指定大小，或者按实际大小，依次紧靠，必须确保起始位置是4的倍数。
		OrderDatas总大小，可以设计为动态的，最大长度<=12288 - 1024

		每个OrderTable的数据参考I2C动态加载时序的指令。
		Byte 0 Write Pattern Order Table Entry (98h)
		Byte 1 Write Control
		Byte 2 Pattern Set Entry Index
		Byte 3 Number of patterns to display
		Byte 4 Illumination Select
		Byte 5 Pattern Invert LSWord LSByte
		Byte 6 Pattern Invert LSWord
		Byte 7 Pattern Invert LSWord
		Byte 8 Pattern Invert LSWord MSByte
		Byte 9 Pattern Invert MSWord LSByte
		Byte 10 Pattern Invert MSWord
		Byte 11 Pattern Invert MSWord
		Byte 12 Pattern Invert MSWord MSByte
		Byte 13 Illumination Time (µs) LSByte
		Byte 14 Illumination Time (µs)
		Byte 15 Illumination Time (µs)
		Byte 16 Illumination Time (µs) MSByte
		Byte 17 Pre-illumination Dark Time (µs) LSByte
		Byte 18 Pre-illumination Dark Time (µs)
		Byte 19 Pre-illumination Dark Time (µs)
		Byte 20 Pre-illumination Dark Time (µs) MSByte
		Byte 21 Post-illumination Dark Time (µs) LSByte
		Byte 22 Post-illumination Dark Time (µs)
		Byte 23 Post-illumination Dark Time (µs)
		Byte 24 Post-illumination Dark Time (µs) MSByte
		....
		根据需求重复以上条目。


		**/
	} OrderTableDatas;

	/// <summary>
	/// 回调消息结构体
	/// </summary>
	typedef struct  {
		char* sn;// 设备名称
		char* op;// 操作名称
		uint32_t step; //操作步骤
		float completed;//进度
		uint64_t useTime;//耗时毫秒数
		char* message;  //消息描述

	}CallbackMessage;
#pragma pack(pop)
	/// <summary>
	/// 库初始化
	/// </summary>
	/// <returns></returns>
	SWL_API bool SWL_Init();

	 /// <summary>
	 /// 多设备情况下连接指定SN设备（sn名称通过SWL_Devices得到），返回单独的SDI设备操作句柄
	 /// </summary>
	 /// <param name="SwlIdx"></param>
	 /// <returns></returns>
	SWL_API SDIHandler  SWL_SDIConnect(const char* SwlIdx);

	/// <summary>
	/// 关闭并释放SDI设备句柄
	/// </summary>
	/// <param name="handler"></param>
	/// <returns></returns>
	SWL_API void  SWL_SDIClose(SDIHandler handler);

	/// <summary>
	/// 设置光机电流
	/// </summary>
	/// <param name="RCurrent"></param>
	/// <param name="GCurrent"></param>
	/// <param name="BCurrent"></param>
	/// <returns></returns>
	SWL_API uint32_t  SWL_SetLedCurrent(uint16_t RCurrent, uint16_t GCurrent, uint16_t BCurrent);

	/// <summary>
	/// 设置指定光机电流
	/// </summary>
	/// <param name="handler"></param>
	/// <param name="RCurrent"></param>
	/// <param name="GCurrent"></param>
	/// <param name="BCurrent"></param>
	/// <returns></returns>
	SWL_API uint32_t  SWL_SetLedCurrentX(SDIHandler handler, uint16_t RCurrent, uint16_t GCurrent, uint16_t BCurrent);


	/// <summary>
	/// 初始化默认分辨率，如：4710-1080P,3010-720P，2010-480P
	/// </summary>
	/// <returns></returns>
	SWL_API uint32_t SWL_InitResolution();

	/// <summary>
	/// 设置色温索引
	/// </summary>
	/// <param name="lookno"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SetLook(uint8_t lookno);

	/// <summary>
	/// 设置显示图像大小
	/// </summary>
	/// <param name="size"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_WriteDiplaySize(EvmResolution* size);

	/// <summary>
	/// 读取显示图像大小
	/// </summary>
	/// <param name="size"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_ReadDiplaySize(EvmResolution* size);

	/// <summary>
	/// 16进制字符串形式打印字节数组
	/// </summary>
	/// <param name="buf">字节数组</param>
	/// <param name="num">长度</param>
	/// <returns></returns>
	SWL_API void SWL_HexPrint(const unsigned char* bytes, const int num);

	/// <summary>
	/// byte转化成16进制的字符串形式，中间用空格隔开
	/// </summary>
	/// <param name="bytes">字节缓冲区</param>
	/// <param name="byteSize">字节大小</param>
	/// <param name="str">输出字符串缓冲区</param>
	/// <param name="split">分割字节数组的字符串</param>
	SWL_API void SWL_HextoString(const unsigned char* bytes, const int byteSize, char* str, char split);

	/// <summary>
	/// 16进制的字符串形式转化成byte数组，XXYYZZ
	/// </summary>
	/// <param name="bytes">字节缓冲区</param>
	/// <param name="byteSize">字节大小</param>
	/// <param name="str">源字符串</param>
	/// <returns></returns>
	SWL_API void SWL_HexfromString(unsigned char* bytes, const int byteSize, const char* str);


	/// <summary>
	/// 获取最后一个底层错误信息
	/// </summary>
	/// <param name="status">错误码</param>
	/// <param name="statusText">错误描述</param>
	/// <returns></returns>
	SWL_API void SWL_CypressLastError(uint32_t* status, char* statusText);

	/// <summary>
	/// 获取选中的光机SN或序号
	/// </summary>
	/// <param name="SwlIdx"></param>
	/// <returns></returns>
	SWL_API void  SWL_GetUsbIdx(char* SwlIdx);


	/// <summary>
	/// 获取默认或最后一次切换的光机的设备名称:SN或串口号
	/// </summary>
	/// <returns></returns>
	SWL_API char* SWL_GetCurDevName();

	/// <summary>
	/// 设置光机USB序号。同时连接多个USB光机时，指定光机的序号。
	/// 等同于父进程设置环境变量:
	/// WINDOWS:set SWL_USB_IDX=%SwlIdx%
	/// LINUX:  export SWL_USB_IDX=%SwlIdx%
	/// </summary>
	/// <param name="SwlIdx">光机设备序号，参考SWL_Devies方法返回值中的serial number</param>
	/// <returns></returns>
	SWL_API void  SWL_SetUsbIdx(const char* SwlIdx);

	/// <summary>
	/// 获取设备id列表，最多10个设备，包含USB设备和串口设备，串口设备显示为串口号
	/// </summary>
	/// <param name="devices">设备序列号ID列表</param>
	/// <param name="nums">设备数量</param>
	/// <returns></returns>
	SWL_API bool  SWL_Devices(char devices[10][30], uint32_t*nums);


	/// <summary>
	/// 写入DLPC命令
	/// </summary>
	/// <param name="wlen">命令长度</param>
	/// <param name="wbuf">命令数据</param>
	/// <returns></returns>
	SWL_API uint32_t  SWL_Write(int wlen, uint8_t* wbuf);

	/// <summary>
	/// 读写IIC子设备命令
	/// </summary>
	/// <param name="slaveAddr">0:DLPC,  1:MCU  2:AMS光传感器,或其他7位IIC地址</param>
	/// <param name="wlen">写入命令长度</param>
	/// <param name="wbuf">写入命令缓冲区</param>
	/// <param name="rlen">读取数据长度</param>
	/// <param name="rbuf">读取数据缓冲区</param>
	/// <returns>状态码，非0表示错误</returns>
	SWL_API uint32_t  SWL_CommandS(int slaveAddr, int wlen, uint8_t* wbuf, int rlen, uint8_t* rbuf);

	/// <summary>
	/// 读写命令，写入请求，返回数据
	/// </summary>
	/// <param name="wlen">命令长度</param>
	/// <param name="wbuf">命令内容</param>
	/// <param name="rlen">返回长度</param>
	/// <param name="rbuf">接收返回数据的缓冲区</param>
	/// <returns></returns>
	SWL_API uint32_t  SWL_Command(int wlen, uint8_t* wbuf, int rlen, uint8_t* rbuf);

	/// <summary>
	/// 运行EVM-GUI中Debug导出的批处理文件
	/// </summary>
	/// <param name="bfFile">BF文件路径</param>
	/// <returns></returns>
	SWL_API void SWL_RunBatchFile(char* bfFile);




	/// <summary>
	/// 设置光控制模式，
	/// </summary>
	/// <param name="mode">3外部patter流模式,4内部pattern模式 5splash pattern模式</param>
	/// <returns>0表示成功，非0为I2C错误</returns>
	SWL_API uint32_t SWL_SetMode(int mode);

	/// <summary>
	/// 读取光控制模式，
	/// </summary>
	/// <param name="mode">3外部patter流模式,4内部pattern模式 5splash pattern模式</param>
	/// <returns>0表示成功，非0为I2C错误</returns>
	SWL_API uint32_t SWL_GetMode(int* mode);


	/// <summary>
	/// 动态加载时序表集合，命令行参数方式传入字符串数组配置。动态时序表后断电后丢失。如果要支持触发时序。需要手动调用SWL_SDIOrderUpdate
	/// 数组中每一个字符串配置项如：1,2,[2,]2500,7000,2500   
	/// 分别表示patternset索引,pattern数量,led灯(此项可省略默认为G灯，R:1,G:2,B:4,RGB:7),pre dark,exposure,post dark
	/// 其中分隔符可以用","或者"-"，如1-2-[2-]2500-7000-2500  
	/// </summary>
	/// <param name="argv">字符串数组</param>
	/// <param name="argc">数组总长度</param>
	/// <param name="start">数组起始索引,如果出错，更新为错误的位置</param>
	/// <returns>0表示成功，非0为I2C错误</returns>
	SWL_API uint32_t SWL_PatternOrderLoad(char* argv[], int argc, int* start);

	/// <summary>
	/// 动态加载时序表集合，功能同SWL_PatternOrderLoad。动态时序表后断电后丢失。如果要支持触发时序。需要手动调用SWL_SDIOrderUpdate
	/// </summary>
	/// <param name="Entry">时序表集合第一个指针</param>
	/// <param name="num">总数</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_PatternOrderLoadEntries(PatternOrderTableEntry Entry[], uint8_t num);

	/// <summary>
	/// 动态加载时序表集合，功能同SWL_PatternOrderLoad。动态时序表后断电后丢失。如果要支持触发时序。需要手动调用SWL_SDIOrderUpdate
	/// </summary>
	/// <param name="handler">SDI设备句柄</param>
	/// <param name="Entry">时序表集合第一个指针</param>
	/// <param name="num">总数</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_PatternOrderLoadEntriesX(SDIHandler handler, PatternOrderTableEntry Entry[], uint8_t num);

	/// <summary>
	/// 从新加载Flash默认保存的patter时序
	/// </summary>
	/// <returns>0表示成功，非0为I2C错误</returns>
	SWL_API uint32_t SWL_PatternOrderReloadFromFlash();

	/// <summary>
	/// 获取当前设备中PatternOrderEntries数量
	/// </summary>
	/// <param name="num">返回数量</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_PatternOrderEntriesNum(int* num);

	/// <summary>
	/// 获取当前设备中PatternOrderEntries拼接内容，按空格隔开每个Entry。每个Entry的格式参考SWL_PatternOrderLoad
	/// </summary>
	/// <param name="buffer">字符串缓冲区,建议256*30</param>
	/// <param name="readNums">读取的数量</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_PatternOrderRead(char* buffer, int readNums);


	/// <summary>
	/// 获取当前设备指定索引序号的Entry信息。
	/// </summary>
	/// <param name="entry">传入PatternOrderTableEntry的指针，读取后更新</param>
	/// <param name="entryIndex">读取表项索引号，小于总时序项数</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_PatternOrderTableEntryRead(PatternOrderTableEntry* entry, int entryIndex);



	/// <summary>
	/// 读取EVM信息
	/// </summary>
	/// <param name="evm">evm信息</param>
	/// <returns>0表示成功，非0为I2C错误</returns>
	SWL_API uint32_t SWL_GetEvmInfo(EvmInfo* evm);

	/// <summary>
	/// 获取DMD信息
	/// </summary>
	/// <param name="dmd_e">传入类型对象指针</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetDmdInfo(DMD_e* dmd_e);


	/// <summary>
	/// 获取最大PWM电流
	/// </summary>
	/// <param name="RCurrent"></param>
	/// <param name="GCurrent"></param>
	/// <param name="BCurrent"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetMaxCurrent(uint16_t* RCurrent, uint16_t* GCurrent, uint16_t* BCurrent);


	/// <summary>
	/// 读取LED信息
	/// </summary>
	/// <param name="handler">设备句柄</param>
	/// <param name="led">led信息</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetLedInfoX(SDIHandler handler, LedInfo* led);

	/// <summary>
	/// 读取LED信息
	/// </summary>
	/// <param name="led">led信息</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetLedInfo(LedInfo* led);

	/// <summary>
	/// 设置LED信息
	/// </summary>
	/// <param name="led">led信息</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SetLedInfo(LedInfo* led);


	/// <summary>
/// 读取LED电流信息
/// </summary>
/// <param name="led">led信息</param>
/// <returns></returns>
	SWL_API uint32_t SWL_GetLedCurrent(LedInfo* led);

	/// <summary>
	/// 读取指定光机LED电流信息
	/// </summary>
	/// <param name="handler">设备句柄</param>
	/// <param name="led">led信息，仅输出电流，忽略开关设置</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetLedCurrentX(SDIHandler handler, LedInfo* led);

	/// <summary>
	/// 运行一次，开启高帧率后不直接投影，高帧率请使用trigger命令
	/// </summary>
	/// <returns></returns>
	SWL_API uint32_t SWL_Runonce();


	/// <summary>
	/// 一直运行，开启高帧率后不直接投影，高帧率请使用trigger命令
	/// </summary>
	/// <returns></returns>
	SWL_API uint32_t SWL_RunContinuously();



	/// <summary>
	/// 运行并重复repeat次,总共运行1+repeat次周期.0=SWL_Runonce，255=SWL_RunContinuously
	/// </summary>
	/// <param name="repeat">repeat，重复次数：0-255;0=SWL_Runonce，255=SWL_RunContinuously</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_RunRepeat(uint8_t repeat);


	/// <summary>
	/// 停止运行
	/// </summary>
	/// <returns></returns>
	SWL_API uint32_t SWL_RunStop();

	/// <summary>
	/// 运行控制
	/// </summary>
	/// <param name="contrl">0h: Start 1h: Stop 2h : Pause  3h : Step  4h : Resume  5h : Reset</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_RunControl(uint8_t contrl);


	/// <summary>
	/// 设置triggin
	/// </summary>
	/// <param name="enabled">启用1,禁用0</param>
	/// <param name="polarity">拉高触发1,拉低触发0</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SetTriggerin(bool enabled, bool polarity);

	/// <summary>
	/// 获取triggerin 状态
	/// </summary>
	/// <param name="enabled">是否启用</param>
	/// <param name="polarity">是否高触发</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetPatternReady(bool* enabled, bool* polarity);


	/// <summary>
	/// 设置tPatternReady
	/// </summary>
	/// <param name="enabled">启用1,禁用0</param>
	/// <param name="polarity">拉高触发1,拉低触发0</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SetPatternReady(bool enabled, bool polarity);

	/// <summary>
	/// 获取tPatternReady 状态
	/// </summary>
	/// <param name="enabled">是否启用</param>
	/// <param name="polarity">是否高触发</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetTriggerin(bool* enabled, bool* polarity);


	/// <summary>
	/// 设置触发输出
	/// </summary>
	/// <param name="outport">0：OUT1; 1:OUT2</param>
	/// <param name="enabled">0不启用，1启用</param>
	/// <param name="inversion">0不翻转，1翻转</param>
	/// <param name="delay">延迟触发(us)，16位有效</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SetTriggerout(int outport,bool enabled, bool inversion, int32_t delay);

	/// <summary>
	/// 读取触发输出
	/// </summary>
	/// <param name="outport">0：OUT1; 1:OUT2</param>
	/// <param name="enabled">0不启用，1启用</param>
	/// <param name="inversion">0不翻转，1翻转</param>
	/// <param name="delay">延迟触发(us)，16位有效</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetTriggerout(int outport,bool* enabled, bool* inversion, int32_t* delay);

	/// <summary>
	/// 显示Splash图片
	/// </summary>
	/// <param name="splashIndex">Splash图片序号，0-N</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_Splash(uint8_t splashIndex);

	/// <summary>
	/// 加载Splash Pattern模式，参考SWL_SplashPatternSet
	/// </summary>
	/// <param name="splashIndex">splash图的索引号</param>
	/// <param name="led">LED灯，1:R,2:G,4:B,7:RGB</param>
	/// <param name="bits">0h: 1-bit mono,1h: 1 - bit RGB,2h : 8 - bit mono,3h : 8 - bit RGB</param>
	/// <param name="ppf">每帧pattern数，1.....2,3,16,24</param>
	/// <param name="predark">前暗场时间 单位us</param>
	/// <param name="exposure">曝光时间 单位us</param>
	/// <param name="postdark">后暗场时间 单位us</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SplashPattern(uint8_t splashIndex, uint8_t led, uint8_t bits, uint8_t ppf, uint32_t predark, uint32_t exposure, uint32_t postdark);

	/// <summary>
	/// 简单测试模式
	/// </summary>
	/// <param name="splashIndex">模式类型</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_Test(TestPattern pattern);

	/// <summary>
	/// 进入HDMI视频流输入模式
	/// </summary>
	/// <returns></returns>
	SWL_API uint32_t SWL_Hdmi();


	/// <summary>
	/// 加载Hdmi Pattern模式，等同参考SWL_ExternalPatternSet
	/// </summary>
	/// <param name="led">LED灯，1:R,2:G,4:B,7:RGB</param>
	/// <param name="bits">0h: 1-bit mono,1h: 1 - bit RGB,2h : 8 - bit mono,3h : 8 - bit RGB</param>
	/// <param name="ppf">每帧pattern数，1.....2,3,16,24</param>
	/// <param name="predark">前暗场时间 单位us</param>
	/// <param name="exposure">曝光时间 单位us</param>
	/// <param name="postdark">后暗场时间 单位us</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_HdmiPattern(uint8_t led, uint8_t bits, uint8_t ppf, uint32_t predark, uint32_t exposure, uint32_t postdark);

	/// <summary>
	/// 停止投影和同步信号
	/// </summary>
	/// <returns></returns>
	SWL_API uint32_t SWL_StopProject();

	/// <summary>
	/// 回调函数，指定设备名，操作名，步骤，进度，耗时，消息多状态
	/// </summary>
	//typedef void (*ProgramCallback)(CallbackMessage* msg);

	/// <summary>
	/// 多固件同时刷写的回调函数原型
	/// </summary>
	typedef void (*ProgramCallback)(char* sn,uint32_t step, float completed, char* message);



	/// <summary>
	/// 将internal pattern bin文件写入到dlpc的flash里面。写入后建议重新上电。
	/// </summary>
	/// <param name="patternbin"></param>
	/// <param name="Callback">回调可以设置为NULL</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_WritePatternToFlash(char* patternbin, ProgramCallback Callback);

	/// <summary>
	/// 烧录pattern order table多套静态时序组合文件到数据
	/// </summary>
	/// <param name="orderbin"></param>
	/// <param name="Callback"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_WriteBatchFileToFlash(char* orderbin, ProgramCallback Callback);

	/// <summary>
	/// 刷写完整固件，可以指定只烧录完整固件中的部分，可以设置回调返回进度
	/// </summary>
	/// <param name="imgfile">完整固件路径</param>
	/// <param name="partype">数据类型，ENTIRE_FLASH ，PATTERN_ORDERS，SPLASH_DATA，PATTERN_DATA</param>
	/// <param name="Callback">回调可以设置为NULL</param>
	/// <param name="cancel">允许中途取消变量，可以设置为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_WriteFirmware(char* imgfile, FlashDataType partype,ProgramCallback callback,bool* cancel);

	/// <summary>
	/// 指定设备SN号刷写img固件，SN为空取默认或环境变量的，可以指定只烧录固件中的部分，可以设置回调返回进度
	/// 用于并发烧录多台设备
	/// </summary>
	/// <param name="sn">设备号</param>
	/// <param name="imgfile">完整固件路径</param>
	/// <param name="partype">数据类型，ENTIRE_FLASH ，PATTERN_ORDERS，SPLASH_DATA，PATTERN_DATA</param>
	/// <param name="Callback">回调可以设置为NULL</param>
	/// <param name="cancel">允许中途取消变量，可以设置为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_WriteFirmwareEx(char* sn,char* imgfile, FlashDataType partype, ProgramCallback callback, bool* cancel);

	/// <summary>
	/// 刷写img固件，可以设置回调返回进度
	/// </summary>
	/// <param name="imgfile"></param>
	/// <param name="Callback">回调可以设置为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_WriteFirmwareToFlash(char* imgfile, ProgramCallback Callback);




	/// <summary>
	/// 烧录数据块文件，如果指定为ENTIRE_FLASH类型，等同于SWL_WriteFirmwareToFlash
	/// </summary>
	/// <param name="binfile">数据文件路径</param>
	/// <param name="partype">数据类型：ENTIRE_FLASH ，PATTERN_ORDERS，SPLASH_DATA，PATTERN_DATA</param>
	/// <param name="Callback">回调可以设置为NULL</param>
	/// <param name="cancel">允许中途取消变量，可以设置为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_WriteFlashBlock(const char* binfile, FlashDataType partype, ProgramCallback Callback, bool* cancel);




	/// <summary>
	/// 获取FLASH固件总字节数
	/// </summary>
	/// <returns>FLASH固件总字节数</returns>
	SWL_API uint32_t SWL_GetFlashImageSize();

	/// <summary>
	/// 获取指定设备FLASH固件总字节数
	/// </summary>
	/// <param name="sn"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_GetFlashImageSizeX(char* sn);


	/// <summary>
	/// 备份完整固件到文件
	/// </summary>
	/// <param name="imgfile">文件名</param>
	/// <param name="Callback">回调通知，可以为NULL</param>
	/// <param name="cancel">取消控制，可以为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_BackupFirmware(char* imgfile, ProgramCallback Callback, bool* cancel);

	/// <summary>
	/// 指定设备备份FLASH数据，一般为主控制器，可选副控制器
	/// </summary>
	/// <param name="sn">设备SN</param>
	/// <param name="imgfile">备份文件路径</param>
	/// <param name="Callback">回调</param>
	/// <param name="cancel">取消标志，可以为空</param>
	/// <param name="IsSlaveController">是否为副控制器</param>
	/// <returns></returns>
	SWL_API void SWL_BackupFirmwareEx(char* sn, char* imgfile, ProgramCallback Callback, bool* cancel, bool IsSlaveController);

	/// <summary>
	/// 选择性备份固件数据,底层兼容USB或串口
	/// </summary>
	/// <param name="backupfile">输出文件名</param>
	/// <param name="type">pattern,order,splash</param>
	/// <param name="Callback">通知回调，可以为空</param>
	/// <param name="cancel">取消标志，可以为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_BackupFlashBlock(char* backupfile, FlashDataType type, ProgramCallback Callback, bool* cancel);


	/// <summary>
	/// 光机软重启DLPC，烧录固件或更新pattern set，开机脚本等后。可以执行此操作，实现软重启
	/// </summary>
	/// <returns></returns>
	SWL_API bool SWL_SDIReboot();


	/// <summary>
	/// 光机复位，MCU系统重启，DLP同步重启
	/// </summary>
	/// <returns></returns>
	SWL_API bool SWL_SDIReset();


	/// <summary>
	/// 使用USB连接光机，出现连接异常问题，可尝试使用此API恢复。大多数正常情况无须调用
	/// </summary>
	/// <returns></returns>
	SWL_API bool SWL_SDIResetUSB();

	/// <summary>
	/// 光机级联时序更新。动态烧录时序后，需要通知Mcu更新时序计数功能。
	/// </summary>
	/// <returns></returns>
	SWL_API bool SWL_SDIOrderUpdate();


	/// <summary>
	/// SDI光机级联trigger in.会同步发出trigger out信号,投影完成后自动触发下一台
	/// </summary>
	/// <returns></returns>
	SWL_API bool SWL_SDITriggerIn();
	/// <summary>
	/// 指定SDI光机级联trigger in.会同步发出trigger out信号,投影完成后自动触发下一台
	/// </summary>
    /// <param name="handler">句柄</param>
	/// <returns></returns>
	SWL_API bool SWL_SDITriggerInX(SDIHandler handler);


	/// <summary>
	/// 持久化设置触发功能，重新上电不丢失
	/// </summary>
	/// <param name="set"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDITriggerSet(SDITriggerSet set);

	/// <summary>
	/// 快速扩展设置，MCU2.0.0之后版本有效
	/// </summary>
	/// <param name="handler">设备句柄</param>
	/// <param name="set">设置</param>
	/// <param name="disabledPersisent">flase默认持久化,  true禁用持久化MCU>=2.2.1有效</param>
	/// <returns></returns>
	SWL_API bool SWL_SDITriggerSetX(SDIHandler handler, SDITriggerSet set, bool disabledPersisent);

	/// <summary>
	/// 读取触发设置
	/// </summary>
	/// <param name="set"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDITriggerSetRead(SDITriggerSet* set);

	/// <summary>
	/// 读取触发状态
	/// </summary>
	/// <param name="stat"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDITriggerStatRead(SDITriggerStat* stat);


	/// <summary>
	 /// 临时切换静态时序表，0为恢复默认，1-62为用户刷入的时序表
	 /// </summary>
	 /// <param name="tableNo"></param>
	 /// <returns></returns>
	SWL_API bool SWL_SDISelectedOrderTable(uint8_t tableNo);

	/// <summary>
   /// 持久化切换静态时序表，0为恢复默认，1-62为用户刷入的时序表。
   /// </summary>
   /// <param name="tableNo"></param>
   /// <returns></returns>
	SWL_API bool SWL_SDISelectedOrderTableP(uint8_t tableNo);

	/// <summary>
   /// 指定光机临时切换静态时序表，0为恢复默认，1-62为用户刷入的时序表
   /// </summary>
   /// <param name="handler">句柄</param>
   /// <param name="tableNo"></param>
   /// <returns></returns>
	SWL_API bool SWL_SDISelectedOrderTableX(SDIHandler handler, uint8_t tableNo);

	/// <summary>
   /// 指定光机持久化切换静态时序表，0为恢复默认，1-62为用户刷入的时序表。
   /// </summary>
   /// <param name="handler">句柄</param>
   /// <param name="tableNo"></param>
   /// <returns></returns>
	SWL_API bool SWL_SDISelectedOrderTablePX(SDIHandler handler, uint8_t tableNo);

	/// <summary>
	/// 读取SDI设备信息，包含SN和类型
	/// </summary>
	/// <param name="info"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDIDevinfo(Devinfo* info);

	/// <summary>
	/// 统一设置电流（mA）
	/// </summary>
	/// <param name="current_ma">电流值</param>
	/// <returns></returns>
	SWL_API bool SWL_SDISetAllCurrent(uint16_t current_ma);

	/// <summary>
	/// 读取光机温度
	/// </summary>
	/// <param name="centigrade"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDICentigrade(uint8_t* centigrade);

	/// <summary>
	/// 读取用户数据，最大64个字节
	/// </summary>
	/// <param name="data"></param>
	/// <param name="len"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDIGetUserData(uint8_t* data, int len);

	/// <summary>
	/// 设置用户数据64个字节，不够64字节的填充为0
	/// </summary>
	/// <param name="data"></param>
	/// <param name="len"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDISetUserData(uint8_t* data, int len);

	/// <summary>
	/// 获取MCU版本号;传入3个字节的缓冲区存储版本号，如：01 09 10 （V1.9.16）
	/// </summary>
	/// <param name="data"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDISoftVersion(uint8_t* data);

	/// <summary>
	///  获取SDK版本号;传入3个字节的缓冲区存储版本号，如：01 09 10 （V1.9.16）
	/// </summary>
	/// <param name="data"></param>
	/// <returns></returns>
	SWL_API bool SWL_SDISDKVersion(uint8_t* data);



	/*2022-06-22 新接口，实现配置和烧录静态时序表，patterndata*/


	/// <summary>
	/// 连接设备打开静态时序表操作句柄
	/// </summary>
	/// <param name="num">返回数量</param>
	/// <returns></returns>
	SWL_API SDIOTableHandler SWL_SDIOrderTablesOpen();

	/// <summary>
	/// 关闭静态时序表句柄
	/// </summary>
	/// <param name="handler"></param>
	/// <returns></returns>
	SWL_API  void SWL_SDIOrderTablesClose(SDIOTableHandler handler);

	/// <summary>
	/// 获取静态时序表的数目。大于等于1，小于63
	/// </summary>
	/// <param name="handler"></param>
	/// <returns></returns>
	SWL_API  uint8_t SWL_SDIOrderTablesNum(SDIOTableHandler handler);

	/// <summary>
	/// 获取指定时序表中的时序项数目
	/// </summary>
	/// <param name="handler"></param>
	/// <param name="tabIndex"></param>
	/// <returns></returns>
	SWL_API uint8_t SWL_SDIOrderTableEntriesNum(SDIOTableHandler handler, uint8_t tabIndex);



	/// <summary>
	/// 获取指定静态时序表的配置信息
	/// </summary>
	/// <param name="handler">操作句柄</param>
	/// <param name="tabIndex">表索引</param>
	/// <param name="entryIndex">时序项索引</param>
	/// <param name="entry">时序信息输出对象地址</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIOrderTableEntryRead(SDIOTableHandler handler, uint8_t tabIndex, int entryIndex, PatternOrderTableEntry* entry);

	/// <summary>
	/// 设置静态开机默认控制脚本
	/// </summary>
	/// <param name="handler">句柄</param>
	/// <param name="mainBuffer">主缓冲区，小于400</param>
	/// <param name="num">长度</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIOrderTableSetMain(SDIOTableHandler handler, uint8_t* mainBuffer, int num);

	/// <summary>
	/// 重设指定时序表的时序项数量，同步更新时序表的数量。原时序表号大于tabIndex后的时序表被清空。时序表总数被重置为tabIndex+1
	/// </summary>
	/// <param name="handler">操作句柄</param>
	/// <param name="tabIndex">表索引，1-62，小于源时序表数量则更新，等于源时序表数量则新增，大于源时序表数量则无效</param>
	/// <param name="entNum">时序项数量</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIOrderTableSet(SDIOTableHandler handler, uint8_t tabIndex, int entNum);

	/// <summary>
	/// 设置指定静态时序表的配置信息
	/// </summary>
	/// <param name="handler">操作句柄</param>
	/// <param name="tabIndex">表索引，1-62</param>
	/// <param name="entryIndex">时序项索引</param>
	/// <param name="entry">时序信息输出对象地址</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIOrderTableEntrySet(SDIOTableHandler handler, uint8_t tabIndex, int entryIndex, PatternOrderTableEntry* entry);

	/// <summary>
	/// 下载静态时序表到设备
	/// </summary>
	/// <param name="handler">操作句柄</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIOrderTablesProgramLoad(SDIOTableHandler handler);


	
	/// <summary>
	/// 持久化设置RGB3灯电流,重启有效，使用PWM电流值
	/// </summary>
	/// <param name="ledinfo">PWM电流值</param>
	/// <returns></returns>
	SWL_API bool SWL_SDISetCurrent(LedInfo ledinfo);

	/// <summary>
	/// 读取SplashPattern设置，会被ExternalPattern覆盖
	/// </summary>
	/// <param name="splashIndex">读取splash索引</param>
	/// <param name="patternConfig">读取的设置</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SplashPatternConfigGet(uint8_t* splashIndex, PatternConfiguration* patternConfig);

	/// <summary>
	/// 读取外部pattern设置
	/// </summary>
	/// <param name="fps">读取的Frame帧率</param>
	/// <param name="patternConfig">读取的设置</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_ExternalPatternConfigGet(float* fps, PatternConfiguration* patternConfig);

	/// <summary>
	/// 设置SplashPattern 2D模式
	/// </summary>
	/// <param name="splashIndex"></param>
	/// <param name="patternConfig">注意，SplashPattern支持单通道模式</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_SplashPatternConfigSet(uint8_t splashIndex, PatternConfiguration patternConfig);

	/// <summary>
	/// 设置外部Pattern 2D模式
	/// </summary>
	/// <param name="patternConfig"></param>
	/// <returns></returns>
	SWL_API uint32_t SWL_ExternalPatternConfigSet(PatternConfiguration patternConfig);

	/// <summary>
	/// 检测模式的曝光参数
	/// </summary>
	/// <param name="patternMode">模式选择</param>
	/// <param name="bitDepth">位深和通道</param>
	/// <param name="exposureTime">传入曝光时间</param>
	/// <param name="support">输出支持的曝光设置</param>
	/// <param name="validExposureTime">输出验证的曝光时间</param>
	/// <param name="MinimumPreDarkTime">输出最小前暗场时间</param>
	/// <param name="minimumPostDarkTime">输出最小后暗场时间</param>
	/// <returns></returns>
	SWL_API uint32_t SWL_ValidateExposureTime(PatternMode patternMode, BitDepth bitDepth, uint32_t exposureTime,
		uint8_t* support, uint32_t* validExposureTime, uint32_t* MinimumPreDarkTime, uint32_t* minimumPostDarkTime);

	/// <summary>
	/// 生成PatternData数据文件
	/// </summary>
	/// <param name="DMD">4710或3010，决定了内部图案的分辨率</param>
	/// <param name="PatternSetCount">图案集总数量</param>
	/// <param name="PatternSetArray">图案集列表</param>
	/// <param name="PatternOrderTableCount">默认时序表时序项数量</param>
	/// <param name="PatternOrderTable">默认时序表项</param>
	/// <param name="EastWestFlip">沿短边轴翻转</param>
	/// <param name="LongAxisFlip">沿长边轴翻转</param>
	/// <param name="FilePath">生成的pattern data文件名</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIGeneratePatternData(
		DMD_e                     DMD,
		uint32_t                                   PatternSetCount,
		PAT_PatternSet_s* PatternSetArray,
		uint32_t                                   PatternOrderTableCount,
		PAT_PatternOrderTableEntry_s* PatternOrderTable,
		bool                                       EastWestFlip,
		bool                                       LongAxisFlip,
		const char* FilePath);


	/// <summary>
	/// 创建内部1D灰度图案和默认时序表，支持128灰度补偿
	/// </summary>
	/// <param name="DMD">4710或3010，决定了内部图案的分辨率</param>
	/// <param name="PatternSetCount">图案集总数量</param>
	/// <param name="PatternSetArray">图案集列表</param>
	/// <param name="PatternOrderTableCount">默认时序表时序项数量</param>
	/// <param name="PatternOrderTable">默认时序表项</param>
	/// <param name="EastWestFlip">沿短边轴翻转</param>
	/// <param name="LongAxisFlip">沿长边轴翻转</param>
	/// <param name="gray128pad">默认0，否则，当灰度大于等于128时，增加gray128pad</param>
	/// <param name="FilePath">生成的pattern data文件名</param>
	/// <returns></returns>
	SWL_API bool SWL_SDIGeneratePatternDataX(
		DMD_e                     DMD,
		uint32_t                                   PatternSetCount,
		PAT_PatternSet_s* PatternSetArray,
		uint32_t                                   PatternOrderTableCount,
		PAT_PatternOrderTableEntry_s* PatternOrderTable,
		bool                                       EastWestFlip,
		bool                                       LongAxisFlip,
		uint8_t	gray128pad,
		const char* FilePath);


	/// <summary>
	/// 刷写MCU固件，机器内置的版本>=2.0.0。才支持
	/// </summary>
	/// <param name="hexfile">app.hex升级包路径</param>
	/// <param name="portName">SN号或者串口号，如:4701H00000000001,COM8,/dev/ttyUSB0 </param>
	/// <param name="baudrate">波特率,115200</param>
	/// <param name="Callback">进度通知回调函数</param>
	/// <param name="cancel">取消控制标志，可以设置为NULL</param>
	/// <returns></returns>
	SWL_API void SWL_WriteMCU(char* hexfile, char* portName, uint32_t baudrate, ProgramCallback Callback, bool* cancel);

	/// <summary>
	/// 计数开始
	/// </summary>
	/// <returns></returns>
	SWL_API uint64_t SWL_CounterStart();

	/// <summary>
	/// 计算SWL_CounterStart()之后的耗时（秒）
	/// </summary>
	/// <param name="start">相对起始刻度</param>
	/// <returns></returns>
	SWL_API double SWL_CounterSecond(uint64_t start);



	/// <summary>
	/// 创建固件功能，支持跨平台
	/// </summary>
	/// <param name="dmd">DMD类型，4710,3010,2010</param>
	/// <param name="srcImg">源固件，如果为空则尝试使用当前设备内部的固件</param>
	/// <param name="destImg">目标固件地址，不能为空</param>
	/// <param name="batchFile">脚本路径，NULL则跳过更新</param>
	/// <param name="patternFile">PATTERN文件路径，NULL则跳过</param>
	/// <param name="splashFile">SPLASH图案路径列表，回车换行隔开多个图像，NULL则跳过</param>
	/// <returns></returns>
	SWL_API int SWL_CreateFirmware(DMD_e dmd, const char* srcImg, const char* destImg, const char* batchFile, const char* patternFile,
		const char* splashFiles);

	/// <summary>
	/// 创建固件功能，可修改子版本号
	/// </summary>
	/// <param name="dmd">DMD类型，4710,3010,2010</param>
	/// <param name="srcImg">源固件，不能为空</param>
	/// <param name="destImg">目标固件地址，不能为空</param>
	/// <param name="subVersion">目标固件子版本号，TI官方1-2位数，SWL官方5位数，其他客户自定义版本使用3位数</param>
	/// <param name="batchFile">脚本路径，NULL则跳过更新</param>
	/// <param name="patternFile">PATTERN文件路径，NULL则跳过</param>
	/// <param name="splashFile">SPLASH图案路径列表，回车换行隔开多个图像，NULL则跳过</param>
	/// <returns></returns>
	SWL_API int SWL_CreateFirmwareX(DMD_e dmd, const char* srcImg, const char* destImg, uint16_t subVersion, const char* batchFile, 
		const char* patternFile,const char* splashFiles);

#ifdef __cplusplus
}
#endif

#endif