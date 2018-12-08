#include "login.h"
#include "ui_login.h"
#include <QLineEdit>
#include <QPixmap>
#include <QMouseEvent>
#include <QTextCodec>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <utility>
#include "thirdparty/rapidjson/rapidjson.h"
#include "thirdparty/rapidjson/document.h"
#include "status.h"
#include "config.h"
#include <QDebug>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //初始化操作
    init();
    //设置无边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //给手机号和验证码输入框设置图标
    QAction *userAction = new QAction(this);
    userAction->setIcon(QIcon(":/loginwnd/username.png"));
    ui->lineedit_phone_num->addAction(userAction,QLineEdit::LeadingPosition);

    QAction *passAction = new QAction(this);
    passAction->setIcon(QIcon(":/loginwnd/passwd.png"));
    ui->lineedit_check_num->addAction(passAction, QLineEdit::LeadingPosition);
    //设置logo
    QPixmap pixmap(":/loginwnd/logo.png");
    pixmap = pixmap.scaled(QSize(ui->label_logo->width(), ui->label_logo->height()));
    ui->label_logo->setPixmap(pixmap);

    m_timer_showsec = new QTimer(this);
    m_timer_showsec->setTimerType(Qt::PreciseTimer);
    m_timer_showsec->setInterval(1000);
    connect(m_timer_showsec, SIGNAL(timeout()), this, SLOT(slot_showsec()));
}

//初始化操作
bool login::init()
{
    //初始化秒数
    m_isec = 0;
    //初始化
    m_poWindowPos = QPoint(0,0);
    m_poDPos = QPoint(0,0);
    m_poMousePos = QPoint(0,0);
    return true;
}


login::~login()
{
    delete ui;
}

QString login::getToken()
{
    return m_token;
}

QString login::getPhone()
{
    return m_phone;
}

//获取验证码
void login::on_pushButton_getChecknum_clicked()
{
    QRegExp ep("^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\\d{8}$");
    QString strPhone = ui->lineedit_phone_num->text();
    if(!strPhone.isEmpty() && !ep.exactMatch(strPhone))
    {
        QMessageBox::critical(this, tr("error"), tr("phone is not valid."), QMessageBox::Ok);
        return;
    }
    //设置定时器
    ui->pushButton_getChecknum->setEnabled(false);
    m_timer_showsec->start();
    m_isec = 60;
    //提交表单数据
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"phone\""));
    textPart.setBody(ui->lineedit_phone_num->text().toStdString().c_str());

    const char* szurl = config::getSingleton()->getValue((char*)"getAuthorizedCode");
    if(nullptr == szurl)
    {
        QMessageBox::critical(this, tr("error"), tr("getAuthorizedCode url is not allowed empty"));
        reset_timer();
        return;
    }
    QUrl url(szurl);//获取验证码
    QNetworkRequest request(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    multiPart->append(textPart);

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    m_phone = ui->lineedit_phone_num->text();
}

void login::replyFinished(QNetworkReply* reply)
{
    QString str = reply->readAll();
#ifdef QT_DEBUG
    qDebug() << str;
#endif
    reply->deleteLater();
    using namespace rapidjson;
    Document d;
    if(d.Parse(str.toStdString().c_str()).HasParseError())
    {
        QMessageBox::information(this, tr("Info"), tr("server error"), QMessageBox::Ok);
    }
    if(d.HasMember("errCode") && d["errCode"].IsInt() && d["errCode"] != 10001)
    {
        m_timer_showsec->stop();
        ui->pushButton_getChecknum->setEnabled(true);
        std::string key = QString("%1").arg(d["errCode"].GetInt()).toStdString();
        QMessageBox::information(this, "Info", status_map[QString::fromStdString(key)], QMessageBox::Ok);
    }
}

//自定义关闭按钮
void login::on_pushButton_close_clicked()
{
    this->close();
}

void login::reset_timer()
{
    m_timer_showsec->stop();
    ui->pushButton_getChecknum->setEnabled(true);
    ui->pushButton_getChecknum->setText(tr("get check num"));
}

//窗体移动
void login::mousePressEvent(QMouseEvent *event){
    m_poWindowPos = this->pos();
    m_poMousePos = event->globalPos();
    m_poDPos = m_poMousePos - m_poWindowPos;
}
void login::mouseMoveEvent(QMouseEvent *event){
    this->move(event->globalPos() - m_poDPos);
}

//关闭按钮
void login::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("quit"),tr("are you sure to quit?"), QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes) {
        event->accept();  //接受退出信号，程序退出
    }
}

//登录
void login::on_pushButton_login_clicked()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart phonePart;
    phonePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    phonePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"phone\""));
    phonePart.setBody(ui->lineedit_phone_num->text().toStdString().c_str());

    QHttpPart codePart;
    codePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    codePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"verifity\""));
    codePart.setBody(ui->lineedit_check_num->text().toStdString().c_str());


    const char* szurl = config::getSingleton()->getValue((char*)"login");
    if(nullptr == szurl)
    {
        QMessageBox::critical(this, tr("error"), tr("login url is not allowed empty"));
        return;
    }
    QUrl url(szurl);
    QNetworkRequest request(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_login(QNetworkReply*)));
    multiPart->append(phonePart);
    multiPart->append(codePart);

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}

void login::slot_login(QNetworkReply* reply)
{
    reset_timer();
    QString str = reply->readAll();
#ifdef QT_DEBUG
    qDebug() << str;
#endif
    reply->deleteLater();
    using namespace rapidjson;
    Document d;
    if(d.Parse(str.toStdString().c_str()).HasParseError())
    {
        QMessageBox::information(this, tr("Info"), tr("server error"), QMessageBox::Ok);
    }
    if(d.HasMember("errCode") && d["errCode"].IsInt())
    {
        if(d["errCode"] != 10001)
        {
            std::string key = QString("%1").arg(d["errCode"].GetInt()).toStdString();
            QMessageBox::information(this, tr("Info"), status_map[QString::fromStdString(key)], QMessageBox::Ok);
        }
        else
        {
            this->accept();
        }
    }
    if(d.HasMember("data") && d["data"].IsObject())
    {
        const rapidjson::Value& obj = d["data"];
        if(obj.HasMember("header_token"))
        {
            const rapidjson::Value& v = obj["header_token"];
            m_token = v.GetString();
            qDebug() << m_token;
        }
    }
}

void login::slot_showsec()
{
    ui->pushButton_getChecknum->setText(QString("%1").arg(m_isec--));
    if(m_isec <= 0 && !ui->pushButton_getChecknum->text().compare("0"))
    {
       reset_timer();
    }
}


void login::on_lineedit_phone_num_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(!ui->lineedit_phone_num->text().isEmpty())
        ui->pushButton_getChecknum->setEnabled(true);
    else
        ui->pushButton_getChecknum->setEnabled(false);
}

void login::on_lineedit_check_num_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(!ui->lineedit_phone_num->text().isEmpty() && !ui->lineedit_check_num->text().isEmpty())
        ui->pushButton_login->setEnabled(true);
    else
        ui->pushButton_login->setEnabled(false);
}
