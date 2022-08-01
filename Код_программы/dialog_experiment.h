#ifndef DIALOG_EXPERIMENT_H
#define DIALOG_EXPERIMENT_H

#include <QDialog>

namespace Ui {
class dialog_experiment;
}

class dialog_experiment : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_experiment(double,double,QWidget *parent = nullptr);
    ~dialog_experiment();

    double T;
    double V;

signals:
    void experement(double, double, double, double, double, bool,bool);
private slots:
    void on_pushButton_clicked();


    bool on_lineEdit_18_textChanged(const QString &arg1);

    bool on_lineEdit_19_textChanged(const QString &arg1);

    bool on_lineEdit_21_textChanged(const QString &arg1);

    bool on_lineEdit_20_textChanged(const QString &arg1);

    bool on_lineEdit_textChanged(const QString &arg1);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::dialog_experiment *ui;
};

#endif // DIALOG_EXPERIMENT_H
