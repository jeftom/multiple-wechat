#ifndef BINDPHONE_H
#define BINDPHONE_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class bindphone;
}

class bindphone : public QDialog
{
    Q_OBJECT

public:
    explicit bindphone(QWidget *parent = 0);
    ~bindphone();

    void setToken(QString token);

    void setWechatNo(QString no);

private slots:
    void on_pushButton_ok_clicked();

    void slot_bind(QNetworkReply *reply);
    void on_pushButton_cancel_clicked();

private:
    Ui::bindphone *ui;
    QString m_token;
    QString m_wechatNo;
};

#endif // BINDPHONE_H
