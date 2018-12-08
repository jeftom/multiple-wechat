#include "bindphone.h"
#include "conf.h"
#include "ui_bindphone.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QRegExp>
#include <QMessageBox>
#include "md5.h"
#include "config.h"
#include "thirdparty/rapidjson/document.h"

bindphone::bindphone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bindphone)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
}

bindphone::~bindphone()
{
    delete ui;
}

void bindphone::setToken(QString token)
{
    m_token = token;
}

void bindphone::setWechatNo(QString no)
{
    m_wechatNo = no;
}

void bindphone::on_pushButton_ok_clicked()
{
    QRegExp ep("^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\\d{8}$");
    QString strPhone = ui->lineEdit->text();
    if(!strPhone.isEmpty() && !ep.exactMatch(strPhone))
    {
        QMessageBox::critical(this, tr("error"), tr("phone is not valid."), QMessageBox::Ok);
        return;
    }
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart phonePart;
    phonePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    phonePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"phone\""));
    phonePart.setBody(strPhone.toLatin1());

    QHttpPart wxidPart;
    wxidPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    wxidPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"wxid\""));
    MD5 md5;
    md5.update(m_wechatNo.toStdString());
    const char* p = md5.toString().c_str();
    wxidPart.setBody(p);


    const char* szurl = config::getSingleton()->getValue((char*)"bindPhone");
    if(nullptr == szurl)
    {
        QMessageBox::critical(this, tr("error"), tr("bind phone url is not allowed empty."));
        return;
    }
    QUrl url(szurl);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("headerToken"),QByteArray(m_token.toLatin1()));
#ifdef QT_DEBUG
    qDebug() << m_token;
#endif
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_bind(QNetworkReply*)));
    multiPart->append(phonePart);
    multiPart->append(wxidPart);

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}


void bindphone::slot_bind(QNetworkReply* reply)
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
            if(!ui->lineEdit->text().isEmpty())
                QMessageBox::information(this, tr("Info"), tr("bind success."), QMessageBox::Ok);
            else
                QMessageBox::information(this, tr("Info"), tr("unbind success."), QMessageBox::Ok);
            this->accept();
        }
        else
        {
            std::string key = QString("%1").arg(d["errCode"].GetInt()).toStdString();
            QMessageBox::information(this, tr("Info"), conf_map[QString::fromStdString(key)], QMessageBox::Ok);
        }
    }
}



void bindphone::on_pushButton_cancel_clicked()
{
    this->reject();
}
