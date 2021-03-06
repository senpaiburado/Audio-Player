#include <QApplication>
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    MainWindow window;
    window.setFixedSize(window.width(), window.height());
    window.show();

    return app.exec();
}
