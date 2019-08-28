#include "instruction.h"

instruction::instruction(QWidget *parent) : QWidget(parent)
{
    QPixmap bkg(":/fig/howtoplay.png");
    bkg = bkg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkg);
    this->setPalette(palette);
}
