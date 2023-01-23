#ifndef COUNTING_H
#define COUNTING_H

#include <QWidget>

namespace Ui {
class counting;
}

class counting : public QWidget
{
    Q_OBJECT

public:
    int number;
    explicit counting(QWidget *parent = nullptr);
    ~counting();

    void setValue(int value);

private:
    Ui::counting *ui;
};

#endif // COUNTING_H
