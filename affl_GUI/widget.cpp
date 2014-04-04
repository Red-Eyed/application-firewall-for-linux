#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);





    //=============================load table===============================================
    QStringList lst,lstbl;
    lst<<"PID"<<"Process Name";
    lstbl<<"Process Name";
    ui->tableWidget->setHorizontalHeaderLabels(lst);
    ui->tableWidget_2->setHorizontalHeaderLabels(lstbl);
    //connect(ui->tableWidget,SIGNAL(cellEntered(int,int)),this,SLOT(changeloltest(int,int)));
    //======================================================================================


    //=============================load buttons=============================================
    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(update()));
    connect(ui->pushButton_2,SIGNAL(pressed()),this,SLOT(killAsPID()));
    connect(ui->pushButton_7,SIGNAL(pressed()),this,SLOT(killAsName()));
    connect(ui->pushButton_3,SIGNAL(pressed()),this,SLOT(addbl()));
    connect(ui->pushButton_4,SIGNAL(pressed()),this,SLOT(rmbl()));
    //======================================================================================

    //=============================connect==================================================
    connect(this,SIGNAL(sendMesage(QString)),this,SLOT(resiveMesage(QString)));
    connect(this,SIGNAL(sendStatMod(QString)),this,SLOT(resiveStatMod(QString)));
    //======================================================================================

    //===================default config=====================================================
    exist=0;
    lockSlot=0;
    insmod();
    //======================================================================================
    //===================set BL=============================================================
    int cntProc=-1,i=0;
    char nameProc[50];
    char charAnsver[50];
    QString ansver;
    std::fstream fib("bl",std::ios_base::in);
    fib>>cntProc;
    fib.get();
    if(cntProc==-1)
    {
        std::fstream fob("bl",std::ios_base::out);
        fob<<'0';
        fob.close();
    }
    else
    {
        ui->tableWidget_2->setRowCount(cntProc);
        for(i=i;i<cntProc;i++)
        {
            fib.getline(nameProc,50);
            QString command="addProc@";
            command+=QString(nameProc);
            command+="#";
            std::fstream fi("/dev/affl_comm",std::ios_base::out);
            fi<<command.toStdString();
            fi.close();
            //====resive========================

            std::fstream fo("/dev/affl_comm",std::ios_base::in);
            fo.getline(charAnsver,50);
            ansver=QString(charAnsver);
            if(ansver==command)
            {
                emit sendMesage("Comand addProc didn't execute");
                emit sendStatMod("Disable");
            }

            if(ansver=="0")
            {
                ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(nameProc));
                emit sendMesage("Comand addProc is execute");
            }
            fo.close();
        }
    }
    //======================================================================================
    emit sendMesage("Start System");
}

Widget::~Widget()
{
    int cntProc=-1,i=0;
    char nameProc[50];
    char c;

    QString command="getBL@";
    std::fstream fi("/dev/affl_comm",std::ios_base::out);
    fi<<command.toStdString();
    fi.close();

    std::fstream fo("/dev/affl_comm",std::ios_base::in);
    fo>>cntProc;

    if(cntProc==-1)
    {
        emit sendMesage("Comand getBL didn't execute");
        emit sendStatMod("Disable");
    }
    else
    {
        std::fstream fob("bl",std::ios_base::out);
        fob<<cntProc;
        fo>>c;
        fob<<c;
        for(i=i;i<cntProc;++i)
        {
            fo>>nameProc;
            fob<<nameProc;
        }
        fob.close();
        emit sendMesage("Comand getBL is execute");
    }
    fo.close();




    rmmod();
    emit sendMesage("Finish System");
    delete ui;
}

void Widget::insmod()
{
    if(lockSlot==0)
    {
        lockSlot=1;

        system("insmod /home/orest/my_dev/affl_release/trunk/release/affl_mod.ko");
        exist=1;

        emit sendStatMod("Enable");
        lockSlot=0;
    }
}

void Widget::rmmod()
{
    if(lockSlot==0)
    {
        lockSlot=1;

        system("rmmod affl_mod");
        exist=1;

        emit sendStatMod("Disable");
        lockSlot=0;
    }
}

void Widget::update()
{

    if(lockSlot==0)
    {
        lockSlot=1;

        //=====send========================
        QString command="view@";

        std::fstream fi("/dev/affl_comm",std::ios_base::out);
        fi<<command.toStdString();
        fi.close();
        //====resive========================
        int cntProc=0;
        int PID;
        char procName[50];

        std::fstream fo("/dev/affl_comm",std::ios_base::in);
        fo>>cntProc;
        if(cntProc==0)
        {
            emit sendMesage("Comand view didn't execute");
            emit sendStatMod("Disable");
            exist=0;
        }
        else
        {
            emit sendMesage("Comand view is execute");
            ui->tableWidget->setRowCount(cntProc);
            for(int i=0;i<cntProc;i++)
            {
                fo>>PID;
                fo.get();
                fo.getline(procName,50);
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString("%1").arg(PID)));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(procName)));
            }
        }
        fo.close();
        lockSlot=0;
    }
}

void Widget::killAsPID()
{
    if(lockSlot==0)
    {
        lockSlot=1;
        if(ui->tableWidget->currentColumn()==-1 || ui->tableWidget->currentRow()==-1)
        {
            emit sendMesage("Please allocation process");
        }
        else
        {
            //=====send========================
            QString command="kill@";
            command+=ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text();
            command+="#%";
            command+=ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text();

            std::fstream fi("/dev/affl_comm",std::ios_base::out);
            fi<<command.toStdString();
            fi.close();

            //====resive========================
            char charAnsver[50];
            QString ansver;

            std::fstream fo("/dev/affl_comm",std::ios_base::in);
            fo.getline(charAnsver,50);
            ansver=QString(charAnsver);
            if(ansver==command)
            {
                emit sendMesage("Comand killAsPid didn't execute");
                emit sendStatMod("Disable");
            }

            if(ansver=="0")
            {
                emit sendMesage("Comand killAsPid is execute");
            }
            if(ansver=="-1")
            {
                ui->label_6->setText("Invalid process");
            }

        }
        lockSlot=0;
    }
}

void Widget::killAsName()
{
    if(lockSlot==0)
    {
        lockSlot=1;
        if(ui->tableWidget->currentColumn()==-1 || ui->tableWidget->currentRow()==-1)
        {
            emit sendMesage("Please allocation process");
        }
        else
        {
            //=====send========================
            QString command="kill@";
            command+=ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text();
            command+="#%-1";

            std::fstream fi("/dev/affl_comm",std::ios_base::out);
            fi<<command.toStdString();
            fi.close();

            //====resive========================
            char charAnsver[50];
            QString ansver;

            std::fstream fo("/dev/affl_comm",std::ios_base::in);
            fo.getline(charAnsver,50);
            ansver=QString(charAnsver);
            if(ansver==command)
            {
                emit sendMesage("Comand killAsName didn't execute");
                emit sendStatMod("Disable");
            }

            if(ansver=="0")
            {
                emit sendMesage("Comand killAsName is execute");
            }
            if(ansver=="-1")
            {
                ui->label_6->setText("Invalid process");
            }

        }
        lockSlot=0;
    }
}

void Widget::addbl()
{
    if(lockSlot==0)
    {
        lockSlot=1;
        //=====send========================
        QString command="addProc@";
        std::fstream fi("/dev/affl_command2",std::ios_base::out);
        fi<<command.toStdString();
        fi.close();
        //====resive========================
        char charAnsver[50];
        QString ansver;

        std::fstream fo("/dev/affl_command3",std::ios_base::in);
        fo.getline(charAnsver,50);
        ansver=QString(charAnsver);
        if(ansver==command)
        {
            ui->label_4->setText("Module: Disable");
            ui->label_6->setText("Module disable");
            exist=0;
        }

        if(ansver=="0")
        {

            ui->label_6->setText("WellDone");
            exist=1;
        }

        lockSlot=0;
    }

}

void Widget::rmbl()
{
    if(lockSlot==0)
    {
        lockSlot=1;



        lockSlot=0;
    }
}

void Widget::modExist()
{
    //=====send========================
    QString command="exist@";

    std::fstream fi("/dev/affl_command4",std::ios_base::out);
    fi<<command.toStdString();
    fi.close();

    //====resive========================
    char charAnsver[50];
    QString ansver;

    std::fstream fo("/dev/affl_command4",std::ios_base::in);
    fo.getline(charAnsver,50);
    ansver=QString(charAnsver);
    if(ansver==command)
    {
        ui->label_4->setText("Module: Disable");
        ui->label_6->setText("Module disable");
        exist=0;
    }

    if(ansver=="0")
    {
        ui->label_4->setText("Module: Enable");
        ui->label_6->setText("WellDone");
        exist=1;
    }

}


void Widget::resiveMesage(QString arg)
{
    ui->label_6->setText(arg);
}

void Widget::resiveStatMod(QString arg)
{
    ui->label_9->setText(arg);
}
