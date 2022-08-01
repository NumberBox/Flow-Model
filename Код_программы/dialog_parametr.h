#ifndef DIALOG_PARAMETR_H
#define DIALOG_PARAMETR_H

#include <QDialog>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"

namespace Ui {
class Dialog_Parametr;
}

class Dialog_Parametr : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Parametr(QStringList list_h, QStringList list_present,QStringList list_bd,QSqlDatabase db, QWidget *parent = nullptr);
    ~Dialog_Parametr();
    QStringList list_BD;
    QSqlDatabase db2;
    int id_delete;

signals:
    void close_dialog();
private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
private:
    Ui::Dialog_Parametr *ui;
};

#endif // DIALOG_PARAMETR_H
