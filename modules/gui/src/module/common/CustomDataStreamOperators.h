#ifndef CUSTOMDATASTREAMOPERATORS_H
#define CUSTOMDATASTREAMOPERATORS_H

#include <QDataStream>
#include <QString>
#include "as/PadCore.hpp"

#include "qtcpsocket.h"
#include <winsock.h>
//
class Net
{
public:
    Net();
    ~Net() {};

    Net& Net::Ins()
    {
        static Net instance;
        return instance;
    }
public:
    //名字转IP(OK,New)
    char* GetIp(char* szhostname)
    {
        char* szip = NULL;
        char* ipbit = NULL;
        hostent* ipstr = NULL;

        szip = new char[30];
        if (!szip)
            goto Err;
        ZeroMemory(szip, 30);
        ipstr = gethostbyname(szhostname);
        if (ipstr == NULL)
            goto Err;
        ipbit = inet_ntoa(*(in_addr*)ipstr->h_addr);
        if (ipbit == NULL)
            goto Err;
        memcpy(szip, ipbit, strlen(ipbit));


        return szip;
    Err:
        return NULL;
    }

    //连接HTTP服务器（OK,Connected）
    SOCKET ConnectHttpServer(char* szip)
{
    SOCKET szclient_socket = 0;

    szclient_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == szclient_socket)
        goto Err;
    //连接服务器
    SOCKADDR_IN info;
    info.sin_port = htons(80);
    info.sin_family = AF_INET;
    info.sin_addr.S_un.S_addr = inet_addr(szip);
    if (0 != connect(szclient_socket, (SOCKADDR*)&info, sizeof(SOCKADDR)))
        goto Err;

    //返回
    return szclient_socket;
Err:
    if (szclient_socket != NULL && szclient_socket != INVALID_SOCKET)
    {
        closesocket(szclient_socket);
        szclient_socket = 0;
    }
    return 0;
}

    //设置套接字缓冲区大小（OK）
    bool SetSocketInfo(SOCKET szsocket, int uBufferSize)
    {
        if (SOCKET_ERROR == setsockopt(szsocket, SOL_SOCKET, SO_RCVBUF, (char*)&uBufferSize, sizeof(int)))
            goto Err;
        if (SOCKET_ERROR == setsockopt(szsocket, SOL_SOCKET, SO_SNDBUF, (char*)&uBufferSize, sizeof(int)))
            goto Err;
        return TRUE;
    Err:
        return FALSE;
    }


    //UTF8转ANSI字符串(OK,New)
    char* Utf8ToAnsi(char* utf8, int len, char szcc)
    {
        BOOL szok = FALSE;
        int szlen = 0;
        char* szretstr = NULL;
        wchar_t* whar_str = NULL;
        //UTF8转UNICODE
        szlen = MultiByteToWideChar(CP_UTF8, 0, utf8, len, NULL, 0);
        if (szlen <= 0)
            goto Err;
        szlen += 1;
        whar_str = new wchar_t[szlen];
        if (!whar_str)
            goto Err;
        ZeroMemory(whar_str, szlen * sizeof(wchar_t));
        MultiByteToWideChar(CP_UTF8, 0, utf8, len, whar_str, szlen);
        if (GetLastError() != 0)
            goto Err;
        //UNICODE转ANSI
        szlen = WideCharToMultiByte(CP_ACP, 0, whar_str, len, NULL, 0, &szcc, &szok);
        if (szlen <= 0)
            goto Err;
        szlen += 1;
        szretstr = new char[szlen];
        if (!szretstr)
            goto Err;
        ZeroMemory(szretstr, szlen * sizeof(char));
        WideCharToMultiByte(CP_ACP, 0, whar_str, len, szretstr, szlen, &szcc, &szok);
        if (GetLastError() != 0)
            goto Err;
        if (whar_str)
        {
            delete[] whar_str;
            whar_str = NULL;
        }
        return szretstr;
    Err:
        if (whar_str)
        {
            delete[]whar_str;
            whar_str = NULL;
        }
        return NULL;
    }

    char* StrStrA(char* str1, char* str2)
    {
        if (str1 == "" || str2 == "") 
            return nullptr;
        int size = strlen(str1);

        for (int i = 0; i < size; i++, str1++)
        {
            char* p = str1;

            for (char* q = str2; ; q++, p++)
            {

                if (*q == '\0') return str1;
                if (*q != *p) break;
            }
        }
        return nullptr;
    }

    //获取物理地址(OK,ip.chinaz.com,New)
    char* GetPoint(char* ip_address, char* get_ip_de_host_name)
    {
        int szlen = 0;
        char* szPoint = NULL;
        char* szTemp = NULL;
        char* szTemp2 = NULL;
        char* szTemp3 = NULL;
        char* get_host_name_ip = NULL;
        SOCKET http_client_socket = NULL;
        char packet[400] = "GET /";
        strcat(packet, "?IP=");
        strcat(packet, ip_address);
        strcat(packet, " HTTP/1.1\r\n" \
            "Host: "
        );
        strcat(packet, get_ip_de_host_name);
        strcat(packet, "\r\n"
            "Connection: keep-alive\r\n"
            "Cache-Control: max-age=0\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
            "Accept-Language: zh-CN,zh;q=0.8\r\n\r\n"
        );


        char* recv_buffer = NULL;
        //代码开始
        recv_buffer = new char[50000];
        if (!recv_buffer)
            goto Err;
        ZeroMemory(recv_buffer, 50000);
        //返回解析物理地址的主机的IP地址
        get_host_name_ip = GetIp(get_ip_de_host_name);
        if (get_host_name_ip == NULL)
            goto Err;

        //连接服务器
        http_client_socket = ConnectHttpServer(get_host_name_ip);
        if (http_client_socket == 0)
            goto Err;

        //设置缓冲区大小
        if (!SetSocketInfo(http_client_socket, 30000))
            goto Err;
        if (send(http_client_socket, packet, strlen(packet), 0) <= 0)
            goto Err;
        if (recv(http_client_socket, recv_buffer, 50000, 0) <= 0)
            goto Err;
        szTemp3 = Utf8ToAnsi(recv_buffer, -1, '.');
        if (szTemp3 == NULL)
            goto Err;
        szTemp = StrStrA(szTemp3, "IP的物理位置");
        if (szTemp == NULL)
            goto Err;
        szTemp = StrStrA(szTemp, "w50");
        if (szTemp == NULL)
            goto Err;
        szTemp += strlen("w50-0\">"); //???
        if (szTemp == NULL)
            goto Err;
        szTemp2 = strchr(szTemp, '<');
        if (szTemp2 == NULL)
            goto Err;
        szlen = szTemp2 - szTemp;
        if (szlen <= 0)
            goto Err;
        szlen += 1;
        szPoint = new char[szlen];
        if (!szPoint)
            goto Err;
        ZeroMemory(szPoint, szlen);
        memcpy(szPoint, szTemp, szlen - 1);

        if (get_host_name_ip)
        {
            delete[]get_host_name_ip;
            get_host_name_ip = NULL;
        }
        if (http_client_socket)
        {
            closesocket(http_client_socket);
            http_client_socket = 0;
        }
        if (recv_buffer)
        {
            delete[]recv_buffer;
            recv_buffer = NULL;
        }
        if (szTemp3)
        {
            delete[]szTemp3;
            szTemp3 = NULL;
        }
        return szPoint;
    Err:
        if (get_host_name_ip)
        {
            delete[]get_host_name_ip;
            get_host_name_ip = NULL;
        }
        if (http_client_socket)
        {
            closesocket(http_client_socket);
            http_client_socket = 0;
        }
        if (recv_buffer)
        {
            delete[]recv_buffer;
            recv_buffer = NULL;
        }
        if (szTemp3)
        {
            delete[]szTemp3;
            szTemp3 = NULL;
        }
        return NULL;
    }


};

#endif