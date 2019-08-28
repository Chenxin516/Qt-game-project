#include "holdingwidget.h"
#include "dialog.h"
#include "ui_dialog.h"
#include "instruction.h"

//construct the holding widget, including wll it's layouts and connections
holdingwidget::holdingwidget(QWidget *parent) : QWidget(parent)
{

    this->setFixedSize(1000,537);
    dialog_window_button = new QPushButton("");
    game_window_button = new QPushButton("");
    const QSize btnSize = QSize(150, 50);
    const QSize btnSize2 = QSize(150, 62);

    game_window_button->setFixedSize(btnSize);
    dialog_window_button->setFixedSize(btnSize2);
    game_window_button->setStyleSheet("border-image: url(:/fig/playbutton.png);");
    dialog_window_button ->setStyleSheet("border-image: url(:/fig/helpbutton.png);");

    QHBoxLayout* button_layout = new QHBoxLayout;
    button_layout->addWidget(dialog_window_button);
    button_layout->addWidget(game_window_button);
    QWidget* button_holder_widget = new QWidget;
    button_holder_widget->setLayout(button_layout);

    main_layout = new QVBoxLayout;
    main_layout->addWidget(button_holder_widget);

    QPixmap bkg(":/fig/jump.png");
    bkg = bkg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkg);
    this->setPalette(palette);


    widget_holder = new QStackedWidget;
    second_win = new game;
    widget_holder->addWidget(second_win);
    first_win = new Dialog;
    widget_holder->addWidget(first_win);
    third_win = new instruction;
    widget_holder->addWidget(third_win);

    main_layout->addWidget(widget_holder);
    setLayout(main_layout);

    connect(dialog_window_button,SIGNAL(pressed()),
            this, SLOT(DisplayInstruction()));
    connect(game_window_button,SIGNAL(pressed()),
            this, SLOT(DisplayGame()));


    DisplayDialog();
}


//disaly the dialog window
void holdingwidget::DisplayDialog()
{
    widget_holder->setCurrentIndex(widget_holder->indexOf(first_win));
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sound/91476_Glorious_morning.mp3"));
    music -> play();
}

void holdingwidget::DisplayInstruction()
{
    widget_holder->setCurrentIndex(widget_holder->indexOf(third_win));

    QPixmap bkg(":/fig/howtoplay.png");
    bkg = bkg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkg);
    this->setPalette(palette);
    //dialog_window_button->hide();
}


//dislay the game window and its setting
void holdingwidget::DisplayGame()
{
    widget_holder->setCurrentIndex(widget_holder->indexOf(second_win));
    dialog_window_button -> hide();
    game_window_button -> hide();

    music -> stop();
    QPixmap bkg(":/fig/background.png");

    bkg = bkg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkg);
    this->setPalette(palette);
}
