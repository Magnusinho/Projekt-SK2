#ifndef GETTINGANS_H
#define GETTINGANS_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>

namespace Ui {
class gettingAns;
}

class gettingAns : public QWidget
{
    Q_OBJECT

public:
    explicit gettingAns(QWidget *parent = nullptr);
    QVector<QString> categories;
    QString letter;
    QTcpSocket *socket;
    QVBoxLayout *layout;
    QLineEdit *city_ans;
    QLineEdit *country_ans;
    QLineEdit *animal_ans;
    QLineEdit *plant_ans;
    QLineEdit *job_ans;
    QLineEdit *color_ans;
    QPushButton *pushButton;
    int roomId;
    QString nickname;
    void pushButton_clicked();
    void createWindow();
    void sendAns();
};

#endif // GETTINGANS_H
