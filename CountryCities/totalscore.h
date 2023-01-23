#ifndef TOTALSCORE_H
#define TOTALSCORE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

namespace Ui {
class totalScore;
}

class totalScore : public QWidget
{
    Q_OBJECT

public:
    explicit totalScore(QWidget *parent = nullptr);
    ~totalScore();
    struct playerScore{
        QString nickname;
        int score;
    };
    QVector<playerScore> scoreRankingVec;
    QVBoxLayout *layout;
    void setPlayerScoreOnWindow();

private:
    Ui::totalScore *ui;
};

#endif // TOTALSCORE_H
