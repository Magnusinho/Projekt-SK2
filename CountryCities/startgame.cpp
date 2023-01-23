#include "startgame.h"
#include "ui_startgame.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTime>

startGame::startGame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::startGame)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, &startGame::readyRead);
    //socket->connectToHost("localhost", 3737);
    socket->connectToHost("192.168.1.5", 3737);

    connect(socket, &QTcpSocket::connected, this, &startGame::socketConnected);
    findrooms = new findRooms();
    findrooms->hide();

    lobbyroom = new lobby();
    lobbyroom->hide();

    gettingans = new gettingAns;
    gettingans->hide();

    showplayerans = new showPlayersAns;
    showplayerans->hide();

    totalscore = new totalScore;
    totalscore->hide();
}

startGame::~startGame()
{
    delete ui;
}


void startGame::on_pushButton_clicked()
{
    QJsonObject json;
    if(socket->waitForConnected(3000)){
        QString text = ui->lineEdit->text();
        if(text != "letter"){
            json["action"] = "show_rooms";
            json["nick"] = text;
            this->nickname = text;
            QJsonDocument doc(json);
            QByteArray jsonData = doc.toJson();
            socket->write(jsonData);
        }
    } else {
        qDebug() << "Error" << socket->errorString();
    }
}

void startGame::readyRead(){
    QByteArray dataFromServer;
    while(socket->bytesAvailable()){
        dataFromServer.append(socket->readAll());
    }
    QByteArray data = dataFromServer.left(dataFromServer.lastIndexOf('}')+1);
    QJsonDocument respDoc = QJsonDocument::fromJson(data);
    QJsonObject respJson = respDoc.object();
    //textBrowser->setText(QString(respDoc.toJson()));
    if(respJson["status"] == true){
        if(respJson["action"] == "show_rooms"){
            this->hide();
            totalscore->hide();
            findrooms = new findRooms();
            findrooms->socket = socket;
            findrooms->nickname = this->nickname;

            std::vector<findRooms::roomData> roomsVector;
            QVariantMap jsonMap = respJson.toVariantMap();
            QJsonArray jsonRooms = respJson["rooms"].toArray();
            for (int i=0; i<jsonRooms.size(); i++){
                QJsonObject roomObj = jsonRooms[i].toObject();
                findRooms::roomData room;
                room.name = roomObj["host"].toString();
                room.id = roomObj["id"].toInt();
                roomsVector.push_back(room);
            }
            findrooms->setRooms(roomsVector);
            findrooms->updateRoomItemWidgets();
            findrooms->show();
        } else if(respJson["action"] == "joined") {
            findrooms->hide();
            lobbyroom->setPlayerRule(respJson["role"].toString());
            lobbyroom->setRoomID(respJson["room_id"].toInt());
            lobbyroom->show();
            this->room_id = lobbyroom->roomID;
        } else if(respJson["action"] == "show_players_list") {
            QJsonArray jsonPlayersList = respJson["players"].toArray();
            QVector<QString> playersVec;
            for(int i=0; i<jsonPlayersList.size(); i++){
                QString playerName = jsonPlayersList[i].toString();
                playersVec.push_back(playerName);
            }
            lobbyroom->playersVec = playersVec;
            lobbyroom->updatePlayerList();
        } else if(respJson["action"] == "start_round") {
            lobbyroom->hide();
            showplayerans->hide();
            gettingans = new gettingAns();
            gettingans->socket = socket;
            gettingans->roomId = this->room_id;
            gettingans->letter = QString(char(respJson["letter"].toInt())).toUpper();
            QJsonArray jsonCategories = respJson["categories"].toArray();
            QVector<QString> categoriesVec;
            for(int i=0; i<jsonCategories.size(); i++){
                QString categoryName = jsonCategories[i].toString();
                categoriesVec.push_back(categoryName);
            }
            gettingans->categories = categoriesVec;
            gettingans->createWindow();
            gettingans->nickname = this->nickname;
            gettingans->show();
        } else if(respJson["action"] == "get_ans") {
            gettingans->sendAns();
        } else if(respJson["action"] == "show_ans") {
            gettingans->hide();
            showplayerans = new showPlayersAns();
            QJsonObject players = respJson["players"].toObject();
            for(auto player : players.keys()){
                showPlayersAns::playerAns ans;
                QJsonObject playerinfo = players[player].toObject();
                ans.playername = player;
                for(auto category : playerinfo.keys()){
                    if(category == "score"){
                        ans.score = playerinfo[category].toInt();
                    } else if(category == "total_score"){
                        ans.total_score = playerinfo[category].toInt();
                    } else {
                        ans.categories.append(category);
                        ans.answers.append(playerinfo[category].toString());
                    }

                }
                showplayerans->playersAnswersVec.append(ans);
            }
            showplayerans->setPlayerAnsOnWindow();
            showplayerans->show();
        } else if(respJson["action"] == "show_total_score") {
            showplayerans->hide();
            QJsonObject score = respJson["score"].toObject();
            for(auto nickname : score.keys()){
                totalScore::playerScore playerscore;
                playerscore.nickname = nickname;
                playerscore.score = score[nickname].toInt();
                totalscore->scoreRankingVec.append(playerscore);
            }
            totalscore->setPlayerScoreOnWindow();
            totalscore->show();
        } else {
            qDebug() << "Invalid message from serwer";
        }
    } else {
        qDebug() << "Error ocured";
    }
}

void startGame::socketConnected()
{
       findrooms->socket = socket;
       lobbyroom->socket = socket;
       gettingans->socket = socket;
}
