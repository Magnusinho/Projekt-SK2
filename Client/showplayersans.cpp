#include "showplayersans.h"
#include "ui_showplayersans.h"

showPlayersAns::showPlayersAns(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showPlayersAns)
{
    ui->setupUi(this);
    layout = new QVBoxLayout();
}

showPlayersAns::~showPlayersAns()
{
    delete ui;
}

void showPlayersAns::setPlayerAnsOnWindow()
{
    for(int i=0; i<this->playersAnswersVec.size(); i++){
        playerAns playerans = playersAnswersVec[i];
        QString playerScoreLabel = "<ul><h3>" + playerans.playername + ":</h3>";
        for(int i=0; i<playerans.categories.size(); i++){
            if(playerans.categories[i] == "country"){
                playerScoreLabel = playerScoreLabel + "<li>Państwo: " + playerans.answers[i] + "</li>";
            } else if(playerans.categories[i] == "city"){
                playerScoreLabel = playerScoreLabel + "<li>Miasto: " + playerans.answers[i] + "</li>";
            } else if(playerans.categories[i] == "animal"){
                playerScoreLabel = playerScoreLabel + "<li>Zwierzę: " + playerans.answers[i] + "</li>";
            } else if(playerans.categories[i] == "plant"){
                playerScoreLabel = playerScoreLabel + "<li>Roślina: " + playerans.answers[i] + "</li>";
            } else if(playerans.categories[i] == "job"){
                playerScoreLabel = playerScoreLabel + "<li>Zawód: " + playerans.answers[i] + "</li>";
            } else if(playerans.categories[i] == "color"){
                playerScoreLabel = playerScoreLabel + "<li>Kolor: " + playerans.answers[i] + "</li>";
            }
        }
        playerScoreLabel = playerScoreLabel + "</ul>";
        QLabel *answerLabel = new QLabel(playerScoreLabel);
        layout->addWidget(answerLabel);
        QLabel *scoreLabel = new QLabel("Wynik w tej rundzie: " + QString::number(playersAnswersVec[i].score) + "\nWynik ostateczny: " + QString::number(playersAnswersVec[i].total_score));
        layout->addWidget(scoreLabel);
        }
    setLayout(layout);
}
