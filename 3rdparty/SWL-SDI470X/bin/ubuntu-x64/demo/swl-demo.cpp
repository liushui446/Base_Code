// swl-demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "string.h"
#include "time.h"
using namespace std;

#ifdef WIN32
#include "windows.h"
#else

#include "unistd.h"
#define Sleep(x) usleep((x)*1000)
#endif

#include "../../../dlpc-swl-lib.h"
int main(int argc, char* argv[], char* envp[])
{
    //SWL_Init();
    if (argc > 1 && strcmp(argv[1], "devices") == 0) {//获取设备链接列表
        char devices[10][30] = { 0 };
        uint32_t num = 0;
        SWL_Devices(devices,&num);
        for (uint32_t i = 0; i < num; i++)
        {
            printf("%s\n", devices[i]);
        }
        return 0;
    }
    
    else if (argc > 1 && strcmp(argv[1], "runonce") == 0) {
        SWL_Runonce();
        return 0;
    }
    else if (argc > 1 && strcmp(argv[1], "runcont") == 0) {
        SWL_RunContinuously();
        return 0;
    }
    else if (argc > 1 && strcmp(argv[1], "runstop") == 0) {
        SWL_RunStop();
        return 0;
    }else if (argc > 1 && strcmp(argv[1], "writePattern") == 0) {
        printf("正在刷入Pattern，请不要插拔电源，操作光机，否则可能导致烧坏。\n");
		uint32_t startTime = (uint32_t)(time(0));
        SWL_WritePatternToFlash(argv[2], NULL);
        uint32_t useTime = (uint32_t)(time(0)) - startTime;
        printf("\n刷新结束。总耗时：%d 秒\n", useTime);
        return 0;
       
    }
    else if (argc > 1 && strcmp(argv[1], "patorder") == 0) {


        char devs[10][30] = { 0 };
        uint32_t num = 0;
        SWL_Devices(devs, &num);
        printf("SWL_Devices(devs, &num);//获取USB和串口设备列表,设备数:%d.;\n", num);
        
        //构造一个动态时序表  需要保证引用的patternset存在  且图片数量是一致的
#define PENUM 5
        PatternOrderTableEntry entrys[PENUM];
		
		for(int lo=0;lo<5;lo++){
        for (size_t i = 0; i < PENUM; i++)
        {
            entrys[i].PatternSetIndex = 1;
            entrys[i].IlluminationSelect = 2;
            entrys[i].NumDisplayPatterns = 1;
            entrys[i].PreIlluminationDarkTime = 3000;
            entrys[i].IlluminationTime = 3000+lo;
            entrys[i].PostIlluminationDarkTime = 3000;
            entrys[i].PatternInvert0 = 0;
            entrys[i].PatternInvert1 = 0;
        }

        //初始化循环写到不同的设备，并同步更新，方便后续的trigger in能正常工作
        for (size_t i = 0; i < num; i++)
        {
            SWL_SetUsbIdx(devs[i]);
            SWL_PatternOrderLoadEntries(entrys, PENUM);
            //SWL_SDIOrderUpdate();
            //Sleep(2000);

        }
        for (size_t i = 0; i < num; i++)
        {
            SWL_SetUsbIdx(devs[i]);
            printf("\n%s\n eno\tpat-set\tpatnum\trgbled\tpredark\tillumination\tpostdark\tinvert\n", devs[i]);
            for (int e = 0; e < PENUM; e++)
            {
                PatternOrderTableEntry entry;
                SWL_PatternOrderTableEntryRead(&entry, e);
                printf("%4d\t%4d\t%4d\t%4d\t%7d\t%8d\t%7d\t%9d\n",
                    e, entry.PatternSetIndex, entry.NumDisplayPatterns, entry.IlluminationSelect,
                    entry.PreIlluminationDarkTime, entry.IlluminationTime, entry.PostIlluminationDarkTime,
                    entry.PatternInvert0 ? 1 : 0);
            }
            
            
            //Sleep(2000);

        }
		}

        return 0;

    }
    
   printf("swl-demo runonce\n\n");
   printf("swl-demo runcont\n\n");
   printf("swl-demo runstop\n\n");
   printf("swl-demo writePattern xxxpattern.bin\n\n");



}
