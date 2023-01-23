#include "gettingans.h"
#include "ui_gettingans.h"


gettingAns::gettingAns(QWidget *parent) :
    QWidget(parent)
{
    layout = new QVBoxLayout;
}

void gettingAns::pushButton_clicked()
{
    QJsonObject json;
    json["action"] = "stop_game";
    json["room_id"] = roomId;
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();
    socket->write(jsonData);
}

void gettingAns::createWindow()
{
    layout = new QVBoxLayout;
    QLabel *label = new QLabel("Litera w rundzie: " + this->letter);
    layout->addWidget(label);
    for(int i=0; i<this->categories.size(); i++){
        QHBoxLayout *row = new QHBoxLayout;
        if(categories[i] == "country"){
            QLabel *label = new QLabel("Państwo");
            country_ans = new QLineEdit;
            row->addWidget(label);
            row->addWidget(country_ans);
            country_ans->setEnabled(true);
        } else if(categories[i] == "city") {
            QLabel *label = new QLabel("Miasto");
            city_ans = new QLineEdit;
            row->addWidget(label);
            row->addWidget(city_ans);
            city_ans->setEnabled(true);
        } else if(categories[i] == "plant") {
            QLabel *label = new QLabel("Roślina");
            plant_ans = new QLineEdit;
            row->addWidget(label);
            row->addWidget(plant_ans);
            plant_ans->setEnabled(true);
        } else if(categories[i] == "animal") {
            QLabel *label = new QLabel("Zwierzę");
            animal_ans = new QLineEdit;
            row->addWidget(label);
            row->addWidget(animal_ans);
            animal_ans->setEnabled(true);
        } else if(categories[i] == "job") {
            QLabel *label = new QLabel("Zawód");
            job_ans = new QLineEdit;
            row->addWidget(label);
            row->addWidget(job_ans);
            job_ans->setEnabled(true);
        } else if(categories[i] == "color") {
            QLabel *label = new QLabel("Kolor");
            color_ans = new QLineEdit;
            row->addWidget(label);
            row->addWidget(color_ans);
            color_ans->setEnabled(true);
        }

        layout->addLayout(row);
    }
    pushButton = new QPushButton("Zatwierdź odpowiedź");
    pushButton->setEnabled(true);
    connect(pushButton, &QPushButton::clicked, this, &gettingAns::pushButton_clicked);

    layout->addWidget(pushButton);
    setLayout(layout);
}

void gettingAns::sendAns()
{
    pushButton->setEnabled(false);
    QJsonObject json;
    QJsonObject ans;
    for(int i=0; i<this->categories.size(); i++){
        if(categories[i] == "country"){
            ans["country"] = country_ans->text();
            country_ans->setEnabled(false);
        } else if(categories[i] == "city") {
           ans["city"] = city_ans->text();
           city_ans->setEnabled(false);
        } else if(categories[i] == "plant") {
            ans["plant"] = plant_ans->text();
            plant_ans->setEnabled(false);
        } else if(categories[i] == "animal") {
           ans["animal"] = animal_ans->text();
           animal_ans->setEnabled(false);
        } else if(categories[i] == "job") {
           ans["job"] = job_ans->text();
           job_ans->setEnabled(false);
        } else if(categories[i] == "color") {
            ans["color"] = color_ans->text();
            color_ans->setEnabled(false);
        }
    }
    json["letter"] = letter;
    json["ans"] = ans;
    json["action"] = "send_ans";
    json["room_id"] = roomId;
    json["nickname"] = nickname;
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();
    socket->write(jsonData);
}
