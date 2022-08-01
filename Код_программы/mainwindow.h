#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>
#include <QVector>
#include <QPointF>
#include <QSqlTableModel>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"
#include "QTableWidgetItem"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    double G_L;
    double G_W;
    double G_H;

    double U_V;
    double U_T;

    double P_Z;

    double P_P;
    double P_C;
    double P_T;

    double K_U;
    double K_B;
    double K_N;
    double K_T;
    double K_A;


    bool stop_rachet = true;

    int table_number_toltip = 1;

    int progress_int = 0;


    QtCharts::QChartView *chartView;

    QtCharts::QSplineSeries *seriesA = new QtCharts::QSplineSeries();
    QtCharts::QSplineSeries *seriesB = new QtCharts::QSplineSeries();

    QtCharts::QChartView *chartView_exp;

    QtCharts::QSplineSeries *seriesA_exp = new QtCharts::QSplineSeries();
    QtCharts::QSplineSeries *seriesB_exp = new QtCharts::QSplineSeries();
    QtCharts::QSplineSeries *seriesC_exp = new QtCharts::QSplineSeries();


    QVector<double> vec_m;
    QVector<double> vec_t;
    QVector<double> vec_u;

    QVector<QString> material_tooltop;
    QVector<QString> parametr_tooltop;

    QStringList list;
    QSqlDatabase db;
    QSqlDatabase db_person;
    QSqlTableModel *model;
    QStringList list_bd;

//    QVector<double> vec_v_exp;
    QVector<double> vec_t_exp;

private slots:
    void ToolTip_end(QModelIndex item);

    void on_pushButton_clicked();

    bool on_lineEdit_textChanged(const QString &arg1);

    bool on_lineEdit_2_textChanged(const QString &arg1);

    bool on_lineEdit_3_textChanged(const QString &arg1);

    bool on_lineEdit_4_textChanged(const QString &arg1);

    bool on_lineEdit_5_textChanged(const QString &arg1);

    bool on_lineEdit_15_textChanged(const QString &arg1);

    bool on_lineEdit_6_textChanged(const QString &arg1);

    bool on_lineEdit_7_textChanged(const QString &arg1);

    bool on_lineEdit_8_textChanged(const QString &arg1);

    bool on_lineEdit_9_textChanged(const QString &arg1);

    bool on_lineEdit_10_textChanged(const QString &arg1);

    bool on_lineEdit_11_textChanged(const QString &arg1);

    bool on_lineEdit_12_textChanged(const QString &arg1);

    bool on_lineEdit_13_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_otchet();
    void n_otchet_progress();
    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void select_buttons();

    void on_pushButton_2_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_8_clicked();

    void update_model();
    void update_model2();
    void slotEditRecord(QModelIndex index);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_comboBox_activated(const QString &arg1);

    void experement(double, double, double, double, double, bool,bool);



    void on_pushButton_15_clicked();

    void on_pushButton_18_clicked();



    void on_pushButton_20_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
