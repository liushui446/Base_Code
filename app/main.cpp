#include <iostream>
#include <memory>
#include <iostream>

#include "as/core.hpp"
#include "as/data.hpp"
#include "as/database.hpp"
#include "as/gui.hpp"
#include "as/imgproc.hpp"
#include "as/io.hpp"
#include "as/process.hpp"

#include "as/Common/DB_MGR.hpp"
#include "as/ASContext.hpp"
#include <as/DBUtils.hpp>

void MainTest(int argc, char* argv[])
{
    as::GuiTest(argc, argv);
}

bool InitDB()
{
    //获取数据库路径
    as::DataBasePath::GetDataBasePath();
    as::DB_MGR::GetInstance().GetUserData();

    return false;
}

int main(int argc, char* argv[])
{
    if (InitDB())
    {
        return 0;
    }

    //MainTest(argc, argv);

    as::RunGUI(argc, argv);
    return 0;
}