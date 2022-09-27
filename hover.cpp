#include "hover.h"
#include "ui_hover.h"

hover::hover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hover)
{
    ui->setupUi(this);
}

hover::~hover()
{
    delete ui;
}
