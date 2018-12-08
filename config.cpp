#include "config.h"
#include "thirdparty/rapidjson/rapidjson.h"
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/writer.h"
#include "thirdparty/rapidjson/stringbuffer.h"
#include "thirdparty/tinyxml/tinyxml2.h"
#include <QFileInfo>
#include <QApplication>
#include <QDebug>

config config::instance;

config* config::getSingleton()
{
    return &instance;
}


void config::test()
{
    //json库测试
    using namespace rapidjson;
    StringBuffer sb;
    Writer<StringBuffer> w(sb);
    w.StartObject();
    w.Key("hello");
    w.String("world");
    w.Key("test");
    w.Int(10);
    w.EndObject();

    //xml库测试
//    load_config_file();
//    for(auto& x:m_config)
//    {
//        qDebug() << x.first.c_str() << ":" << x.second.c_str();
//    }
}

int config::load_config_file(char* msg, int& len)
{
    using namespace tinyxml2;
    XMLDocument doc;
    QString strPath = QApplication::applicationDirPath()+"/config.xml";
    QFileInfo fileInfo(strPath);
    if(!fileInfo.exists())
    {
        char* tmp = (char*)"config file not exist";
        if(strlen(tmp) >= len)
        {
            char* t = (char*)"error msg is too long.";
            strcpy(msg, t);
            len = strlen(t)+1;
            return -3; //缓冲区太小，错误信息太长
        }
        else
        {
            strcpy(msg, tmp);
            len = strlen(tmp)+1;
            return -1; //配置文件不存在
        }
    }
    enum XMLError eRet = doc.LoadFile(strPath.toStdString().c_str());
    if(eRet != XML_SUCCESS)
    {
        std::string str =  std::string("config file format error: ") + doc.ErrorIDToName(eRet);
        if(str.length() >= len)
        {
            char* t = (char*)"error msg is too long.";
            strcpy(msg, t);
            len = strlen(t)+1;
            return -3; //缓冲区太小，错误信息太长
        }
        else
        {
            strcpy(msg, str.c_str());
            len = str.length()+1;
            return -2; //配置文件格式错误
        }
    }
    for(XMLNode* e = doc.RootElement()->FirstChild(); e != nullptr; e = e->NextSibling())
    {
        //过滤掉注释
         if(nullptr == e->ToElement())
             continue;
         m_config[e->Value()] = e->ToElement()->FirstAttribute()->Value();
    }
    return 0;
}

const char* config::getValue(const char* key)
{
    if(nullptr == key || m_config[key].empty())
        return nullptr;
    else
        return m_config[key].c_str();
}

void config::setValue(const char *key, const char *value)
{
    if(key == nullptr || value == nullptr)
        return;
    m_config[key] = value;
}
