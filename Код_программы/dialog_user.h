#ifndef DIALOG_USER_H
#define DIALOG_USER_H

#include <QDialog>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"

namespace Ui {
class Dialog_user;
}

class Dialog_user : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_user(QStringList list_h, QStringList list_present,QStringList list_bd,QSqlDatabase db, QWidget *parent = nullptr);
    ~Dialog_user();

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
    Ui::Dialog_user *ui;
};

#endif // DIALOG_USER_H
