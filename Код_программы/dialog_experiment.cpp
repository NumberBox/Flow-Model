#include "dialog_experiment.h"
#include "ui_dialog_experiment.h"
#include <QDebug>
#include <QMessageBox>

dialog_experiment::dialog_experiment(double t, double v,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_experiment)
{
    ui->setupUi(this);
    T=t;
    V=v;
    this->on_radioButton_clicked();
}

dialog_experiment::~dialog_experiment()
{
    delete ui;
}

void dialog_experiment::on_pushButton_clicked()
{
    if(
    this->on_lineEdit_textChanged(ui->lineEdit->text())&&
    this->on_lineEdit_18_textChanged(ui->lineEdit_18->text())&&
    this->on_lineEdit_19_textChanged(ui->lineEdit_19->text())&&
    this->on_lineEdit_21_textChanged(ui->lineEdit_21->text())&&
    this->on_lineEdit_20_textChanged(ui->lineEdit_20->text())){

    }else{
        qDebug()<<"no";
        QMessageBox::information(this,"Ошибка","Проверьте правильность входных данных");
        return;
    }



    double v1 = ui->lineEdit_18->text().toDouble();
    double v2 = ui->lineEdit_19->text().toDouble();
    double t1 = ui->lineEdit_21->text().toDouble();
    double t2 = ui->lineEdit_20->text().toDouble();
    double tt = ui->lineEdit->text().toDouble();
    bool o1 = ui->radioButton->isChecked();
    bool o2 = ui->radioButton_2->isChecked();

    emit experement(v1,v2,t1,t2,tt,o1,o2);
    this->close();
}

bool dialog_experiment::on_lineEdit_18_textChanged(const QString &arg1)
{
    bool check;
    double U_T = arg1.toDouble(&check);
    if(U_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_18->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_18->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool dialog_experiment::on_lineEdit_19_textChanged(const QString &arg1)
{
    bool check;
    double U_T = arg1.toDouble(&check);
    if(U_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_19->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_19->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool dialog_experiment::on_lineEdit_21_textChanged(const QString &arg1)
{
    bool check;
    double U_T = arg1.toDouble(&check);
    if(U_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_21->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_21->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool dialog_experiment::on_lineEdit_20_textChanged(const QString &arg1)
{
    bool check;
    double U_T = arg1.toDouble(&check);
    if(U_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_20->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_20->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool dialog_experiment::on_lineEdit_textChanged(const QString &arg1)
{
    bool check;
    double U_T = arg1.toDouble(&check);
    if(U_T<=0)
        check = false;
    if(!check){
        ui->lineEdit->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

void dialog_experiment::on_radioButton_clicked()//скорость
{
//    qDebug()<<"ss";
    ui->lineEdit_20->setEnabled(false);
    ui->lineEdit_21->setEnabled(false);
    ui->lineEdit_18->setEnabled(true);
    ui->lineEdit_19->setEnabled(true);
//    ui->label_44->setEnabled(false);
//    ui->label_45->setEnabled(false);
//    ui->label_46->setEnabled(false);
//    ui->label_41->setEnabled(true);
//    ui->label_42->setEnabled(true);
//    ui->label_43->setEnabled(true);
//    ui->lineEdit_18->setText("0.5");
//    ui->lineEdit_19->setText("2.5");
//    ui->lineEdit_20->setText(QString::number(T));
//    ui->lineEdit_21->setText(QString::number(T));
}

void dialog_experiment::on_radioButton_2_clicked()//tem
{
    ui->lineEdit_20->setEnabled(true);
    ui->lineEdit_21->setEnabled(true);
//    ui->label_44->setEnabled(true);
//    ui->label_45->setEnabled(true);
//    ui->label_46->setEnabled(true);
    ui->lineEdit_18->setEnabled(false);
    ui->lineEdit_19->setEnabled(false);
//    ui->label_41->setEnabled(false);
//    ui->label_42->setEnabled(false);
//    ui->label_43->setEnabled(false);
//    ui->lineEdit_20->setText("180");
//    ui->lineEdit_21->setText("220");
//    ui->lineEdit_18->setText(QString::number(V));
//    ui->lineEdit_19->setText(QString::number(V));
}
