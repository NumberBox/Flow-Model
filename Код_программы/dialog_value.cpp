#include "dialog_value.h"
#include "ui_dialog_value.h"

Dialog_value::Dialog_value(QStringList list_h,QStringList list_present,QStringList list_bd,QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_value)
{
    ui->setupUi(this);
    list_BD = list_bd;
    db2 = db;

    ui->comboBox->clear();
    ui->comboBox_2->clear();

    db2.open();
    QSqlQuery query;
//    query.exec("SELECT name FROM PARAMETR");
//    while (query.next()){
//        ui->comboBox->addItem(query.value(0).toString());
//    }
    query.exec("SELECT name FROM MATERIAL");
    while (query.next()){
        ui->comboBox_2->addItem(query.value(0).toString());
    }
    query.exec("SELECT name FROM PARAMETR");
    while (query.next()){
        ui->comboBox->addItem(query.value(0).toString());
    }
    //this->on_comboBox_2_activated(ui->comboBox_2->currentText());


    db2.close();


        if(list_present.size()>0){
            id_delete = list_present[0].toInt();
            ui->lineEdit->setText(list_present[0]);
            ui->lineEdit_3->setText(list_present[1]);
            //ui->lineEdit_4->setText(list_present[2]);
            //ui->lineEdit_5->setText(list_present[3]);
            ui->pushButton_3->setEnabled(true);

            db2.open();
            QSqlQuery query;
            query.exec("SELECT name FROM PARAMETR WHERE id = "+list_present[2]);
            while (query.next()){
                ui->comboBox->setCurrentText(query.value(0).toString());
            }
            query.exec("SELECT name FROM MATERIAL WHERE id = "+list_present[3]);
            while (query.next()){
                ui->comboBox_2->setCurrentText(query.value(0).toString());
            }
            db2.close();

        }
        else{
            db2.open();
            QSqlQuery query;
            query.exec("SELECT id FROM VALUE ORDER BY id DESC LIMIT 1");
            while (query.next()){
                ui->lineEdit->setText(QString::number(query.value(0).toInt()+1));
            }
            db2.close();
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_2->setText("Добавить");
        }

}

Dialog_value::~Dialog_value()
{
    delete ui;
}

void Dialog_value::on_pushButton_clicked()
{
    this->close();
}

void Dialog_value::on_pushButton_2_clicked()
{
    db2.open();
    QSqlQuery a_query;

        if(ui->pushButton_2->text()!="Добавить"){
            a_query.exec("DELETE FROM "+list_BD[0]+" WHERE "+list_BD[1]+" = "+QString::number(id_delete)+"");
        }

        QString str = " (";
        for (int i = 1;i<list_BD.size();i++) {
            if(i==list_BD.size()-1){
              str+=list_BD[i]+")";
            }
            else
                str+=list_BD[i]+", ";
        }
        str+="VALUES (";
        for (int i = 1;i<list_BD.size();i++) {
            if(i==list_BD.size()-1){
              str+="?)";
            }
            else
                str+="?, ";
        }

        a_query.prepare("INSERT INTO "+list_BD[0]+str);
            a_query.addBindValue(ui->lineEdit->text());//
            a_query.addBindValue(ui->lineEdit_3->text());
            //a_query.addBindValue(ui->lineEdit_4->text());

            QSqlQuery query;
            query.exec("SELECT id FROM PARAMETR WHERE name = \""+ui->comboBox->currentText()+"\"");
            while (query.next()){
                a_query.addBindValue(query.value(0).toString());
            }

            //a_query.addBindValue(ui->lineEdit_5->text());
            query.exec("SELECT id FROM MATERIAL WHERE name = \""+ui->comboBox_2->currentText()+"\"");
            while (query.next()){
                a_query.addBindValue(query.value(0).toString());
            }

    a_query.exec();

    db2.close();
    emit close_dialog();
    this->close();
}

void Dialog_value::on_pushButton_3_clicked()
{
    db2.open();
    QSqlQuery a_query;
    a_query.exec("DELETE FROM "+list_BD[0]+" WHERE "+list_BD[1]+" = "+QString::number(id_delete)+"");
    db2.close();
    emit close_dialog();
    this->close();
}

void Dialog_value::on_comboBox_2_activated(const QString &arg1)
{
//    ui->comboBox->clear();
//    db2.open();
//    QSqlQuery query;
//    query.exec("SELECT id FROM MATERIAL WHERE MATERIAL.name = \""+arg1+"\"");
//    int id  = 0;
//    while (query.next()){
//        id = query.value(0).toInt();
//    }
//    query.exec("SELECT name FROM PARAMETR WHERE PARAMETR.material_id = "+QString::number(id));
//    while (query.next()){
//        ui->comboBox->addItem(query.value(0).toString());
//    }
//    db2.close();
}
