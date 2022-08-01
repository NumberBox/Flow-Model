#ifndef DIALOG_MATERIAL_H
#define DIALOG_MATERIAL_H

#include <QDialog>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"

namespace Ui {
class Dialog_material;
}

class Dialog_material : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_material(QStringList list_h, QStringList list_present,QStringList list_bd,QSqlDatabase db, QWidget *parent = nullptr);
    ~Dialog_material();
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
    Ui::Dialog_material *ui;
};

#endif // DIALOG_MATERIAL_H
