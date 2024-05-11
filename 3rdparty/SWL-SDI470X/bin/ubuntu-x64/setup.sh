#!/bin/bash
# swl setup

#sudo sh ./setup.sh

echo $PWD


clean(){
        echo clean

        if [  -e "/usr/lib/libcyusbserial.so"  ]; then
                rm /usr/lib/libcyusbserial.so
        fi

        if [  -e "/usr/lib/libdlpc-swl-api.so"  ]; then
                rm /usr/lib/libdlpc-swl-api.so
        fi

        if [  -e "/usr/bin/dlpc-swl-shell"  ]; then
                rm /usr/bin/dlpc-swl-shell
        fi

        if [  -e "/usr/bin/dss"  ]; then
                rm /usr/bin/dss
        fi

        if [  -f "/etc/udev/rules.d/swl-usb.rules"  ]; then
                rm -f /etc/udev/rules.d/swl-usb.rules
        fi


        echo clean success
}

clean

if [ "$1" = "clean" ]; then
        exit
fi

echo 安装命令，安装后请使用“dlpc-swl-shell”或者“dss”快捷方式执行光控制命令
sleep 2

#install
apt install usb-1.0

chmod 777 $PWD/dlpc-swl-shell.out

ln -s $PWD/libcyusbserial.so /usr/lib/libcyusbserial.so
ln -s $PWD/libdlpc-swl-api.so  /usr/lib/libdlpc-swl-api.so
ln -s $PWD/dlpc-swl-shell.out  /usr/bin/dlpc-swl-shell
ln -s $PWD/dlpc-swl-shell.out  /usr/bin/dss

#usb rule
echo 'SUBSYSTEMS=="usb",ATTRS{idVendor}=="04b4",ATTRS{idProduct}=="000a",MODE:="0777",OWNER:="root",GROUP:="dialout"'>/etc/udev/rules.d/swl-usb.rules
udevadm trigger


#test
dlpc-swl-shell runonce
sleep 2
dlpc-swl-shell test 1
sleep 2
dlpc-swl-shell splash 1


