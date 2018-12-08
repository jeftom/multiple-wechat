#include "userpreference.h"
#include "ui_userpreference.h"
#include "thirdparty/tinyxml/tinyxml2.h"
#include "config.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

userpreference::userpreference(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userpreference)
{
    ui->setupUi(this);
}

userpreference::~userpreference()
{
    delete ui;
}

void userpreference::on_pushButton_close_clicked()
{
    this->close();
}

void userpreference::on_pushButton_ok_clicked()
{
    using namespace tinyxml2;
    XMLDocument doc;
    QString strConfPath = QApplication::applicationDirPath()+"/config.xml";
    enum XMLError eRet = doc.LoadFile(strConfPath.toStdString().c_str());
    if(eRet != XML_SUCCESS)
    {
        std::string str =  std::string("config file format error: ") + doc.ErrorIDToName(eRet);
        QMessageBox::critical(this, tr("error"), str.c_str(), QMessageBox::Ok);
        return;
    }

    QString strPath = ui->lineEdit->text();
    strPath.replace('\\', '/');
    if(!QFileInfo::exists(strPath))
    {
        QMessageBox::critical(this, tr("error"), tr("the content you fill not valid or the path is not exist."));
        return;
    }
    XMLElement* ele = doc.RootElement()->FirstChildElement("wechatInstallPath");
    ele->SetAttribute("path", strPath.toStdString().c_str());
    doc.SaveFile(strConfPath.toStdString().c_str());

    config::getSingleton()->setValue(ele->Value(), strPath.toStdString().c_str());
    this->accept();
}
