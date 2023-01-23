#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTcpSocket>

namespace Ui {
class createRoom;
}

class createRoom : public QWidget
{
    Q_OBJECT

public:
    explicit createRoom(QWidget *parent = nullptr);
    ~createRoom();
    QTcpSocket *socket;
    QString nickname;

private slots:
    void on_pushButton_clicked();

private:
    Ui::createRoom *ui;
    void getRoomData();
};

#endif // CREATEROOM_H
