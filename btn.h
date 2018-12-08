#ifndef __BTN__
#define __BTN__
#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT
public:
    Button(QWidget *parent = Q_NULLPTR);

protected:
    virtual void paintEvent ( QPaintEvent * event);

};

#endif

