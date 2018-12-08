#ifndef QPNGLINEEDITEX_H
#define QPNGLINEEDITEX_H

#include <QPushButton>
class QLineEdit;
class QString;

class QPngLineEdit : public QPushButton
{
    Q_OBJECT
public:
    explicit QPngLineEdit(const QString &text, QLineEdit *edit, const QString &strImg);

signals:

public slots:

};

#endif // QPNGLINEEDITEX_H