#ifndef DIALOG_VALUE_H
#define DIALOG_VALUE_H

#include <QDialog>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"


namespace Ui {
class Dialog_value;
}

class Dialog_value : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_value(QStringList list_h, QStringList list_present,QStringList list_bd,QSqlDatabase db, QWidget *parent = nullptr);
    ~Dialog_value();
    QStringList list_BD;
    QSqlDatabase db2;
    int id_delete;

signals:
    void close_dialog();
private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void on_comboBox_2_activated(const QString &arg1);

private:
    Ui::Dialog_value *ui;
};

#endif // DIALOG_VALUE_H
