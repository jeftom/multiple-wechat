#include "widget.h"
#include "ui_widget.h"
#include "config.h"
#include <QWindow>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QToolButton>
#include <QDebug>
#include "userpreference.h"
#include "globalconf.h"
#include "bindphone.h"
#include "thirdparty/rapidjson/document.h"
#include "widgetconf.h"

#define LSIZE 65
#define WSIZE 100

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //（1）配置监控线程
    m_pThreadLogin  = new MonitorUserLoginThread;
    qRegisterMetaType<HWND>("HWND");
    connect(m_pThreadLogin, SIGNAL(windowCreate(HWND)), this, SLOT(embed(HWND)));
    m_pThreadLogout = new MonitorUserLogoutThread;
    connect(m_pThreadLogout, SIGNAL(windowDestroy(QWindow*)), this, SLOT(unembed(QWindow*)));

    //30个位置都为空
    m_iCurIndex = 0;
    QVBoxLayout *v_layout = new QVBoxLayout;
    for(int i = 0; i <30; i++)
    {
        m_listPos.append(false);
        QWidget* widget = new QWidget(this);
        widget->setGeometry(107, 75, 715, 505);
        m_mapPos2Widget.insert(i, widget);
        QHBoxLayout *h_layout = new QHBoxLayout();
        widget->setLayout(h_layout);
        ui->stackedWidget->insertWidget(i, widget);

        QToolButton *pBtn = new QToolButton(ui->scrollAreaWidgetContents);
        pBtn->setMinimumSize(LSIZE, LSIZE);
        pBtn->setCheckable(true);
        pBtn->setAutoExclusive(true);
        pBtn->setGeometry(0, i * LSIZE + i * 1, LSIZE, LSIZE);
        pBtn->setVisible(false);
        pBtn->setStyleSheet("border:none");
        connect(pBtn, SIGNAL(toggled(bool)), this, SLOT(btn_toggled(bool)));

        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->addWidget(pBtn);
        hlayout->setMargin(3);

        m_mapPos2Btn.insert(i, pBtn);
        m_mapBtn2Pos.insert(pBtn, i);

        v_layout->addLayout(hlayout);
        v_layout->setMargin(0);
    }

    v_layout->addStretch();
    v_layout->setSpacing(0);
    //初始化主窗体的样式
    setWindowFlags(Qt::FramelessWindowHint|Qt::Widget);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->widget()->resize(91, LSIZE*30-1);
    ui->scrollAreaWidgetContents->setLayout(v_layout);

//    ui->label_4->setOpenExternalLinks(true);
    //（2）启动线程
    m_pThreadLogin->start();
    m_pThreadLogout->start();
#ifdef QT_DEBUG
    m_token = "a1f90201890fd02095a0fdb532fbea40";
#endif
}

//析构
Widget::~Widget()
{
    g_threadLogin = false;
    g_threadLogout = false;
    m_pThreadLogin->quit();
    m_pThreadLogout->quit();
    m_pThreadLogin->wait(1);
    m_pThreadLogout->wait(1);

    //释放窗口
    for(auto& w: m_listProc)
    {
        w->close();
    }
    QThread::msleep(500);
    //将该目录下得头像全部删除
    QDir dir("C:/Users/Administrator/AppData/Roaming/yunsenke/headimages");
    QFileInfoList list = dir.entryInfoList();
    for(auto l:list)
    {
        l.dir().remove(l.fileName());
    }
    delete ui;
}

void Widget::setToken(QString token)
{
    m_token = token;
    getBasicInfo();
}

void Widget::setPhone(QString phone)
{
    m_phone = phone;
}

void Widget::showHeadImages()
{
    //1. 展示头像
    QString strFileName = QDir::homePath()+"/AppData/Roaming/yunsenke/headimages/"+m_baWeChatNo+".jpg";
    QPixmap *pixmap = new QPixmap(strFileName);
    *pixmap = pixmap->scaled(QSize(WSIZE,WSIZE));
    m_mapPos2Btn.value(m_iCurIndex)->setVisible(true);
    m_mapPos2Btn.value(m_iCurIndex)->setIcon(*pixmap);
    m_mapPos2Btn.value(m_iCurIndex)->setIconSize(QSize(LSIZE,LSIZE)); //设置填充按键这么大的
    m_mapPos2Btn.value(m_iCurIndex)->setStyleSheet("background-color: rgb(222, 222, 222,0);");  //设置透明的
}

void Widget::getUserInfo()
{
//    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
//    QHttpPart wxidPart;
//    wxidPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
//    wxidPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"wxid\""));
//    wxidPart.setBody();

//    const char* szurl = config::getSingleton()->getValue((char*)"getUserMsg");
//    if(nullptr == szurl)
//    {
//        QMessageBox::critical(this, tr("error"), tr("getUserMsg url is not allowed empty"));
//        return;
//    }
//    QUrl url(szurl);
//    QNetworkRequest request(url);

//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//    connect(manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(slot_login(QNetworkReply*)));
//    multiPart->append(phonePart);
//    multiPart->append(codePart);

//    QNetworkReply *reply = manager->post(request, multiPart);
//    multiPart->setParent(reply);
}

void Widget::getBasicInfo()
{
    const char* szurl = config::getSingleton()->getValue((char*)"basicInfo");
    if(nullptr == szurl)
    {
        QMessageBox::critical(this, tr("error"), tr("getbasicinfo url is not allowed empty"));
        return;
    }
    QUrl url(szurl);
    QNetworkRequest request(url);
    request.setRawHeader("headerToken", m_token.toLatin1());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(showBasicInfo(QNetworkReply*)));

    manager->get(request);
}

void Widget::getLabelInfo()
{

}

void Widget::setLabelInfo()
{

}

void Widget::getQuickReply()
{

}

void Widget::setQuickReply()
{

}
//获取应用程序句柄， 实际是回调函数来获取
bool Widget::insertWindow(QWindow* win)
{
    //2. 窗口嵌入
    m_mapPos2Widget.value(m_iCurIndex)->setVisible(true);
    QWidget * wechatWidget = QWidget::createWindowContainer(win, m_mapPos2Widget.value(m_iCurIndex));
    wechatWidget->setGeometry(m_mapPos2Widget.value(m_iCurIndex)->geometry());
    m_mapPos2Widget.value(m_iCurIndex)->layout()->addWidget(wechatWidget);
    QHBoxLayout* hBox = new QHBoxLayout;
    wechatWidget->setLayout(hBox);
    ui->stackedWidget->setCurrentIndex(m_iCurIndex);

    m_mapPos2Win.insert(m_iCurIndex, win);
    m_mapWin2Pos.insert(win, m_iCurIndex);
    m_mapPos2Widget.insert(m_iCurIndex, wechatWidget);

    return true;
}


//此种策略暂时不用，放在此处以备将来优化用
//bool Widget::eventFilter(QObject *watched, QEvent *event)
//{
//    if(m_mapPos2Win.value(0) == (QWindow*)watched)
//    {
//        if (event->type() == QEvent::Enter){
//            qDebug() << event;
//            return true;
//        } else {
//            return false;
//        }
//    } else {
//        return QWidget::eventFilter(watched, event);
//    }
//}

//关闭微信单例句柄
bool Widget::closeMutex()
{
    HANDLE mt = OpenMutex(MUTEX_ALL_ACCESS, true, L"_WeChat_App_Instance_Identity_Mutex_Name");
    HANDLE mtconf = OpenMutex(MUTEX_ALL_ACCESS, true, L"WeChat_GlobalConfig_Multi_Process_Mutex");
    if(nullptr != mt || nullptr != mtconf) //此时标识有微信登录
    {
        CloseHandle(mt);
        CloseHandle(mtconf);
        mtconf = nullptr;
        mt = nullptr;
        return true;
    }
    else  //此时表示无微信登录
    {
        return false;
    }

}

//获取微信头像
void Widget::getHeadImages()
{
    QNetworkAccessManager *workmanager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(m_baHeadImageUrl));
    connect(workmanager, SIGNAL(finished(QNetworkReply *)), this, SLOT(saveHeadImages(QNetworkReply *)));
    workmanager->get(request);
}

//从微信的配置文件读取所需数据
bool Widget::readWechatConf()
{
    //读取all users配置文件
    QFile allConf(QDir::homePath()+"/Documents/WeChat Files/All Users/config/config.data");
    allConf.open(QIODevice::ReadOnly);
    QByteArray baAllConf;
    baAllConf = allConf.readAll();
    baAllConf =  baAllConf.mid(baAllConf.indexOf("C:\\Users"), -1);
    QList<QByteArray> list_ba = baAllConf.split('\\');
    allConf.close();

    //根据all users配置读取当前登陆用户的配置，来获取头像的url
    QFile accountConf(QDir::homePath()+"/Documents/WeChat Files/"+list_ba.at(5)+"/config/AccInfo.dat");
    accountConf.open(QIODevice::ReadOnly);
    QByteArray baAccountConf;
    baAccountConf = accountConf.readAll();
    QByteArray baHeadImage;//headimage url
    while(0 != baAccountConf.length())
    {
        QByteArray ba = baAccountConf.mid(baAccountConf.indexOf("http"), -1);
        QByteArray ba2 = baAccountConf.mid(baAccountConf.indexOf("http")+4, -1);
        int iDst = ba2.indexOf("http");
        if(iDst == -1 || ba.indexOf("/132") < iDst+4)
        {
            baHeadImage = ba.mid(0, ba.indexOf("/132")+4);
            break;
        }
        int tmp = ba.indexOf("http");
        if(tmp == -1)
            break;
        baAccountConf = ba.mid(tmp+4, -1);
    }
    accountConf.close();

    m_baWeChatNo     = list_ba.at(5);
    m_baHeadImageUrl = baHeadImage;

    return true;
}

void Widget::on_pushButton_add_clicked()
{
    //将应用程序单例锁关闭
    closeMutex();
    QThread::sleep(1);
    
    QProcess *p = new QProcess(this);
    //wechatInstallPath
    QString path = config::getSingleton()->getValue("wechatInstallPath");
    if(path.length() == 0)
    {
        QMessageBox::critical(this, "error", "wechat path is empty.", QMessageBox::Ok);
        //此时需要让用户配置一下
        userpreference dlg(this);
        if(QDialog::Rejected == dlg.exec())
            return;
        path = config::getSingleton()->getValue("wechatInstallPath");
    }
    if(!QFileInfo::exists(path))
    {
        QMessageBox::critical(this, "error", "config's wechat path error or wechat application not exist already.", QMessageBox::Ok);
        return;
    }

    p->start(config::getSingleton()->getValue("wechatInstallPath"));
    m_listProc.append(p);
}

bool Widget::saveHeadImages(QNetworkReply *reply)
{
    QDir headImageDir(QDir::homePath()+"/AppData/Roaming/yunsenke/headimages");
    if(!headImageDir.exists())
    {
        if(!headImageDir.mkpath(QDir::homePath()+"/AppData/Roaming/yunsenke/headimages"))
        {
            QMessageBox::critical(this, "error", "headimages path create failure, please use administrator permissions and try again.");
            return false;
        }
    }

    QString headImagePath = QDir::homePath()+"/AppData/Roaming/yunsenke/headimages/"+m_baWeChatNo+".jpg";
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        // 缓存到本地
        QFile file(headImagePath);

        if (file.open(QIODevice::Append))
        {
            file.write(bytes);
            file.close();
            showHeadImages();
        }
        else
        {
            // 错误处理-显示错误信息，或显示上一次缓存的图片或叉图。
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


//最小化
void Widget::on_pushButton_min_clicked()
{
    this->showMinimized();
}

//最大化
void Widget::on_pushButton_max_clicked()
{
    if (this->isMaximized())
    {
        this->showNormal();
        ui->pushButton_max->setStyleSheet("border-image:url(:/mainwnd/max.png)");
    }
    else
    {
        this->showMaximized();
        ui->pushButton_max->setStyleSheet("border-image:url(:/mainwnd/recover.png)");
    }
}

//关闭
void Widget::on_pushButton_close_clicked()
{
    this->close();  //程序退出函数，调用closeEvent
}

void Widget::embed(HWND win)
{
    int i = 0;
    for(; i<m_listPos.length(); i++)
    {
        if(m_listPos[i] == false)
        {
            m_iCurIndex = i;
            m_listPos[m_iCurIndex] = true;
            break;
        }
    }
    if(i >= 30)
    {
        QMessageBox::critical(this, "error", "the number of login devices has reached the limit.", QMessageBox::Ok);
        return;
    }
    //0. 读取配置
    readWechatConf();
    //1. 获取头像
    getHeadImages();
    //2. 获取窗口并嵌入
    QWindow * wechatWindow = QWindow::fromWinId(WId(win));
    m_mapHwnd_win.insert(wechatWindow, win);
    insertWindow(wechatWindow);
    g_listWindow.append(wechatWindow);
    //3. 绑定手机
//    bindphone bp;
//    bp.setWechatNo(m_baWeChatNo);
//    bp.setToken(m_token);
//    bp.exec();
}

void Widget::unembed(QWindow *win)
{
    m_mapPos2Btn.value(m_mapWin2Pos.value(win))->setVisible(false);
    m_listPos[m_mapWin2Pos.value(win)] = false;
    g_listWindowHandle.removeOne(m_mapHwnd_win.value(win));
}

void Widget::btn_toggled(bool bChecked)
{
    QToolButton* obj = (QToolButton*)QObject::sender();
    if(bChecked)
    {
        obj->setStyleSheet("border-color:rgb(255,0,0);border:5px;");
        ui->stackedWidget->setCurrentIndex(m_mapBtn2Pos.value(obj));
    }
    if(bChecked)
    {
        obj->setStyleSheet("border:none;");
    }
}

void Widget::showUserInfo(QNetworkReply *reply)
{

}

void Widget::showBasicInfo(QNetworkReply *reply)
{
    QString str = reply->readAll();
    reply->deleteLater();
    using namespace rapidjson;
    Document d;
    if(d.Parse(str.toStdString().c_str()).HasParseError())
    {
        QMessageBox::information(this, tr("Info"), tr("server error"), QMessageBox::Ok);
    }
    if(d.HasMember("errCode") && d["errCode"].IsInt())
    {
        if(d["errCode"].GetInt() == 10001)
        {
            if(d.HasMember("data") && d["data"].IsObject())
            {
                const rapidjson::Value& obj = d["data"];
                if(obj.HasMember("devices_num"))
                {
                    const rapidjson::Value& v = obj["devices_num"];
                    ui->label_deviceNum->setText(QString("devices num:%1").arg(v.GetInt()));
                }
                if(obj.HasMember("user_url"))
                {
                    const rapidjson::Value& v = obj["user_url"];
                    ui->label_officeWebSite->setText(QString("official web site:%1").arg(v.GetString()));
                }
                if(obj.HasMember("version"))
                {
                    const rapidjson::Value& v = obj["version"];
                    ui->label_Version->setText(QString("version:%1").arg(v.GetString()));
                }
                ui->label_Account->setText(QString("account:%1").arg(m_phone));
            }
        }
        else
        {
            std::string key = QString("%1").arg(d["errCode"].GetInt()).toStdString();
            QMessageBox::information(this, tr("Info"), widget_status_map[QString::fromStdString(key)], QMessageBox::Ok);
        }
    }
}

void Widget::showLabelInfo(QNetworkReply *reply)
{

}

//void Widget::selectWin()
//{
//    QToolButton* pBtn = (QToolButton*)QObject::sender();
//    m_mapPos2Widget.value(m_mapBtn2Pos.value(pBtn))->setVisible(true);
//    //设置选中
//}

void Widget::closeEvent(QCloseEvent * event)
{
    QMessageBox box(QMessageBox::Question, "quit", "are you sure to quit?", QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == box.exec())
    {
        event->accept();  //在此处释放资源
    }
    else
    {
        event->ignore();
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    m_poWindowPos = this->pos();
    m_poMousePos = event->globalPos();
    m_poDPos = m_poMousePos - m_poWindowPos;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - m_poDPos);
}

//*******************************************
void MonitorUserLoginThread::run()
{
    do
    {
        EnumWindows(EnumWindowsProc, (LPARAM)this);
        QThread::msleep(500);
    }while(g_threadLogin);
}

BOOL MonitorUserLoginThread::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    MonitorUserLoginThread* pThis = (MonitorUserLoginThread*)lParam;

    TCHAR szClassName[MAX_PATH] = {0};
    int iclsLen = ::GetClassName(hwnd, szClassName, MAX_PATH);
    QString strClsName = QString::fromRawData((QChar*)szClassName, iclsLen);


    //此时说明登录用户数已到极限
    if(strClsName.compare("WeChatMainWndForPC") == 0)
    {
        TCHAR szTitle[MAX_PATH] = {0};
        int ititLen = ::GetWindowText(hwnd,szTitle,MAX_PATH);
        QString strTit = QString::fromRawData((QChar*)szTitle, ititLen);
        if(strTit.compare("微信") == 0)
        {
            if(g_listWindowHandle.indexOf(hwnd) == -1)
            {
                g_listWindowHandle.append(hwnd);
                emit pThis->windowCreate(hwnd);
            }
        }
    }
    return TRUE;
}


//**********************************************
void MonitorUserLogoutThread::run()
{
    while(g_threadLogout)
    {
        for(auto& w: g_listWindow)
        {
            if(w == nullptr)
            {
                g_listWindow.removeOne(w);
                emit windowDestroy(w);
            }
        }
        QThread::sleep(1);
    }
}
