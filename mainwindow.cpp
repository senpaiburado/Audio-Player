#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    player->setVolume(ui->volumeSlider->value());
    path = "";
    ui->clock->


    connect(ui->setMusicButton, SIGNAL(pressed()), this, SLOT(setSongPath()));
    connect(ui->stopButton, SIGNAL(pressed()), this, SLOT(stop()));
    connect(ui->pauseButton, SIGNAL(pressed()), this, SLOT(pause()));
    connect(ui->playButton, SIGNAL(pressed()), this, SLOT(play()));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    connect(player, &QMediaPlayer::positionChanged, this, [&](qint64 value){
        int pos = static_cast<int>(value);
        ui->positionSlider->setValue(pos);
    });
    connect(ui->positionSlider, &QSlider::valueChanged, this, [&](int value){
        if (ui->positionSlider->isSliderDown())
            player->setPosition(value);
    });
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setValueClock(qint64)));
    connect(player, &QMediaPlayer::stateChanged, this, [&](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState)
        {
            ui->playButton->setEnabled(true);
            ui->stopButton->setEnabled(false);
            ui->pauseButton->setEnabled(false);
        }
    });

    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
}

void MainWindow::repairName()
{
    QString temp = "";
    int index = 0;
    for(int i(name.length()-1); i > 0; i--)
    {
        if (name[i] == '/')
        {
            index = i+1;
            break;
        }
    }

    for (int i(0); index < name.length(); index++, i++)
        temp[i] = name[index];
    name = temp;
}

void MainWindow::play()
{
    if (path != "")
    {
        ui->stopButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        ui->playButton->setEnabled(false);
        ui->positionSlider->setMaximum(this->duration);
        player->play();
    }
    else
        QMessageBox::critical(NULL,QObject::tr("Error!"),tr("Please choose a song!"));
}

void MainWindow::pause()
{
    ui->playButton->setEnabled(true);
    player->pause();
}

void MainWindow::stop()
{
    ui->playButton->setEnabled(true);
    if (ui->pauseButton->isEnabled())
        ui->pauseButton->setEnabled(false);
    player->stop();
}

void MainWindow::setSongPath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open audio"), "", tr("Audio Files (*.mp3 *.wav)"));
    this->path = path;
    name = QFile(path).fileName();
    repairName();
    ui->songNameLabel->setText(name);
    connect(player, &QMediaPlayer::durationChanged, this, [&](qint64 dur) {
        this->duration = dur;
        qDebug() << "Duration: " << dur << endl;
    });
    player->setMedia(QMediaContent(QUrl(path)));
    qDebug() << "Error: " << player->error() << " : " << player->errorString() << endl;
}

void MainWindow::setVolume(int value)
{
    player->setVolume(value);
}

void MainWindow::setValueClock(qint64 value)
{
    double temp = static_cast<double>(value/1000);
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    seconds = temp;
    for (; seconds > 60; seconds -= 60)
        minutes++;
    seconds = temp;
    for (; seconds > 3600; seconds -= 3600)
        hours++;
    seconds = temp - (hours*3600+minutes*60);

    if (hours > 0)
        ui->clock->display(QString(QString::number(hours) + ":" + QString::number(minutes) + ":" + (seconds >= 10 ? QString::number(seconds) : "0"+QString::number(seconds))));
    else if (minutes > 0)
        ui->clock->display(QString(QString::number(minutes) + ":" + (seconds >= 10 ? QString::number(seconds) : "0"+QString::number(seconds))));
    else
        ui->clock->display(QString((seconds >= 10 ? QString::number(seconds) : "0"+QString::number(seconds))));
}
