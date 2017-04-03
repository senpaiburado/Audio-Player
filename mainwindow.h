#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtWidgets>
#include <QSignalMapper>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QString path;
    QString name;
    qint64 duration;

    void repairName();

private slots:
    void play();
    void pause();
    void stop();
    void setSongPath();
    void setVolume(int value);
    void setValueClock(qint64 value);
};

#endif // MAINWINDOW_H
