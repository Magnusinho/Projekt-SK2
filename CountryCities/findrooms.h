#ifndef FINDROOMS_H
#define FINDROOMS_H

#include <QWidget>
#include "createroom.h"
#include "lobby.h"
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QJsonDocument>

namespace Ui {
class findRooms;
}

class findRooms : public QWidget
{
    Q_OBJECT

public:
    explicit findRooms();
    ~findRooms();
    struct roomData{
        QString name;
        int id;
    };
    QString nickname;
    QTcpSocket *socket;
    void updateRoomItemWidgets();
    void setRooms(std::vector<roomData> rooms);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::findRooms *ui;
    std::vector<roomData> roomDatas;
    createRoom *createroom;
    lobby *lobbyroom;
    void readyRead();
    void itemSelectedOnList();
};

#endif // FINDROOMS_H
