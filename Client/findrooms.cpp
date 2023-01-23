#include "findrooms.h"
#include "ui_findrooms.h"


findRooms::findRooms() :
    QWidget(nullptr),
    ui(new Ui::findRooms)
{
    ui->setupUi(this);

    connect(ui->listWidget, &QListWidget::currentItemChanged, this, &findRooms::itemSelectedOnList);
    ui->listWidget->setEnabled(true);
}

findRooms::~findRooms()
{
    delete ui;
}

void findRooms::setRooms(std::vector<findRooms::roomData> rooms)
{
    this->roomDatas = rooms;
    //this->updateRoomItemWidgets();
}


void findRooms::updateRoomItemWidgets(){
    ui->listWidget->clear();
    for(int i = 0; i < int(this->roomDatas.size());i++){
        QListWidgetItem* item = new QListWidgetItem(QString::number(this->roomDatas[i].id) +" | " + this->roomDatas[i].name + "'s room");
        ui->listWidget->addItem(item);
    }
}

void findRooms::itemSelectedOnList()
{
    ui->pushButton_3->setEnabled(true);
}


void findRooms::on_pushButton_3_clicked()
{
    QListWidgetItem *curRoom = ui->listWidget->currentItem();
    QString temp = curRoom->text();
    QStringList parts = temp.split("|");
    int roomID = parts.at(0).toInt();
    QJsonObject json;
    json["action"] = "join";
    json["room_id"] = roomID;
    json["nick"] = this->nickname;
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();
    socket->write(jsonData);
}

void findRooms::on_pushButton_2_clicked()
{
    createroom = new createRoom();
    createroom-> socket = socket;
    createroom->nickname = this->nickname;
    createroom->show();
}

