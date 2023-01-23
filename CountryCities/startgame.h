#ifndef STARTGAME_H
#define STARTGAME_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTextBrowser>
#include "findrooms.h"
#include "lobby.h"
#include "gettingans.h"
#include "showplayersans.h"
#include "totalscore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class startGame; }
QT_END_NAMESPACE

class startGame : public QMainWindow
{
    Q_OBJECT

public:
    startGame(QWidget *parent = nullptr);
    ~startGame();
    QString nickname;
    int room_id;

private slots:
    void on_pushButton_clicked();
    void readyRead();
    void socketConnected();

private:
    Ui::startGame *ui;
    QTcpSocket *socket;
    findRooms *findrooms;
    lobby *lobbyroom;
    gettingAns *gettingans;
    showPlayersAns *showplayerans;
    totalScore *totalscore;

};
#endif // STARTGAME_H
