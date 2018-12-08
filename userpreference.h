#ifndef USERPREFERENCE1_H
#define USERPREFERENCE1_H

#include <QDialog>

namespace Ui {
class userpreference;
}

class userpreference : public QDialog
{
    Q_OBJECT

public:
    explicit userpreference(QWidget *parent = 0);
    ~userpreference();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::userpreference *ui;
};

#endif // USERPREFERENCE1_H
