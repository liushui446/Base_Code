@echo off
set avg=dlpc-swl-shell

if "%1"=="" (
echo 光机列表:
dlpc-swl-shell devices 

echo 操作指定关机命令用法：
echo 是将dlpc-swl-shell的命令参数前加上光机序号或SN号。并使用SDI命令
echo sdi 1 runonce
echo sdi 0 runonce

ping 11.1.1.1 -n 1 -w 1000 > nul
goto :end
)else (
echo 切换光机:%1
set SWL_USB_IDX=%1

goto args
)


:args
set x=%2
if  not "%x%"==""  (
set avg=%avg% %x%
rem echo %avg%
shift
goto args
) 
echo %avg%
%avg%


:end

