#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QApplication>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include "ui_form.h"

game::game(QWidget *parent) : QWidget(parent),
ui(new Ui::Form)
{
    //set the cursor to be the hammer
    ui->setupUi(this);
    QCursor cursor;
    QPixmap pixmap(":/fig/hammer1.png");
    cursor = QCursor(pixmap);
    this->setCursor(cursor);
    ui->label_4 ->setStyleSheet("border-image: url(:/fig/easy.png);");

    // set current song

    currentSong = all_song[song_pos];

    ui->pushButton_14->setStyleSheet("border-image: url(:/fig/button_settings.png);");
    ui->pushButton_13->setStyleSheet("border-image: url(:/fig/button_play.png);");
    ui->pushButton_11->setStyleSheet("border-image: url(:/fig/button_decline.png);");
    ui->pushButton_12->setStyleSheet("border-image: url(:/fig/button_accept.png);");
    ui->pushButton_10->setStyleSheet("border-image: url(:/fig/button_plus.png);");
    ui->pushButton_15->setStyleSheet("border-image: url(:/fig/button_leaderbord.png);");
    ui->label_5 ->setStyleSheet("border-image: url(:/fig/scoreboard.png);");


    ui->tableWidget->hide();
    //set progress bar
    ui->progressBar -> setMinimum(0);
    ui->progressBar ->setMaximum(currentSong.length()-1);
    ui->progressBar ->setValue(0);
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->label_9 -> setStyleSheet("border-image: url(:/fig/win2.png);");
    set_all_holes_empty();
    mapSongs.insert("Basic Scale","123");
    mapSongs.insert("Little Star","115566544332215544332554433211556654433221");
    mapSongs.insert("Ode to Joy","334554321123322334554321123211");

    ui->progressBar->hide();
    ui->prompt_button->setStyleSheet("border-image: url(:/fig/emty.png);");

    ui->pushButton->hide();


    connect(ui->pushButton_14, SIGNAL(pressed()), this, SLOT(custom_song()));

    connect(ui->pushButton_13, SIGNAL(pressed()), this, SLOT(time()));


    connect(ui->pushButton_12, SIGNAL(pressed()), this, SLOT(resume()));
    connect(ui->pushButton_11, SIGNAL(pressed()), this, SLOT(quit()));
    //conect the next button to change song
    connect(ui->pushButton_10, SIGNAL(pressed()), this, SLOT(change_song()));
    connect(ui->pushButton_15, SIGNAL(pressed()), this, SLOT(show_table()));

    // connect the button for button pressed and released
    connect(ui->pushButton_1, SIGNAL(pressed()), this, SLOT(press_1()));
    connect(ui->pushButton_2, SIGNAL(pressed()), this, SLOT(press_2()));
    connect(ui->pushButton_3, SIGNAL(pressed()), this, SLOT(press_3()));

    connect(ui->pushButton_4, SIGNAL(pressed()), this, SLOT(press_4()));
    connect(ui->pushButton_5, SIGNAL(pressed()), this, SLOT(press_5()));
    connect(ui->pushButton_6, SIGNAL(pressed()), this, SLOT(press_6()));

    connect(ui->pushButton_7, SIGNAL(pressed()), this, SLOT(press_7()));
    connect(ui->pushButton_8, SIGNAL(pressed()), this, SLOT(press_8()));
    connect(ui->pushButton_9, SIGNAL(pressed()), this, SLOT(press_9()));

    connect(ui->pushButton_1, SIGNAL(released()), this, SLOT(release_1()));
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(release_2()));
    connect(ui->pushButton_3, SIGNAL(released()), this, SLOT(release_3()));

    connect(ui->pushButton_4, SIGNAL(released()), this, SLOT(release_4()));
    connect(ui->pushButton_5, SIGNAL(released()), this, SLOT(release_5()));
    connect(ui->pushButton_6, SIGNAL(released()), this, SLOT(release_6()));

    connect(ui->pushButton_7, SIGNAL(released()), this, SLOT(release_7()));
    connect(ui->pushButton_8, SIGNAL(released()), this, SLOT(release_8()));
    connect(ui->pushButton_9, SIGNAL(released()), this, SLOT(release_9()));
}

void game::time(){
    ui->label ->setText(song_name[0]);
    ui->label ->show();
    ui->label_2 ->setNum(score);
    ui->label_2 ->show();
    ui->label_3 ->show();
    ui->pushButton_1->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->pushButton_5->show();
    ui->pushButton_6->show();
    ui->pushButton_7->show();
    ui->pushButton_8->show();
    ui->pushButton_9->show();
    ui->pushButton_10->show();
    ui->pushButton_11->show();
    ui->pushButton_12->show();
    ui->pushButton_14->show();
    ui->progressBar->show();
    ui->prompt_button->show();

    // set the timer for the game
    ui->pushButton_13->hide();
    this->timer_2 = new QTimer;
    timer_2->start(1);
    connect(this->timer_2,SIGNAL(timeout()),this,SLOT(countdown()));

    this->prompt_timer = new QTimer;
    prompt_timer -> start(3500);
    connect(this->prompt_timer,SIGNAL(timeout()),this,SLOT(prompt()));

    this->timer = new QTimer;
    set_all_holes_empty();
    timer -> start(4000);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(generate_mouse()));
    this->time_lapse = 2000;

}

void game::quit(){
    if(timer != nullptr){
    timer ->blockSignals(true);
    timer_2 ->blockSignals(true);
    animation_timer_1->blockSignals(true);
    animation_timer_2->blockSignals(true);
    animation_timer_3->blockSignals(true);
    prompt_timer -> blockSignals(true);
    }
}

void game::resume(){
    if(timer!=nullptr){
    timer ->blockSignals(false);
    timer_2 ->blockSignals(false);
    animation_timer_1->blockSignals(false);
    animation_timer_2->blockSignals(false);
    animation_timer_3->blockSignals(false);
    prompt_timer -> blockSignals(false);
    }
}

/*
 * add user's custom song
*/
void game::custom_song(){
    if(timer == nullptr)
        return;
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("Enter the song u want to play:"), QLineEdit::Normal, "",&ok);
    QVector<int> new_song;
    if (ok && !text.isEmpty())
            for (int i=0;i<text.size();i++){
                int j = text[i].digitValue();
                new_song.push_back(j);
            }
    all_song.push_back(new_song);
    song_name.push_back("Custom Song "+ QString::number(num_of_custom+1));
    num_of_custom ++;
    mapSongs.insert("User Custom Song "+ QString::number(num_of_custom),text);
    highest_score.push_back(0);
}

/*
 * show the song table that stores all the song names and notes
 */
void game::show_table()
{
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    if(table){
        ui->tableWidget->setRowCount(mapSongs.count());
        int row = 0;
        for(auto it: mapSongs.toStdMap()){
            QString name = it.first;
            QString note = it.second;
            int high_score = highest_score[row];
            QTableWidgetItem *ItemName = new QTableWidgetItem();
            QTableWidgetItem *ItemNote = new QTableWidgetItem();
            QTableWidgetItem *ItemScore = new QTableWidgetItem();
            ItemName ->setText(name);
            ItemNote ->setText(note);
            ItemScore ->setText(QString::number(high_score));
            ui->tableWidget->setItem(row, 0, ItemName);
            ui->tableWidget->setItem(row, 1, ItemNote);
            ui->tableWidget->setItem(row, 2, ItemScore);
            row++;
        }
        ui->tableWidget->show();
        table = false;
    }
    else {
        ui->tableWidget->hide();
        table = true;
    }
}

//select random holes to generate mouse
void game::generate_mouse()
{
    set_all_holes_empty();
    ui->progressBar->setValue(pos);
    check_1 = true;
    check_2 = true;
    check_3 = true;
    appear_stage_1 = 0;
    appear_stage_2 = 0;
    appear_stage_3 = 0;
    note = currentSong[pos]; //get the correct color number
    other_color_1 = generate_color_not_equal_to(note); // generate one wrong color
    other_color_2 = generate_color_not_equal_to(note); // generate the other wrong color

    //generate random index for three mouses
    int mouse_correct_index = rand()%9 + 1; // the button index for correct color
    correct_button_index = mouse_correct_index;
    int mouse_1_index = generate_index_not_equal_to(mouse_correct_index, 58); // the button index for wrong color
    int mouse_2_index = generate_index_not_equal_to(mouse_correct_index, mouse_1_index);
    wrong_button_index_1 = mouse_1_index;
    wrong_button_index_2 = mouse_2_index;

    //assign three buttons based on the index
    mouse_correct_button = select_hole(mouse_correct_index);
    mouse_1_button = select_hole(mouse_1_index);
    mouse_2_button = select_hole(mouse_2_index);

    // 1,2,3 timer control the appearing of mouses
    this->animation_timer_1 = new QTimer;
    this->animation_timer_2 = new QTimer;
    this->animation_timer_3 = new QTimer;
    animation_timer_1 ->start(time_lapse_animation);
    animation_timer_2 ->start(time_lapse_animation);
    animation_timer_3 ->start(time_lapse_animation);
    connect(this->animation_timer_1,SIGNAL(timeout()),this,SLOT(mouse_appear_animation_1()));
    connect(this->animation_timer_2,SIGNAL(timeout()),this,SLOT(mouse_appear_animation_2()));
    connect(this->animation_timer_3,SIGNAL(timeout()),this,SLOT(mouse_appear_animation_3()));


    //set the time interval to be normal (first time we need to wait fot ld)
    this->timer->setInterval(time_lapse);

    //move to the position of next note
    pos ++;
    //check if we reach the end of the song
    if(pos == currentSong.length())
    {
        //play winning music or fail music
                timer ->stop();
                prompt_timer -> stop();
                game_over = true;
                ui->prompt_button->setStyleSheet("border-image: url(:/fig/emty.png);");
                set_all_holes_empty();
    }
}


/*
 * the animation of mousing appearing and diasappearing
 */
void game::mouse_appear_animation_1()
{
    if(check_1){
        appear_stage_1++;
       }
    mouse_correct_button->setStyleSheet("border-image: url(:/fig/mouse"
                          + QString::number(this->note)
                          + QString::number(this->appear_stage_1) + ");");

    qDebug()<<appear_stage_1;

    if(!check_1){
        appear_stage_1--;
    }
    if (appear_stage_1 == 3)
    {
        check_1 =false;
    }
    if (appear_stage_1 <= -1)
    {
        animation_timer_1 ->stop();
        music_play();
        correct_button_index = 0;
        wrong_button_index_1 = 0;
        wrong_button_index_2 = 0;
    }
}

void game::mouse_appear_animation_2()
{
    if(check_2){
        appear_stage_2++;
       }

    mouse_1_button->setStyleSheet("border-image: url(:/fig/mouse"
                          + QString::number(other_color_1)
                          + QString::number(this->appear_stage_2) + ");");
    if(!check_2){
        appear_stage_2--;
    }
    if (appear_stage_2 == 3)
    {
        check_2 =false;
    }
    if (appear_stage_2 <= -1)
    {
        animation_timer_2 ->stop();
        music_play();
        correct_button_index = 0;
        wrong_button_index_1 = 0;
        wrong_button_index_2 = 0;
    }
}

void game::mouse_appear_animation_3()
{
    if(check_3){
        appear_stage_3++;
    }
    mouse_2_button->setStyleSheet("border-image: url(:/fig/mouse"
                          + QString::number(other_color_2)
                          + QString::number(this->appear_stage_3) + ");");
    if(!check_3){
        appear_stage_3--;
    }
    if (appear_stage_3 == 3)
    {
        check_3 = false;
    }
    if (appear_stage_3 <= -1)
    {
        animation_timer_3 ->stop();
        music_play();
        correct_button_index = 0;
        wrong_button_index_1 = 0;
        wrong_button_index_2 = 0;
    }
}

void game::music_play()
{
    if (game_over == true && check_correct >= currentSong.length()-2)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/winning.mp3"));//play warning music
        music -> play();
        ui->label_5->show();
        win_lose_display();
    }
    else if (game_over == true && check_correct < currentSong.length()-2)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/fail.mp3"));//play warning music
        music -> play();
        ui->label_5->show();
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->label_6 -> setStyleSheet("border-image: url(:/fig/cry.png);");
        ui->label_7 -> setStyleSheet("border-image: url(:/fig/cry.png);");
        ui->label_8 -> setStyleSheet("border-image: url(:/fig/cry.png);");
        ui->label_9 -> setStyleSheet("border-image: url(:/fig/youlose.png);");
        ui->label_9->show();
    }
    //update highest score
    if(check_correct > highest_score[song_pos]){
        highest_score[song_pos] = check_correct;
    }
}

void game::win_lose_display()
{
    ui->label_9 -> setStyleSheet("border-image: url(:/fig/win2.png);");
    ui->label_9->show();
    if (check_correct == currentSong.length()){
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->label_6 -> setStyleSheet("border-image: url(:/fig/star.png);");
        ui->label_7 -> setStyleSheet("border-image: url(:/fig/star.png);");
        ui->label_8 -> setStyleSheet("border-image: url(:/fig/star.png);");
    }
    else if (check_correct == currentSong.length()-1) {
        ui->label_6->show();ui->label_7->show();
        ui->label_6 -> setStyleSheet("border-image: url(:/fig/star.png);");
        ui->label_7 -> setStyleSheet("border-image: url(:/fig/star.png);");
    }
    else if (check_correct == currentSong.length()-2)
    {
        ui->label_6->show();
        ui->label_6 -> setStyleSheet("border-image: url(:/fig/star.png);");
    }
}

int game::generate_index_not_equal_to(int index1, int index2)
{
    int num = 0;
    while(num == index1 || num ==index2 || num == 0)
    {
        num = rand()%9+1;
    }
    return num;
}

int game::generate_color_not_equal_to(int color)
{
    int num = 0;
    while(num == color || num == 0)
    {
        num = rand()%7+1;
    }
    return num;
}

//set the prompt mouse for user
void game::prompt()
{
    if (prompt_pos == currentSong.length())
    {
        prompt_timer->stop();
        prompt_pos = 0;
    }
    prompt_timer->setInterval(time_lapse_prompt);
    int prompt_note = currentSong[prompt_pos];
    ui->prompt_button->setStyleSheet("border-image: url(:/fig/mouse" +QString::number(prompt_note) + QString::number(3) + ");");
    prompt_pos ++;
}

void game::countdown()
{
    this->ui->pushButton->show();
    timer_2->start(1000);
    if (display_num == 0)
    {
        display_num = 3;
        this->timer_2->stop();
        this->ui->pushButton->hide();
    }
    ui->pushButton->setStyleSheet("border-image: url(:/fig/Number" + QString::number(display_num)+ ");");
    display_num --;
}

//set the images of all holes to be empty
void game::set_all_holes_empty()
{
    for (int i = 1; i <= 9; ++i)
    {
        select_hole(i)->setStyleSheet("border-image: url(:/fig/emty.png);");
    }
}

//select the button of holes by integer index
QPushButton* game::select_hole(int num)
{
    switch (num)
    {
    case 1:return ui->pushButton_1;
    case 2:return ui->pushButton_2;
    case 3:return ui->pushButton_3;
    case 4:return ui->pushButton_4;
    case 5:return ui->pushButton_5;
    case 6:return ui->pushButton_6;
    case 7:return ui->pushButton_7;
    case 8:return ui->pushButton_8;
    case 9:return ui->pushButton_9;
    }
}

/*
 * change to the next song when clicking
 */
void game::change_song()
{
    if(timer == nullptr)
        return;
    ui->label_5 ->hide();
    ui->label_6 ->hide();
    ui->label_7 ->hide();
    ui->label_8 ->hide();
    ui->label_9 ->hide();
    ui ->label_2 -> setNum(0);
    score = 0;
    check_correct = 0;
    pos_of_name ++;
    game_over = false;
    timer->stop();
    prompt_timer->stop();
    timer_2 ->stop();
    animation_timer_1->stop();
    animation_timer_2->stop();
    animation_timer_3->stop();

    ui->prompt_button->setStyleSheet("border-image: url(:/fig/emty.png);");
    if (pos_of_name == song_name.size())
    {
        pos_of_name = 0;
        ui->label->setText(song_name[pos_of_name]);
    }
    else
        ui->label->setText(song_name[pos_of_name]);

    prompt_pos = 0;
    pos = 0;
    song_pos ++;
    if (song_pos == all_song.size())
    {
        song_pos = 0;
        currentSong = all_song[song_pos];
    }
    else
        currentSong = all_song[song_pos];

    set_all_holes_empty();
    ui->progressBar ->setMinimum(0);
    ui->progressBar ->setMaximum(currentSong.length()-1);
    ui->progressBar ->setValue(0);

    delete timer_2;
    this->timer_2 = new QTimer;
    timer_2->start(600);
    display_num = 3;
    connect(this->timer_2,SIGNAL(timeout()),this,SLOT(countdown()));

    delete prompt_timer;
    this->prompt_timer = new QTimer;

    if (song_pos == 2) {
        prompt_timer -> start(3600);
        time_lapse = 1200;
        time_lapse_prompt = 1200;
        time_lapse_animation = 120;
        ui->label_4 ->setStyleSheet("border-image: url(:/fig/hard.png);");
    }
    else {
        prompt_timer -> start(3400);
        time_lapse = 1600;
        time_lapse_prompt = 1600;
        time_lapse_animation = 160;
        ui->label_4 ->setStyleSheet("border-image: url(:/fig/medium.png);");
        if(song_pos==0){
            ui->label_4 ->setStyleSheet("border-image: url(:/fig/easy.png);");
        }
    }

    connect(this->prompt_timer,SIGNAL(timeout()),this,SLOT(prompt()));

    delete timer;
    this->timer = new QTimer;
    if(song_pos == 2){
    timer -> start(3850);
    }
    else{
        timer->start(3900);
    }
    connect(this->timer,SIGNAL(timeout()),this,SLOT(generate_mouse()));
}


void game::press_1(){
    if(correct_button_index == 1)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_1 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note) + ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 1)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_1 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1) + ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 1)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();


        ui->pushButton_1 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2)+ ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_2(){
    if(correct_button_index == 2)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_2 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note) + ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 2)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_2 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1) + ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 2)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_2 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2) + ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_3(){
    if(correct_button_index == 3)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_3 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note) + ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 3)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_3 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1)+ ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 3)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_3 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2)+ ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_4(){
    if(correct_button_index == 4)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_4 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note) + ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 4)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_4 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1) + ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 4)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_4 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2) + ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_5(){
    if(correct_button_index == 5)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_5 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note)+ ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 5)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_5 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1) + ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 5)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_5 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2) + ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_6(){
    if(correct_button_index == 6)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_6 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note) + ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 6)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_6 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1)+ ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 6)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_6 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2) + ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_7(){
    if(correct_button_index == 7)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_7 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note)+ ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 7)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_7 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1)+ ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 7)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_7 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2) + ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}
void game::press_8(){
    if(correct_button_index == 8)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_8 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note)+ ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 8)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_8 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1)+ ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 8)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_8 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2)+ ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::press_9(){
    if(correct_button_index == 9)
    {
        score++; ui->label_2 ->setNum(score);
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/note" + QString::number(note) + ".mp3"));//play note music
        music -> play();
        check_correct++;
        //show beaten image
        ui->pushButton_9 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(note) + ");");
        animation_timer_1->stop();
    }
    else if(wrong_button_index_1 == 9)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_9 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_1)+ ");");
        //stop all the timers
        animation_timer_2->stop();
    }
    else if(wrong_button_index_2 == 9)
    {
        QMediaPlayer* music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/sound/wrong.mp3"));//play warning music
        music -> play();

        ui->pushButton_9 ->setStyleSheet("border-image: url(:/fig/mouse_beaten"
                                         + QString::number(other_color_2)+ ");");
        //stop all the timers
        animation_timer_3->stop();
    }
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

void game::release_1(){
    ui->pushButton_1->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_2(){
    ui->pushButton_2->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_3(){
    ui->pushButton_3->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_4(){
    ui->pushButton_4->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_5(){
    ui->pushButton_5->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_6(){
    ui->pushButton_6->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_7(){
    ui->pushButton_7->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_8(){
    ui->pushButton_8->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}
void game::release_9(){
    ui->pushButton_9->setStyleSheet("border-image: url(:/fig/emty.png);");
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}

//set the cursor image
void game::mousePressEvent(QMouseEvent *event)
{
    this->setCursor(QCursor(QPixmap(":/fig/hammer2.png")));
}

//set the cursor image
void game::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(QCursor(QPixmap(":/fig/hammer1.png")));
}

game::~game()
{
    delete ui;
}

/*
 change the speed of mouse appearing by changing dial value
 */
void game::on_dial_valueChanged(int value)
{
    this->setCursor(Qt::ArrowCursor);
    time_lapse = 2000 - 10*value;
    time_lapse_prompt = 2000 - 9*value;
    time_lapse_animation = 0.1*time_lapse;
    if (value <= 35)
        ui->label_4 ->setStyleSheet("border-image: url(:/fig/easy.png);");
    else if (value> 35 && value < 75) {
        ui->label_4 ->setStyleSheet("border-image: url(:/fig/medium.png);");
    }
    else {
        ui->label_4 ->setStyleSheet("border-image: url(:/fig/hard.png);");
    }
}
