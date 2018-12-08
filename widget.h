#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QThread>
#include <QPoint>
#include <QTimer>
#include "qt_windows.h"

class QNetworkReply;
class MonitorUserLoginThread;
class QToolButton;
namespace Ui {
class Widget;
}

class QProcess;
class MonitorUserLoginThread;
class MonitorUserLogoutThread;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void setToken(QString token);
    void setPhone(QString phone);
protected:
//    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


signals:
    void getWechatNo(QByteArray);
private slots:
    void on_pushButton_add_clicked();
    //保存头像
    bool saveHeadImages(QNetworkReply *);

    void on_pushButton_min_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_close_clicked();

    void embed(HWND);

    void unembed(QWindow*);

    void btn_toggled(bool);
    //设置用户信息
    void showUserInfo(QNetworkReply* reply);
    //设置基本信息
    void showBasicInfo(QNetworkReply* reply);
    //设置标签信息
    void showLabelInfo(QNetworkReply* reply);

private:
    //关闭互斥锁
    bool closeMutex();
    //请求头像
    void getHeadImages();
    //动态添加用户头像
    void addHeadImages();
    //从原始微信配置文件读取必要的数据
    bool readWechatConf();
    //获取满足条件的所有窗口句柄
    bool insertWindow(QWindow* win);
    //展示头像
    void showHeadImages();
    //获取用户信息
    void getUserInfo();
    //获取基本信息
    void getBasicInfo();
    //获取标签
    void getLabelInfo();
    //设置标签
    void setLabelInfo();
    //获取快捷回复
    void getQuickReply();
    //设置快捷回复
    void setQuickReply();

private:
    Ui::Widget *ui;
    QList<HWND> m_listWechat;
    QByteArray m_baHeadImageUrl;
    QByteArray m_baWeChatNo;                  //微信号

    QList<bool> m_listPos;
    int         m_iCurIndex;

    //当前索引
    QMap<int, QWidget*> m_mapPos2Widget;      //位置-控件容器
    QMap<int, QWindow*> m_mapPos2Win;
    QMap<QWindow*, int> m_mapWin2Pos;
    QMap<int, QToolButton*> m_mapPos2Btn;     //位置对应按钮
    QMap<QToolButton*, int> m_mapBtn2Pos;
    QMap<int, QString>  m_mapPos2Token;
    QMap<QWindow*,HWND> m_mapHwnd_win;

    //token
    QString m_token;
    QString m_phone;

    QList<QProcess*> m_listProc;

    //控制窗口移动
    QPoint m_poWindowPos;
    QPoint m_poMousePos;
    QPoint m_poDPos;

    //线程
    MonitorUserLoginThread * m_pThreadLogin;
    MonitorUserLogoutThread* m_pThreadLogout;
};

class MonitorUserLoginThread:public QThread
{
    Q_OBJECT
    void run() override;
signals:
    void windowCreate(HWND);

private:
    static BOOL CALLBACK EnumWindowsProc(HWND   hwnd, LPARAM lParam);
};

class MonitorUserLogoutThread:public QThread
{
    Q_OBJECT
    void run() override;
signals:
    void windowDestroy(QWindow*);
};

#endif // WIDGET_H
