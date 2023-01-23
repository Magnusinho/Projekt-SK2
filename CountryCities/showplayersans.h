#ifndef SHOWPLAYERSANS_H
#define SHOWPLAYERSANS_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

namespace Ui {
class showPlayersAns;
}

class showPlayersAns : public QWidget
{
    Q_OBJECT

public:
    explicit showPlayersAns(QWidget *parent = nullptr);
    ~showPlayersAns();
    struct playerAns{
        QString playername;
        QVector<QString> categories;
        QVector<QString> answers;
        int score;
        int total_score;
    };
    QVector<playerAns> playersAnswersVec;
    QVBoxLayout *layout;
    void setPlayerAnsOnWindow();

private:
    Ui::showPlayersAns *ui;
};

#endif // SHOWPLAYERSANS_H
