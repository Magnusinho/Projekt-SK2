#include "lobby.h"
#include "ui_lobby.h"

lobby::lobby(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lobby)
{
    ui->setupUi(this);
}

lobby::~lobby()
{
    delete ui;
}

void lobby::updatePlayerList()
{
    ui->listWidget->clear();
    for(int i=0; i<playersVec.length(); i++){
        QListWidgetItem* item = new QListWidgetItem(this->playersVec[i]);
        ui->listWidget->addItem(item);
    }
}

void lobby::setRoomID(int value)
{
    roomID = value;
    ui->label_2->setText("ID pokoju: " + QString::number(roomID));
}

void lobby::setPlayerRule(const QString &value)
{
    playerRule = value;
    if(value == "admin"){
        ui->pushButton->setEnabled(true);
    } else {
        ui->pushButton->setEnabled(false);
    }
}

void lobby::on_pushButton_clicked()
{
    QJsonObject json;
    json["action"] = "start_round";
    json["room_id"] = roomID;
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();
    socket->write(jsonData);
}
