#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdlib.h>
#include <QStringList>
#include <fstream>
#include <string>
#include <QTableWidgetItem>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
signals:
    void sendMesage(QString);
    void sendStatMod(QString);

public slots:
    void resiveMesage(QString arg);
    void resiveStatMod(QString arg);
    void insmod();
    void rmmod();
    void update();
    void killAsPID();
    void killAsName();
    void addbl();
    void rmbl();
    void modExist();
    
private:
    Ui::Widget *ui;
    int exist;
    int lockSlot;
};

#endif // WIDGET_H
