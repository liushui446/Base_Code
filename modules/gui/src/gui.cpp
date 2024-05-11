#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <corecrt_io.h>
#include <filesystem>
#include <codecvt>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <comutil.h> 

#include <QApplication>
#include "mainwindow.h"
#include <QtWidgets>

#include "LoadDialog.h"
#include "src/module/server/TcpServerDialog.h"

#include "as/gui.hpp"
#include "as/io.hpp"

namespace as
{
    int GuiTest(int argc, char* argv[])
    {
        std::cout << "Module 'Gui' is Ready" << std::endl;
        return 0;
    }

    int RunGUI(int argc, char* argv[])
    {
#if 1
        QApplication app(argc, argv);

        TcpServerDialog serverdialog;
        serverdialog.setWindowTitle(QString::fromLocal8Bit("Server"));
        serverdialog.show();
        /*MainWindow mainwindow;
        LoadDialog appTest;
        appTest.setWindowTitle(QString::fromLocal8Bit("Take Leave System"));
        if (appTest.exec() == QDialog::Accepted)
        {
            mainwindow.SetAuthority(appTest.GetUsername(), appTest.GetUserType());
            mainwindow.SetSocket(appTest.GetSocket());
            mainwindow.ShowUserNa();
            mainwindow.show();
        }*/ 

        return app.exec();
#else
        return 0;
#endif
    }
} // namespace as