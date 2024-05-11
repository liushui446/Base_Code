#include "src/utils/include/AutoLoadQss.h"
// Qt
#include <QFile>
#include <QTime>
#include <QDebug>
#include <QWidget>

namespace as 
{

    AutoLoadQss::AutoLoadQss(QObject *parent): QObject(parent), m_app(nullptr)
    {
    }

    AutoLoadQss::~AutoLoadQss()
    {
    }

    void AutoLoadQss::bindAppAndQss(QWidget *_app, QString _qss)
    {
        //
        m_app = _app;
        //
        QFile file(_qss);
        if (file.open(QFile::ReadOnly))
        {
            //
            file.close();
            //
            loadQss(_qss);
            //
        }
        else
        {
            qWarning("Open styleSheet failed");
        }
    }

    void AutoLoadQss::loadQss(const QString &_qss)
    {
        QFile styleFile(_qss);
        if (styleFile.open(QIODevice::ReadOnly))
        {
            QString styleSheet(styleFile.readAll());;
            m_app->setStyleSheet(styleSheet);
            styleFile.close();
        }
    }

    void AutoLoadQss::loadOneTimeQss(QWidget *_app, QString _qss)
    {
        m_app = _app;
        loadQss(_qss);
    }
} // sm
