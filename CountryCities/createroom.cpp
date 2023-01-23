#include "createroom.h"
#include "ui_createroom.h"

createRoom::createRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createRoom)
{
    ui->setupUi(this);
}

createRoom::~createRoom()
{
    delete ui;
}

void createRoom::getRoomData()
{
    QString string = "Test";
}

void createRoom::on_pushButton_clicked()
{
    QJsonObject json;
    QJsonArray categories;

    if(ui->checkBox_country->isChecked()){
        categories.append("country");
    }
    if(ui->checkBox_city->isChecked()){
        categories.append("city");
    }
    if(ui->checkBox_plant->isChecked()){
        categories.append("plant");
    }
    if(ui->checkBox_animal->isChecked()){
        categories.append("animal");
    }
    if(ui->checkBox_job->isChecked()){
        categories.append("job");
    }
    if(ui->checkBox_color->isChecked()){
        categories.append("color");
    }
    json["action"] = "create";
    json["roundAmount"] = ui->spinBox_2->value();
    json["categories"] = categories;
    json["host"] = this->nickname;

    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();
    socket->write(jsonData);
    this->hide();

}
