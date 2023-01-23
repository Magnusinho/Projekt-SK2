#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class lobby;
}

class lobby : public QWidget
{
    Q_OBJECT

public:
    explicit lobby(QWidget *parent = nullptr);
    ~lobby();
    void updatePlayerList();
    QTcpSocket *socket;
    QVector<QString> playersVec;
    QString playerRule;
    int roomID;


    void setPlayerRule(const QString &value);

    void setRoomID(int value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::lobby *ui;
};

#endif // LOBBY_H
