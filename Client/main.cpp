#include "startgame.h"
#include "findrooms.h"
#include <QApplication>
#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startGame w1;
    w1.show();

    return a.exec();
}
