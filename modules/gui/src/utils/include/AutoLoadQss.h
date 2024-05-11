#ifndef AUTOLOADQSS_H
#define AUTOLOADQSS_H

#include <QObject>

// Qt
class QWidget;

namespace as
{

class  AutoLoadQss : public QObject
{
    Q_OBJECT
public:
    AutoLoadQss(QObject *parent = nullptr);
    ~AutoLoadQss();
    // 绑定控件和样式表文件
    void bindAppAndQss(QWidget *, QString);
    // 控件加载一次样式表文件
    void loadOneTimeQss(QWidget *, QString);
protected slots:
    void loadQss(const QString &);
protected:
    QWidget *m_app;
};

} //as

#endif