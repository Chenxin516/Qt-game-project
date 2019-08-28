#include "dialog.h"
#include "holdingwidget.h"
#include "game.h"
#include <QApplication>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    holdingwidget h;
    h.show();

    return a.exec();
}

