#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

 //Add a background picture

     QPixmap bkg(":/fig/cover.jpg");
     bkg = bkg.scaled(this->size(), Qt::IgnoreAspectRatio);
     QPalette palette;
     palette.setBrush(QPalette::Background, bkg);
     this->setPalette(palette);

     ui->label->setStyleSheet("border-image: url(:/fig/cap.png);");
}

Dialog::~Dialog()
{
    delete ui;
}
