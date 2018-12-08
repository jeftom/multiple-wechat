#include <QApplication>
#include <QMessageBox>
#include "login.h"
#include "config.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    char szReadConfMsg[255] = {0};
    int iMsgLength = sizeof(szReadConfMsg);
    int iReadConfRes = config::getSingleton()->load_config_file(szReadConfMsg, iMsgLength);
    if(-1 == iReadConfRes)
    {
        QMessageBox::critical(nullptr, QObject::tr("error"), QObject::tr("config file not exist!"), QMessageBox::Ok);
        return -1;
    }
    else if(-2 == iReadConfRes)
    {
        QMessageBox::critical(nullptr, QObject::tr("error"), QObject::tr("config file not exist!"), QMessageBox::Ok);
        return -1;
    }

    HANDLE mt = CreateMutex(0, true, L"wechat_my_self");
    if(mt == NULL)
        return -1;

#ifdef QT_DEBUG
    Widget w;
    w.show();
    return a.exec();
#else
    login l;
    if(QDialog::Accepted == l.exec())
    {
        Widget w;
        w.setToken(l.getToken());
        w.setPhone(l.getPhone());
        w.show();
        return a.exec();
    }
#endif
}
