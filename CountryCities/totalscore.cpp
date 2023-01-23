#include "totalscore.h"
#include "ui_totalscore.h"

totalScore::totalScore(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::totalScore)
{
    ui->setupUi(this);
    layout = new QVBoxLayout();
}

totalScore::~totalScore()
{
    delete ui;
}

void totalScore::setPlayerScoreOnWindow()
{
    QString ranking = "<ol type=\"1\">";
    for(int i=0; i<this->scoreRankingVec.size(); i++){
        playerScore playerscore = scoreRankingVec[i];
        ranking += "<li>" + playerscore.nickname + " <b>" + QString::number(playerscore.score) + "</b></li>";
    }
    ranking += "</ol>";
    QLabel *label = new QLabel(ranking);
    layout->addWidget(label);
    setLayout(layout);
}
