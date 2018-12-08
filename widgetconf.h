#ifndef WIDGETCONF_H
#define WIDGETCONF_H
#include <QMap>
#include <QString>
#include <QObject>
QMap<QString, QString> widget_status_map =
{
    {"10001", QObject::tr("success")},
    {"10002", QObject::tr("user token lose")},
    {"10003", QObject::tr("version not exist or expired")},
    {"10004", QObject::tr("operation failure")},
    {"10005", QObject::tr("yunliao not opened")},
    {"10006", QObject::tr("user not exist")},
    {"10007", QObject::tr("user already frozen")},
    {"40001", QObject::tr("Request parameter error or missing")},
    {"40002", QObject::tr("authentication code acquisition time is 60 seconds apart before it can be obtained")},
    {"40003", QObject::tr("today the number of validations has been exhausted")},
    {"40004", QObject::tr("verification code failed to send, please try again later")},
    {"40005", QObject::tr("authentication code does not exist or has expired")},
    {"40006", QObject::tr("authentication code error")},
    {"40010", QObject::tr("the label to bind does not exist")},
    {"40011", QObject::tr("the shortcut reply to edit does not exist")},
    {"40012", QObject::tr("the label you want to edit does not exist")},
    {"40013", QObject::tr("the label alreay existed")},
    {"40014", QObject::tr("the label you want to delete does not exist")},
    {"40015", QObject::tr("custom quick replies must not exceed 100")},
    {"40016", QObject::tr("labels must not exceed 100")}
};
#endif // WIDGETCONF_H
