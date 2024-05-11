#ifndef AS_MAINWINDOW_H
#define AS_MAINWINDOW_H

#include "HomePageDialog.h"
#include "TakeLeaveDialog.h"
#include "SubmitDialog.h"
#include "ApproveLeaveDialog.h"

//QT
#include <QLabel>
#include <QMutex>
#include <QMainWindow>
#include <QStackedWidget>
#include <QStringList>
#include <QLineEdit>
#include <QMenuBar>

#include "qtcpsocket.h"
#include "src/module/common/CustomDataStreamOperators.h"
#include <QNetworkInterface>
#include <QLineEdit>
#include "qtcpsocket.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostInfo>

namespace Ui {
    class MainWindow;
}

// Qt
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

    void iniUI();
    void iniConnect();

    void SetSocket(QTcpSocket* socket);

    void HandinMess(string mess);

    void SetUserNa(string mess);

    void SetAuthority(string mess, UserType auth);

    void ShowUserNa();

    //void handleSendOutData(const sendStruct& data);
    void handleSendOutData(UserLeaveMessageData& data, string des);

    void handleSendOutData();

    void handleGetRecieveData();

    void DataToMess(UserLeaveMessageData data, UserLeaveMessage& mess);

    void MessToData(UserLeaveMessage data, UserLeaveMessageData& mess);

    void queryLocationOfIP(const QString& strIp, const QString& strAk);
    void querySpecialLocation(const QString& strAk);
    QVariantMap parseLocationData(const QByteArray& data);
    void showLocation(QVariantMap varMap);
    void getOnlineIPs();
    void GetOutNetIp();

    void SelfPosition();
    
    Ui::MainWindow* m_ui;

private slots:
    void replyFinished();
private:
    HomePageDialog* m_cHomePageDialog;
    TakeLeaveDialog* m_cTakeLeaveDialog;
    ApproveLeaveDialog* m_cApproveLeaveDialog;
    SubmitDialog* m_cSubmitDialog;


    DialogType m_CurDialogType;
    QMenuBar* menuBar;
    QMenu* fileMenu;

    QTcpSocket* m_socket;

    string m_curusername;
    UserType m_curauthority;

    bool m_isGetPartData;
    int m_requestDataSize;

    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply_1;

    string m_ipAddress;
    QString m_ipnet = "";

    QString m_lat;
    QString m_lon;
    
    QString m_qrOutAdress;
};

#endif