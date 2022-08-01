#ifndef MY_EXCEL_THREAD_H
#define MY_EXCEL_THREAD_H

#include<QObject>
#include <QVector>

class My_Excel_Thread:public QObject
{
    Q_OBJECT
public:
    My_Excel_Thread(QString, double, double, double,double,double,double,double,double,double,
                    double,double,double,double,double, QVector<double>,QVector<double>,QVector<double>, QString, QString, QString,QString);

    QString file; double G_L; double G_W; double G_H; double U_V; double U_T; double P_Z; double P_P; double P_C; double P_T;
     double K_U; double K_B; double K_N; double K_T; double K_A; QVector<double> vec_m; QVector<double> vec_t; QVector<double> vec_u;
     QString combo; QString proi; QString tem; QString vyz;
public slots:
    void doWork();

signals:
    void finish();
    void on_progress();

};

#endif // MY_EXCEL_THREAD_H
