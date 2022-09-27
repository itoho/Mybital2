#ifndef HOVER_H
#define HOVER_H

#include <QWidget>

namespace Ui {
class hover;
}

class hover : public QWidget
{
    Q_OBJECT

public:
    explicit hover(QWidget *parent = nullptr);
    ~hover();

private:
    Ui::hover *ui;
};

#endif // HOVER_H
