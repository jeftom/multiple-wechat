#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

    QString getToken();
    QString getPhone();

signals:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_getChecknum_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_login_clicked();

    void replyFinished(QNetworkReply*);

    void slot_showsec();

    void slot_login(QNetworkReply* reply);

    void on_lineedit_phone_num_textChanged(const QString &arg1);

    void on_lineedit_check_num_textChanged(const QString &arg1);

private:
    bool init();
    void reset_timer();

private:
    Ui::login *ui;
    QPoint m_poWindowPos;
    QPoint m_poMousePos;
    QPoint m_poDPos;
    QTimer       *m_timer_showsec;//读秒
    int          m_isec;  //秒

    QString m_token;
    QString m_phone;
};

#endif // LOGIN_H
