#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <math.h>
#include <QVector>
#include <qaxobject.h>
#include <qaxwidget.h>
#include <qapplication.h>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>
#include <QTime>
#include <QMessageBox>
#include "my_excel_thread.h"
#include <QThread>
#include <dialog_user.h>
#include <dialog_material.h>
#include <dialog_parametr.h>
#include <dialog_value.h>
#include <QToolTip>
#include "Windows.h"
#include "Psapi.h"
#include <QFileInfo>
#include "dialog_experiment.h"

QtCharts::QChart *chart_;
QtCharts::QValueAxis *axisX;
QtCharts::QValueAxis *axisY;
QGraphicsSimpleTextItem *m_coordX;
QGraphicsSimpleTextItem *m_coordY;
QGraphicsSimpleTextItem *m_coordX_exp;
QGraphicsSimpleTextItem *m_coordY_exp;
bool t_or_n;
int p_t_n;
bool o_exp;

QtCharts::QChart *chart__exp;
QtCharts::QValueAxis *axisX_exp;
QtCharts::QValueAxis *axisY_exp;

MainWindow::MainWindow(QWidget *parent)//, прогрес бар, внутрение переменные
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->progressBar->hide();
    ui->groupBox_7->hide();
    ui->groupBox_8->hide();
    ui->groupBox->hide();
    ui->groupBox_11->hide();
    ui->groupBox_10->show();

    chartView  = new QtCharts::QChartView();
    chartView_exp  = new QtCharts::QChartView();
    chart_ = new QtCharts::QChart();
    chart__exp = new QtCharts::QChart();
    chartView->setChart(chart_);
    chartView_exp->setChart(chart__exp);
    axisX = new QtCharts::QValueAxis();
    axisY = new QtCharts::QValueAxis();
    axisX_exp = new QtCharts::QValueAxis();
    axisY_exp = new QtCharts::QValueAxis();
    chart_->addAxis(axisY, Qt::AlignLeft);
    chart_->addAxis(axisX, Qt::AlignBottom);
    chart__exp->addAxis(axisY_exp, Qt::AlignLeft);
    chart__exp->addAxis(axisX_exp, Qt::AlignBottom);
//    chart->setTitle("Моделирование сигнала");
//    axisX->setTitleText("Время");
//    axisY->setTitleText("y");
    axisY->setTickCount(10);
    axisX->setTickCount(16);

    axisY_exp->setTickCount(10);
    axisX_exp->setTickCount(10);

    chart_->legend()->setVisible(false);
    chart__exp->legend()->setVisible(false);
    ui->gridLayout_7->addWidget(chartView);
    ui->gridLayout_15->addWidget(chartView_exp);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView_exp->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout_7->setMargin(0);
    ui->gridLayout_15->setMargin(0);
    m_coordX = new QGraphicsSimpleTextItem(chart_);
    m_coordY = new QGraphicsSimpleTextItem(chart_);
    m_coordX->setZValue(100);
    m_coordY->setZValue(100);
    m_coordX->setOpacity(0.5);
    m_coordY->setOpacity(0.5);

    m_coordX_exp = new QGraphicsSimpleTextItem(chart__exp);
    m_coordY_exp = new QGraphicsSimpleTextItem(chart__exp);
    m_coordX_exp->setZValue(100);
    m_coordY_exp->setZValue(100);
    m_coordX_exp->setOpacity(0.5);
    m_coordY_exp->setOpacity(0.5);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord(QModelIndex)));
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setMouseTracking(true);
    ui->tableView->setStyleSheet("QTableView {"
                                    "color: rgb(0, 0, 0);"
                                    "selection-background-color: rgb(220, 236, 255);"
                                    "background-color: rgb(255, 255, 255);"
                                    "selection-color: rgb(0, 0, 0);"
                                 "}"
                                 "QTableView::item:hover {"
                                     "background-color: rgb(207, 255, 255);"
                                 "}"
                                 "QHeaderView {"
                                    "background-color: rgb(220, 236, 255);"
                                    "font: italic;"
                                 "}"
                                 "QHeaderView::section:horizontal { "
                                    "color: rgb(0, 0, 0);"
                                    "font-style: italic;"
                                    "font-weight: bold;"
                                    "background-color: rgb(220, 236, 255);"
                                 "}");
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    connect(ui->tableView, SIGNAL(entered(QModelIndex)), this, SLOT(ToolTip_end(QModelIndex)));


    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setStyleSheet("QTableView { background-color: rgb(255, 255, 255); }"
                                   "QHeaderView::section:horizontal { "
                                    "color: rgb(0, 0, 0);"
                                  //  "font-size:14px;"
                                    "font: italic;"
                                   "border: 1px solid #000000;"
                                    "background-color: rgb(220, 236, 255);"

                                     "}");


    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Координата по длине\nканала, м"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Температура, °С"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Вязкость, Па·с"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()/3);
    ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()/3);
    ui->tableWidget->setColumnWidth(2,ui->tableWidget->width()/3);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QFile file("flowmodel.db3");
        if((!file.exists())||(!file.open(QIODevice::ReadOnly))){
            if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
                    file.close();
                    db = QSqlDatabase::addDatabase("QSQLITE");
                    db.setDatabaseName("flowmodel.db3");
                    db.open();
                    QMessageBox::warning(this,"Warning", "Отсутствует база данных\nДальнейшее использование невозможно");
                    db.close();
            }
        }
        else{
              file.close();
              db = QSqlDatabase::addDatabase("QSQLITE");
              db.setDatabaseName("flowmodel.db3");
              //db.open();
        }
     QFile file2("flowmodel_user.db3");
        if((!file2.exists())||(!file2.open(QIODevice::ReadOnly))){
            if(file2.open(QIODevice::WriteOnly | QIODevice::Text)){
                    file2.close();
//                    db_person = QSqlDatabase::addDatabase("QSQLITE");
//                    db_person.setDatabaseName("flowmodel_user.db3");
//                    db_person.open();
                    QMessageBox::warning(this,"Warning", "Отсутствует база данных о пользователях\nДальнейшее использование невозможно");
//                    db_person.close();
            }
        }
        else{
              file2.close();
        }

        db.setDatabaseName("flowmodel.db3");
        db.open();
        QSqlQuery query;
        query.exec("SELECT id, name FROM MATERIAL");
        while (query.next()){
            material_tooltop.push_back(query.value(0).toString());
            material_tooltop.push_back(query.value(1).toString());
        }
        material_tooltop.push_back(QString::number(0));


        query.exec("SELECT id, name FROM PARAMETR");
        while (query.next()){
            parametr_tooltop.push_back(query.value(0).toString());
            parametr_tooltop.push_back(query.value(1).toString());
        }
        parametr_tooltop.push_back(QString::number(0));

//        query.exec("SELECT id, name FROM PARAMETR");
//        while (query.next()){
//            parametr_tooltop[query.value(0).toInt()] = query.value(1).toString();
//        }
//       port_tooltop[0]="0";
///
        db.close();



        QFile file_backup("backup_db/flowmodel.db3");
            if(file_backup.exists()){
                QFileInfo info_backup(file_backup);
                QDateTime cur_date;
                cur_date = cur_date.currentDateTime();


                if(cur_date.daysTo(info_backup.birthTime())<=-5){
                    QFile::remove("backup_db/flowmodel.db3");
                    QFile::remove("backup_db/flowmodel_user.db3");
                    QFile::copy("flowmodel.db3","backup_db/flowmodel.db3");
                    QFile::copy("flowmodel_user.db3","backup_db/flowmodel_user.db3");
                    ui->label_52->setText("Сегодня было произведено резервное\nкопирование базы данных");
                    qDebug()<<"Произведен бакап";
                }else{
                    if(5+cur_date.daysTo(info_backup.birthTime())==1){
                         ui->label_52->setText("До резервного копирования\nбазы данных осталось "+QString::number(5+cur_date.daysTo(info_backup.birthTime()))+" день");
                    }else if(5+cur_date.daysTo(info_backup.birthTime())==5){
                         ui->label_52->setText("До резервного копирования\nбазы данных осталось "+QString::number(5+cur_date.daysTo(info_backup.birthTime()))+" дней");
                    }else
                         ui->label_52->setText("До резервного копирования\nбазы данных осталось "+QString::number(5+cur_date.daysTo(info_backup.birthTime()))+" дня");

                    qDebug()<<"До бакапа "<<5+cur_date.daysTo(info_backup.birthTime())<<" дней";
                }

            }
            else{
                QFile::copy("flowmodel.db3","backup_db/flowmodel.db3");
                QFile::copy("flowmodel_user.db3","backup_db/flowmodel_user.db3");
            }





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ToolTip_end(QModelIndex item)
{
    QToolTip::hideText();
    QPoint a = QCursor::pos();
//    if(table_number_toltip==2){
//        if(item.column()==4){
//            QString s = material_tooltop.at(ui->tableView->model()->data(ui->tableView->model()->index(item.row(),item.column())).toInt());
//            QToolTip::showText(a,s);
//        }
//    }
    if(table_number_toltip==3){
        if(item.column()==3){
            QString s;
            for (int i = 0;i<material_tooltop.size();i++) {
               if (material_tooltop.at(i)==ui->tableView->model()->data(ui->tableView->model()->index(item.row(),item.column())).toString()){
                   s = material_tooltop.at(i+1);
                   break;
               }
            }
            //QString s = material_tooltop.at(ui->tableView->model()->data(ui->tableView->model()->index(item.row(),item.column())).toInt());
            QToolTip::showText(a,s);
        }
        if(item.column()==2){
            QString s;
            for (int i = 0;i<parametr_tooltop.size();i++) {
               if (parametr_tooltop.at(i)==ui->tableView->model()->data(ui->tableView->model()->index(item.row(),item.column())).toString()){
                   s = parametr_tooltop.at(i+1);
                   break;
               }
            }
            // = parametr_tooltop.at(ui->tableView->model()->data(ui->tableView->model()->index(item.row(),item.column())).toInt());
            QToolTip::showText(a,s);
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
   ui->groupBox->hide();
   ui->groupBox_10->show();
}

void MainWindow::on_pushButton_3_clicked()//растчет
{
    if(
    this->on_lineEdit_textChanged(ui->lineEdit->text())&&
    this->on_lineEdit_2_textChanged(ui->lineEdit_2->text())&&
    this->on_lineEdit_3_textChanged(ui->lineEdit_3->text())&&
    this->on_lineEdit_4_textChanged(ui->lineEdit_4->text())&&
    this->on_lineEdit_5_textChanged(ui->lineEdit_5->text())&&
    this->on_lineEdit_6_textChanged(ui->lineEdit_6->text())&&
    this->on_lineEdit_7_textChanged(ui->lineEdit_7->text())&&
    this->on_lineEdit_8_textChanged(ui->lineEdit_8->text())&&
    this->on_lineEdit_9_textChanged(ui->lineEdit_9->text())&&
    this->on_lineEdit_10_textChanged(ui->lineEdit_10->text())&&
    this->on_lineEdit_11_textChanged(ui->lineEdit_11->text())&&
    this->on_lineEdit_12_textChanged(ui->lineEdit_12->text())&&
    this->on_lineEdit_13_textChanged(ui->lineEdit_13->text())&&
    this->on_lineEdit_15_textChanged(ui->lineEdit_15->text())){

    }else{
        qDebug()<<"no";
        QMessageBox::information(this,"Ошибка","Проверьте правильность входных данных");
        return;
    }


//    if(!stop_rachet){

//    }
    QTime time;
    time.start();
    ui->groupBox->hide();
    ui->groupBox_7->show();




    double F = 0.125*((G_H/G_W)*(G_H/G_W))-0.625*(G_H/G_W)+1.0;//коэффициент геометрической формы канала
    double Gamma_point = U_V/G_H;//скорость деформации сдвига
    double q_Gamma_point = G_W*G_H*K_U*pow(Gamma_point,K_N+1.0); //удельные тепловые потоки
    double q_Alpha = G_W*K_A*((1.0/K_B)-U_T+K_T);
    double Q_ch = (G_W*G_H*U_V)/2*F;//расход потока
    double M = G_L/P_Z;//расчет числа шагов
    if(q_Alpha==0.0){
        q_Alpha = 1.0;
    }

    qDebug()<<F<<Gamma_point<<q_Gamma_point<<q_Alpha<<Q_ch<<M;

    for(int i = 0;i<=M;i++){
        double z_i = i*P_Z; vec_m.push_back(z_i);//координата по длине канала
        double ae_i = ((K_B*q_Gamma_point+G_W*K_A)/(K_B*q_Alpha))*(1.0-exp((-z_i*K_B*q_Alpha)/(P_P*P_C*Q_ch)))+exp(K_B*(P_T-K_T-((z_i*q_Alpha)/(P_P*P_C*Q_ch))));

        double t_i = K_T+(1/K_B)*log(ae_i); vec_t.push_back(t_i);//floor(t_i*100)/100.0);//температура
        double u = K_U*exp(-K_B*(t_i-K_T));//вязкость
        double nu_i = u*pow(Gamma_point,K_N-1); vec_u.push_back(nu_i);//floor(nu_i*10)/10.0);

        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);//вывод расчетов в таблицу
        ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString::number(z_i)));
        ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QString("%1").arg(t_i, 0, 'f', 2)/*QString::number(floor(t_i*100)/100.0)*/));
        ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(QString("%1").arg(nu_i, 0, 'f', 1)/*QString::number(floor(nu_i*10)/10.0)*/));


    }

//    double z_i = G_L; vec_m.push_back(z_i);
//    double ae_i = ((K_B*q_Gamma_point+G_W*K_A)/(K_B*q_Alpha))*(1-exp((-z_i*K_B*q_Alpha)/(P_P*P_C*Q_ch)))+exp(K_B*(P_T-K_T-((z_i*q_Alpha)/(P_P*P_C*Q_ch))));
//    double t_i = K_T+(1/K_B)*log(ae_i); vec_t.push_back(floor(t_i*100)/100.0);
//    double u = K_U*exp(-K_B*(t_i-K_T));
//    double nu_i = u*pow(Gamma_point,K_N-1); vec_u.push_back(floor(nu_i*10)/10.0);
//    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
//    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString::number(z_i)));
//    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QString("%1").arg(t_i, 0, 'f', 2)/*QString::number(floor(t_i*100)/100.0)*/));
//    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(QString("%1").arg(nu_i, 0, 'f', 1)/*QString::number(floor(nu_i*10)/10.0)*/));

    double Q = Q_ch*P_P*3600;  //производительность



    ui->lineEdit_14->setText(QString("%1").arg(Q, 0, 'f', 0));
    ui->lineEdit_16->setText(QString("%1").arg(vec_t.last(), 0, 'f', 2));
    ui->lineEdit_17->setText(QString("%1").arg(vec_u.last(), 0, 'f', 1));

    for (int i = 0;i<vec_m.size();i++) {
        seriesA->append(vec_m.at(i), vec_t.at(i));
    }
    chart_->addSeries(seriesA);
    seriesA->setColor(QColor(255, 166, 56));
    seriesA->hide();

    for (int i = 0;i<vec_m.size();i++) {
        seriesB->append(vec_m.at(i), vec_u.at(i));
    }
    chart_->addSeries(seriesB);
    seriesB->setColor(QColor(45, 193, 50));
    seriesB->hide();



    this->on_pushButton_4_clicked();

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

    PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)& pmc, sizeof(pmc));
        //SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
        SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

    double a = static_cast<double>(physMemUsedByMe);

    ui->label_34->setText("Расчет произведен за "+QString::number(time.elapsed())+" мс\nИспользовано оперативной памяти "+QString("%1").arg(a/1024/1024, 0, 'f', 2)+" Мб");



}
void MainWindow::on_pushButton_4_clicked()//температура
{
    t_or_n = true;
    ui->pushButton_4->clearFocus();
    ui->pushButton_4->setFont(QFont("MS Shell Dlg 2",8,75));
    ui->pushButton_5->setFont(QFont("MS Shell Dlg 2",8,50));
    seriesA->show();
    seriesB->hide();
    seriesA->attachAxis(axisY);
    seriesA->attachAxis(axisX);

    axisX->setRange(vec_m.first(),vec_m.last());
    if(vec_t.first()<vec_t.last())
        axisY->setRange(vec_t.first(),vec_t.last());
    else
        axisY->setRange(vec_t.last(),vec_t.first());

    axisX->setTitleText("Координата по длине канала, м");
    axisY->setTitleText("Температура, °С");

}

void MainWindow::on_pushButton_5_clicked()
{
    t_or_n = false;
    ui->pushButton_5->clearFocus();
    ui->pushButton_5->setFont(QFont("MS Shell Dlg 2",8,75));
    ui->pushButton_4->setFont(QFont("MS Shell Dlg 2",8,50));
    seriesA->hide();
    seriesB->show();
    seriesB->attachAxis(axisY);
    seriesB->attachAxis(axisX);
    axisX->setRange(vec_m.first(),vec_m.last());
    if(vec_u.first()<vec_u.last())
        axisY->setRange(vec_u.first(),vec_u.last());
    else
        axisY->setRange(vec_u.last(),vec_u.first());
    axisX->setTitleText("Координата по длине канала, м");
    axisY->setTitleText("Вязкость, Па·с");

}


void MainWindow::on_pushButton_6_clicked()
{
    chart_->removeSeries(seriesA);
    chart_->removeSeries(seriesB);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    vec_m.clear();
    vec_t.clear();
    vec_u.clear();
    seriesA->clear();
    seriesB->clear();
    ui->groupBox_7->hide();
    ui->groupBox->show();
}




void QtCharts::QChartView::mouseMoveEvent(QMouseEvent *event){


    /* Setting the mouse position label on the axis from value to position */
        qreal x = (event->pos()).x();
        qreal y = (event->pos()).y();


        qreal xVal = chart_->mapToValue(event->pos()).x();
        qreal yVal = chart_->mapToValue(event->pos()).y();

        qreal xVal_exp = chart__exp->mapToValue(event->pos()).x();
        qreal yVal_exp = chart__exp->mapToValue(event->pos()).y();


        qreal maxX = axisX->max();
        qreal minX = axisX->min();
        qreal maxY = axisY->max();
        qreal minY = axisY->min();

        qreal maxX_exp = axisX_exp->max();
        qreal minX_exp = axisX_exp->min();
        qreal maxY_exp = axisY_exp->max();
        qreal minY_exp = axisY_exp->min();

        if (xVal_exp <= maxX_exp && xVal_exp >= minX_exp && yVal_exp <= maxY_exp && yVal_exp >= minY_exp)
        {
            m_coordX_exp->show();
            m_coordY_exp->show();
            m_coordX_exp->setPos(x+25, y);
            m_coordY_exp->setPos(x+25, y+20);

            /* Displaying value of the mouse on the label */
            if(o_exp){
                 m_coordX_exp->setText(QString("%1%2").arg(xVal_exp, 4, 'f', 2, '0').arg(", м/c"));
            }else{
                m_coordX_exp->setText(QString("%1%2").arg(xVal_exp, 4, 'f', 2, '0').arg(", °С"));
            }

            if(p_t_n==1){
                m_coordY_exp->setText(QString("%1%2").arg(yVal_exp, 4, 'f', 2, '0').arg(", кг/ч"));
            }
            else if(p_t_n==3){
                m_coordY_exp->setText(QString("%1%2").arg(yVal_exp, 4, 'f', 2, '0').arg(", Па·с"));
            }
            else if(p_t_n==2){
                m_coordY_exp->setText(QString("%1%2").arg(yVal_exp, 4, 'f', 2, '0').arg(", °С"));
            }
        }
        else{
            m_coordX_exp->hide();
            m_coordY_exp->hide();
        }


        if (xVal <= maxX && xVal >= minX && yVal <= maxY && yVal >= minY)
        {
            m_coordX->show();
            m_coordY->show();
            m_coordX->setPos(x+25, y);
            m_coordY->setPos(x+25, y+20);

            /* Displaying value of the mouse on the label */
            m_coordX->setText(QString("%1%2").arg(xVal, 4, 'f', 2, '0').arg(", м"));
            if(t_or_n)
                m_coordY->setText(QString("%1%2").arg(yVal, 4, 'f', 2, '0').arg(", °С"));
            else
                m_coordY->setText(QString("%1%2").arg(yVal, 4, 'f', 2, '0').arg(", Па·с"));
        }
        else{
            m_coordX->hide();
            m_coordY->hide();
        }

        QGraphicsView::mouseMoveEvent(event);
}


bool MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    bool check;
    G_L = arg1.toDouble(&check);
    if(G_L<=0)
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
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    bool check;
    G_W = arg1.toDouble(&check);
    if(G_W<=0)
        check = false;
    if(!check){
        ui->lineEdit_2->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_2->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    bool check;
    G_H = arg1.toDouble(&check);
    if(G_H<=0)
        check = false;
    if(!check){
        ui->lineEdit_3->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_3->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    bool check;
    U_V = arg1.toDouble(&check);
    if(U_V<=0)
        check = false;
    if(!check){
        ui->lineEdit_4->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_4->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{
    bool check;
    U_T = arg1.toDouble(&check);
    if(U_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_5->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_5->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_15_textChanged(const QString &arg1)
{
    bool check;
    P_Z = arg1.toDouble(&check);
    if(P_Z<=0)
        check = false;
    if(!check){
        ui->lineEdit_15->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_15->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
    bool check;
    P_P = arg1.toDouble(&check);
    if(P_P<=0)
        check = false;
    if(!check){
        ui->lineEdit_6->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_6->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{
    bool check;
    P_C = arg1.toDouble(&check);
    if(P_C<=0)
        check = false;
    if(!check){
        ui->lineEdit_7->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_7->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_8_textChanged(const QString &arg1)
{
    bool check;
    P_T = arg1.toDouble(&check);
    if(P_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_8->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_8->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_9_textChanged(const QString &arg1)
{
    bool check;
    K_U = arg1.toDouble(&check);
    if(K_U<=0)
        check = false;
    if(!check){
        ui->lineEdit_9->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_9->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_10_textChanged(const QString &arg1)
{
    bool check;
    K_B = arg1.toDouble(&check);
    if(K_B<=0)
        check = false;
    if(!check){
        ui->lineEdit_10->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_10->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_11_textChanged(const QString &arg1)
{
    bool check;
    K_T = arg1.toDouble(&check);
    if(K_T<=0)
        check = false;
    if(!check){
        ui->lineEdit_11->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_11->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_12_textChanged(const QString &arg1)
{
    bool check;
    K_N = arg1.toDouble(&check);
    if(K_N<=0)
        check = false;
    if(!check){
        ui->lineEdit_12->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_12->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

bool MainWindow::on_lineEdit_13_textChanged(const QString &arg1)
{
    bool check;
    K_A = arg1.toDouble(&check);
    if(K_A<=0)
        check = false;
    if(!check){
        ui->lineEdit_13->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_13->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    stop_rachet = check;
    return check;
}

void MainWindow::on_pushButton_7_clicked()
{


    QString file = QFileDialog::getSaveFileName(this,"Сохранить файл","","*.xlsx");
    if(file=="")
        return;
    QFile(file).remove();
    QFile::copy(":/new/prefix1/list_last.xlsx", file);
    QFile f(file);
    f.setPermissions((((f.permissions() |= QFile::WriteOwner) |= QFile::WriteUser) |= QFile::WriteGroup) |= QFile::WriteOther);

    ui->pushButton_7->setText("Идет генерация...");
    ui->label_34->hide();
    ui->progressBar->show();
    ui->progressBar->setRange(0,vec_m.size());
    QThread *thread= new QThread;
    My_Excel_Thread *my = new My_Excel_Thread(file,G_L,G_W,G_H, U_V,U_T,P_Z,P_P, P_C, P_T,
                                              K_U,K_B, K_N, K_T, K_A,vec_m, vec_t,vec_u, ui->comboBox->currentText(), ui->lineEdit_14->text(),ui->lineEdit_16->text(),ui->lineEdit_17->text());

    my->moveToThread(thread);
    connect(my, SIGNAL(on_progress()), this, SLOT(n_otchet_progress()));
    connect(my, SIGNAL(finish()), this, SLOT(on_otchet()));
    //connect(my, SIGNAL(on_progress()), this, SLOT(on_otchet_progress()));
    //connect(my, SIGNAL(send(int)), this, SLOT(update(int)));
    connect(thread, SIGNAL(started()), my, SLOT(doWork()));

    thread->start();
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_6->setEnabled(false);

}

void MainWindow::on_otchet()
{
    ui->progressBar->hide();
    ui->label_34->show();
    ui->pushButton_7->setText("Генерировать отчет");
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    progress_int = 0;
}

void MainWindow::n_otchet_progress()
{
    ui->progressBar->setValue(progress_int++);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->groupBox_8->hide();
    ui->groupBox_10->show();
}

void MainWindow::on_pushButton_11_clicked()//материалы
{
    db.setDatabaseName("flowmodel.db3");
    ui->label_47->setText("Материалы");
    this->select_buttons();
    ui->pushButton_11->setStyleSheet("QPushButton {"
                                    "background-color: rgb(255, 229, 183);"
                                    "border: 1px solid #000000;"
                                    "font: bold;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(223, 200, 160);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("MATERIAL");
    list<<"Id"<<"Название";
    list_bd<<"MATERIAL"<<"id"<<"name";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
    table_number_toltip = 1;
}
void MainWindow::on_pushButton_2_clicked()//параметры
{
    db.setDatabaseName("flowmodel.db3");
    ui->label_47->setText("Параметры");
    this->select_buttons();
    ui->pushButton_2->setStyleSheet("QPushButton {"
                                    "background-color: rgb(255, 229, 183);"
                                    "border: 1px solid #000000;"
                                    "font: bold;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(223, 200, 160);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("PARAMETR");
    list<<"Id"<<"Название"<<"Размерность"<<"Тип";
    list_bd<<"PARAMETR"<<"id"<<"name"<<"demension"<<"type";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
    table_number_toltip = 2;


}
void MainWindow::on_pushButton_12_clicked()//значения
{
    db.setDatabaseName("flowmodel.db3");
    ui->label_47->setText("Значения");
    this->select_buttons();
    ui->pushButton_12->setStyleSheet("QPushButton {"
                                    "background-color: rgb(255, 229, 183);"
                                    "border: 1px solid #000000;"
                                    "font: bold;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(223, 200, 160);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("VALUE");
    list<<"Id"<<"Значение"<<"Параметр (Id)"<<"Материал (Id)";
    list_bd<<"VALUE"<<"id"<<"value"<<"parametr_id"<<"material_id";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
    table_number_toltip = 3;




}
void MainWindow::on_pushButton_10_clicked()//пользователи
{
    db.setDatabaseName("flowmodel_user.db3");
    ui->label_47->setText("Пользователи");
    this->select_buttons();
    ui->pushButton_10->setStyleSheet("QPushButton {"
                                    "background-color:  rgb(213, 255, 234);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 15px;"
                                    "font: bold;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(182, 218, 200);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("USER");
    list<<"Id"<<"Роль"<<"Логин"<<"Пароль";
    list_bd<<"USER"<<"ID"<<"Role"<<"Login"<<"Password";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
    table_number_toltip = 4;



}
void MainWindow::on_pushButton_8_clicked()//добавить
{
    QStringList list_present;
    if(list_bd.at(0)=="USER"){
         Dialog_user *edit = new Dialog_user(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model2()));
         edit->exec();
    }
    if(list_bd.at(0)=="MATERIAL"){
         Dialog_material *edit = new Dialog_material(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="PARAMETR"){
         Dialog_Parametr *edit = new Dialog_Parametr(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="VALUE"){
         Dialog_value *edit = new Dialog_value(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }



}
void MainWindow::slotEditRecord(QModelIndex index)//редактировать
{

    QStringList list_present;
    for(int i =0;i<list.size();i++){
        list_present<<ui->tableView->model()->data(ui->tableView->model()->index(index.row(),i)).toString();
    }
    if(list_bd.at(0)=="USER"){
         Dialog_user *edit = new Dialog_user(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model2()));
         edit->exec();
    }
    if(list_bd.at(0)=="MATERIAL"){
         Dialog_material *edit = new Dialog_material(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="PARAMETR"){
         Dialog_Parametr *edit = new Dialog_Parametr(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="VALUE"){
         Dialog_value *edit = new Dialog_value(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
}





void MainWindow::update_model()
{
    db.setDatabaseName("flowmodel.db3");
    db.open();
    material_tooltop.clear();
    parametr_tooltop.clear();
    QSqlQuery query;
    query.exec("SELECT id, name FROM MATERIAL");
    while (query.next()){
        material_tooltop.push_back(query.value(0).toString());
        material_tooltop.push_back(query.value(1).toString());
    }
    material_tooltop.push_back(QString::number(0));

    query.exec("SELECT id, name FROM PARAMETR");
    while (query.next()){
        parametr_tooltop.push_back(query.value(0).toString());
        parametr_tooltop.push_back(query.value(1).toString());
    }
    parametr_tooltop.push_back(QString::number(0));
    db.close();


    db.open();
    model->select();
    db.close();



}

void MainWindow::update_model2()
{
    db.setDatabaseName("flowmodel_user.db3");
    db.open();
    model->select();
    db.close();

}

void MainWindow::select_buttons()
{
    ui->pushButton_11->setStyleSheet("QPushButton {"
                                    "background-color: rgb(255, 229, 183);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(223, 200, 160);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    ui->pushButton_2->setStyleSheet("QPushButton {"
                                    "background-color: rgb(255, 229, 183);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(223, 200, 160);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    ui->pushButton_12->setStyleSheet("QPushButton {"
                                    "background-color: rgb(255, 229, 183);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(223, 200, 160);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
    ui->pushButton_10->setStyleSheet("QPushButton {"
                                    "background-color:  rgb(213, 255, 234);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 15px;"
                                    "}"
                                    "QPushButton::hover {"
                                    "background-color: rgb(182, 218, 200);"
                                    "}"
                                    "QPushButton::pressed {"
                                    "border: 3px solid #000000;"
                                    "}");
}






void MainWindow::on_pushButton_13_clicked()
{
    ui->lineEdit_18->setText("");
    ui->lineEdit_19->setText("");
}

void MainWindow::experement(double v1, double v2, double t1, double t2, double tt, bool o1,bool o2)
{


    if(
    this->on_lineEdit_textChanged(ui->lineEdit->text())&&
    this->on_lineEdit_2_textChanged(ui->lineEdit_2->text())&&
    this->on_lineEdit_3_textChanged(ui->lineEdit_3->text())&&
    this->on_lineEdit_4_textChanged(ui->lineEdit_4->text())&&
    this->on_lineEdit_5_textChanged(ui->lineEdit_5->text())&&
    this->on_lineEdit_6_textChanged(ui->lineEdit_6->text())&&
    this->on_lineEdit_7_textChanged(ui->lineEdit_7->text())&&
    this->on_lineEdit_8_textChanged(ui->lineEdit_8->text())&&
    this->on_lineEdit_9_textChanged(ui->lineEdit_9->text())&&
    this->on_lineEdit_10_textChanged(ui->lineEdit_10->text())&&
    this->on_lineEdit_11_textChanged(ui->lineEdit_11->text())&&
    this->on_lineEdit_12_textChanged(ui->lineEdit_12->text())&&
    this->on_lineEdit_13_textChanged(ui->lineEdit_13->text())&&
    this->on_lineEdit_15_textChanged(ui->lineEdit_15->text())){

    }else{
        qDebug()<<"no";
        QMessageBox::information(this,"Ошибка","Проверьте правильность входных данных");
        return;
    }

    ui->groupBox_11->show();
    ui->groupBox->hide();
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_22->clear();
    ui->lineEdit_23->clear();
    ui->tableWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget_2->setStyleSheet("QTableView { background-color: rgb(255, 255, 255); }"
                                   "QHeaderView::section:horizontal { "
                                    "color: rgb(0, 0, 0);"
                                    "font-size:13px;"
                                    "font: italic;"
                                   "border: 1px solid #000000;"
                                    "background-color: rgb(220, 236, 255);"

                                     "}");


    ui->tableWidget_2->setColumnCount(4);
    double i_first;
    double i_second;
    if(o1){
        o_exp = true;
        ui->label_61->setText("Результаты вычислительного эксперимента\n(зависимости производительности канала, температуры продукта, вязкости продукта от скорости крышки)");
        ui->label_54->setText("Результаты приведены при варьировании скорости крышки");
        ui->tableWidget_2->setHorizontalHeaderItem(0,new QTableWidgetItem("Скорость\nкрышки, м/с"));
        ui->lineEdit_20->setText(QString::number(v1));
        ui->lineEdit_21->setText(QString::number(v2));
        ui->label_56->setEnabled(false);
        ui->label_58->setEnabled(false);
        ui->label_59->setEnabled(false);
        ui->lineEdit_22->setEnabled(false);
        ui->lineEdit_23->setEnabled(false);
        ui->label_53->setEnabled(true);
        ui->label_55->setEnabled(true);
        ui->label_57->setEnabled(true);
        ui->lineEdit_20->setEnabled(true);
        ui->lineEdit_21->setEnabled(true);
        ui->lineEdit_22->setText(ui->lineEdit_5->text());
        ui->lineEdit_23->setText(ui->lineEdit_5->text());
        i_first = v1;
        i_second = v2;

    }else{
        o_exp = false;
                ui->label_61->setText("Результаты вычислительного эксперимента\n(зависимости производительности канала, температуры продукта, вязкости продукта от температуры крышки)");
        ui->label_54->setText("Результаты приведены при варьировании температуры крышки");
        ui->tableWidget_2->setHorizontalHeaderItem(0,new QTableWidgetItem("Температура\nкрышки, °С"));
        ui->lineEdit_22->setText(QString::number(t1));
        ui->lineEdit_23->setText(QString::number(t2));
        ui->label_53->setEnabled(false);
        ui->label_55->setEnabled(false);
        ui->label_57->setEnabled(false);
        ui->lineEdit_20->setEnabled(false);
        ui->lineEdit_21->setEnabled(false);
        ui->label_56->setEnabled(true);
        ui->label_58->setEnabled(true);
        ui->label_59->setEnabled(true);
        ui->lineEdit_22->setEnabled(true);
        ui->lineEdit_23->setEnabled(true);
        ui->lineEdit_20->setText(ui->lineEdit_4->text());
        ui->lineEdit_21->setText(ui->lineEdit_4->text());
        i_first = t1;
        i_second = t2;
    }
    ui->tableWidget_2->setHorizontalHeaderItem(1,new QTableWidgetItem("Производительность\nканала, кг/ч"));
    ui->tableWidget_2->setHorizontalHeaderItem(2,new QTableWidgetItem("Температура\nпродукта, °С"));
    ui->tableWidget_2->setHorizontalHeaderItem(3,new QTableWidgetItem("Вязкость\nпродукта, Па·с"));
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setColumnWidth(0,ui->tableWidget->width()/4);
    ui->tableWidget_2->setColumnWidth(1,ui->tableWidget->width()/4);
    ui->tableWidget_2->setColumnWidth(2,ui->tableWidget->width()/4);
    ui->tableWidget_2->setColumnWidth(3,ui->tableWidget->width()/4);
    ui->tableWidget_2->verticalHeader()->hide();
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(o1){
        for (double i = i_first;i<=i_second;i+=tt) {
            QVector<double> v_exp_tem;
            QVector<double> v_exp_vaz;
            double F = 0.125*((G_H/G_W)*(G_H/G_W))-0.625*(G_H/G_W)+1.0;//коэффициент геометрической формы канала
                double Gamma_point = i/G_H;//скорость деформации сдвига
                double q_Gamma_point = G_W*G_H*K_U*pow(Gamma_point,K_N+1.0); //удельные тепловые потоки
                double q_Alpha = G_W*K_A*((1.0/K_B)-U_T+K_T);
                double Q_ch = (G_W*G_H*i)/2*F;//расход потока
                double M = G_L/P_Z;//расчет числа шагов
                if(q_Alpha==0.0){
                    q_Alpha = 1.0;
                }
                for(int j = 0;j<=M;j++){
                    double z_i = j*P_Z; //координата по длине канала
                    double ae_i = ((K_B*q_Gamma_point+G_W*K_A)/(K_B*q_Alpha))*(1.0-exp((-z_i*K_B*q_Alpha)/(P_P*P_C*Q_ch)))+exp(K_B*(P_T-K_T-((z_i*q_Alpha)/(P_P*P_C*Q_ch))));

                    double t_i = K_T+(1/K_B)*log(ae_i); v_exp_tem.push_back(t_i);//floor(t_i*100)/100.0);//температура
                    double u = K_U*exp(-K_B*(t_i-K_T));//вязкость
                    double nu_i = u*pow(Gamma_point,K_N-1); v_exp_vaz.push_back(nu_i);//floor(nu_i*10)/10.0);
//                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);//вывод расчетов в таблицу
//                    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString::number(z_i)));
//                    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QString("%1").arg(t_i, 0, 'f', 2)/*QString::number(floor(t_i*100)/100.0)*/));
//                    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(QString("%1").arg(nu_i, 0, 'f', 1)/*QString::number(floor(nu_i*10)/10.0)*/));
                }
                double Q = Q_ch*P_P*3600;  //производительность
                ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);//вывод расчетов в таблицу
               ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(QString::number(i)));
               ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(QString("%1").arg(Q, 0, 'f', 0)/*QString::number(floor(t_i*100)/100.0)*/));
               ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(QString("%1").arg(v_exp_tem.last(), 0, 'f', 2)/*QString::number(floor(nu_i*10)/10.0)*/));
                ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,3,new QTableWidgetItem(QString("%1").arg(v_exp_vaz.last(), 0, 'f', 1)/*QString::number(floor(nu_i*10)/10.0)*/));
                vec_t_exp.push_back(i);
                vec_t_exp.push_back(Q);
                vec_t_exp.push_back(v_exp_tem.last());
                vec_t_exp.push_back(v_exp_vaz.last());
                v_exp_tem.clear();
                v_exp_vaz.clear();
        }
    }else{
        for (double i = i_first;i<=i_second;i+=tt) {
            QVector<double> v_exp_tem;
            QVector<double> v_exp_vaz;
            double F = 0.125*((G_H/G_W)*(G_H/G_W))-0.625*(G_H/G_W)+1.0;//коэффициент геометрической формы канала
                double Gamma_point = U_V/G_H;//скорость деформации сдвига
                double q_Gamma_point = G_W*G_H*K_U*pow(Gamma_point,K_N+1.0); //удельные тепловые потоки
                double q_Alpha = G_W*K_A*((1.0/K_B)-i+K_T);
                double Q_ch = (G_W*G_H*U_V)/2*F;//расход потока
                double M = G_L/P_Z;//расчет числа шагов
                if(q_Alpha==0.0){
                    q_Alpha = 1.0;
                }
                for(int j = 0;j<=M;j++){
                    double z_i = j*P_Z; //координата по длине канала
                    double ae_i = ((K_B*q_Gamma_point+G_W*K_A)/(K_B*q_Alpha))*(1.0-exp((-z_i*K_B*q_Alpha)/(P_P*P_C*Q_ch)))+exp(K_B*(P_T-K_T-((z_i*q_Alpha)/(P_P*P_C*Q_ch))));

                    double t_i = K_T+(1/K_B)*log(ae_i); v_exp_tem.push_back(t_i);//floor(t_i*100)/100.0);//температура
                    double u = K_U*exp(-K_B*(t_i-K_T));//вязкость
                    double nu_i = u*pow(Gamma_point,K_N-1); v_exp_vaz.push_back(nu_i);//floor(nu_i*10)/10.0);
//                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);//вывод расчетов в таблицу
//                    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString::number(z_i)));
//                    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QString("%1").arg(t_i, 0, 'f', 2)/*QString::number(floor(t_i*100)/100.0)*/));
//                    ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(QString("%1").arg(nu_i, 0, 'f', 1)/*QString::number(floor(nu_i*10)/10.0)*/));
                }
                double Q = Q_ch*P_P*3600;  //производительность
                ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);//вывод расчетов в таблицу
               ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(QString::number(i)));
               ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(QString("%1").arg(Q, 0, 'f', 0)/*QString::number(floor(t_i*100)/100.0)*/));
               ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(QString("%1").arg(v_exp_tem.last(), 0, 'f', 2)/*QString::number(floor(nu_i*10)/10.0)*/));
                ui->tableWidget_2->setItem(0+ui->tableWidget_2->rowCount()-1,3,new QTableWidgetItem(QString("%1").arg(v_exp_vaz.last(), 0, 'f', 1)/*QString::number(floor(nu_i*10)/10.0)*/));
                vec_t_exp.push_back(i);
                vec_t_exp.push_back(Q);
                vec_t_exp.push_back(v_exp_tem.last());
                vec_t_exp.push_back(v_exp_vaz.last());
                v_exp_tem.clear();
                v_exp_vaz.clear();
        }
    }

    for (int i = 0;i<vec_t_exp.size();i+=4) {
        seriesA_exp->append(vec_t_exp.at(i), vec_t_exp.at(i+1));
        seriesB_exp->append(vec_t_exp.at(i), vec_t_exp.at(i+2));
        seriesC_exp->append(vec_t_exp.at(i), vec_t_exp.at(i+3));
    }
    chart__exp->addSeries(seriesA_exp);
    chart__exp->addSeries(seriesB_exp);
    chart__exp->addSeries(seriesC_exp);
    seriesB_exp->setColor(QColor(255, 166, 56));
    seriesC_exp->setColor(QColor(45, 193, 50));
    seriesA_exp->setColor(QColor(255, 6, 56));
    seriesA_exp->hide();
    seriesB_exp->hide();
    seriesC_exp->hide();

    this->on_pushButton_20_clicked();



    qDebug()<<v1<< v2<<t2<<t1<<tt<<o1<<o2;
}

void MainWindow::on_pushButton_14_clicked()//войти
{
    db.setDatabaseName("flowmodel_user.db3");
    db.open();
    QSqlQuery query;
    QVector<QString> employ;
    query.exec("SELECT Role, Login, Password FROM USER");
    while (query.next()){
        employ.push_back(query.value(0).toString());
        employ.push_back(query.value(1).toString());
        employ.push_back(query.value(2).toString());
    }
    db.close();

    for (int i = 1;i<employ.size();i+=3) {
        if((ui->lineEdit_18->text()==employ.at(i))&&(ui->lineEdit_19->text()==employ.at(i+1))){
            if(employ.at(i-1)=="Администратор"){
                   ui->groupBox_10->hide();
                   ui->groupBox_8->show();
                   this->on_pushButton_11_clicked();
            }
            if(employ.at(i-1)=="Исследователь"){
                   ui->groupBox_10->hide();
                   ui->comboBox->clear();
                   db.setDatabaseName("flowmodel.db3");
                   db.open();
                   QSqlQuery query;
                   query.exec("SELECT name FROM MATERIAL");
                   while (query.next()){
                       ui->comboBox->addItem(query.value(0).toString());
                   }
                   this->on_comboBox_activated(ui->comboBox->currentText());

                   db.close();

                   ui->groupBox->show();

            }
        }
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    qDebug()<<arg1;
    db.open();
    QSqlQuery query;
    query.exec("SELECT MATERIAL.name, PARAMETR.name, VALUE.value FROM MATERIAL INNER JOIN VALUE on MATERIAL.id = VALUE.material_id"
               " INNER JOIN PARAMETR on VALUE.parametr_id = PARAMETR.id WHERE MATERIAL.name =  \""+arg1+"\"");
    while (query.next()){
        if(query.value(1).toString()=="Плотность"){
          ui->lineEdit_6->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Удельная теплоемкость"){
          ui->lineEdit_7->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Температура плавления"){
          ui->lineEdit_8->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Коэффициент консинстенции материала"){
          ui->lineEdit_9->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Температурный коэффициент вязкости"){
          ui->lineEdit_10->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Температура приведения"){
          ui->lineEdit_11->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Индекс течения материала"){
          ui->lineEdit_12->setText(query.value(2).toString());
        }
        if(query.value(1).toString()=="Коэффициент теплоотдачи"){
          ui->lineEdit_13->setText(query.value(2).toString());
        }

    }

    db.close();




}




void MainWindow::on_pushButton_15_clicked()
{
    //QString s = QFileDialog::getSaveFileUrl(this, "Резервное копирование").toString();
    QString file = QFileDialog::getSaveFileName(this,"Резервное копирование","flowmodel.db3","*.db3");
    if(file=="")
        return;
    QFile::copy("flowmodel.db3",file);
    //QFile(file).remove();
    file = file.insert(file.length()-4,"_user");
    QFile::copy("flowmodel_user.db3", file);
}

void MainWindow::on_pushButton_18_clicked()//назад
{
    chart__exp->removeSeries(seriesA_exp);
    chart__exp->removeSeries(seriesB_exp);
    chart__exp->removeSeries(seriesC_exp);
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    vec_t_exp.clear();
//    vec_m.clear();
//    vec_t.clear();
//    vec_u.clear();
    seriesA_exp->clear();
    seriesB_exp->clear();
    seriesC_exp->clear();
    ui->groupBox_11->hide();
    ui->groupBox->show();
}

void MainWindow::on_pushButton_20_clicked()//производ
{
    p_t_n = 1;
    ui->pushButton_20->clearFocus();
    ui->pushButton_20->setFont(QFont("MS Shell Dlg 2",8,75));
    ui->pushButton_16->setFont(QFont("MS Shell Dlg 2",8,50));
    ui->pushButton_17->setFont(QFont("MS Shell Dlg 2",8,50));
    seriesA_exp->show();
    seriesB_exp->hide();
    seriesC_exp->hide();
    seriesA_exp->attachAxis(axisY_exp);
    seriesA_exp->attachAxis(axisX_exp);

    axisX_exp->setRange(vec_t_exp.first(),vec_t_exp.at(vec_t_exp.size()-4));
    if(o_exp){
        if(vec_t_exp.at(1)<vec_t_exp.at(vec_t_exp.size()-3))
            axisY_exp->setRange(vec_t_exp.at(1),vec_t_exp.at(vec_t_exp.size()-3));
        else
            axisY_exp->setRange(vec_t_exp.at(vec_t_exp.size()-3),vec_t_exp.at(1));

    }else{
        axisY_exp->setRange(vec_t_exp.at(1)-(vec_t_exp.at(1)/100*5),vec_t_exp.at(1)+(vec_t_exp.at(1)/100*5));
    }

    if(o_exp){
        axisX_exp->setTitleText("Скорость крышки, м/с");
    }else{
       axisX_exp->setTitleText("Температура крышки, °С");
    }
    axisY_exp->setTitleText("Производительность канала, кг/ч");
}

void MainWindow::on_pushButton_16_clicked()//тем
{
    p_t_n = 2;
    ui->pushButton_16->clearFocus();
    ui->pushButton_16->setFont(QFont("MS Shell Dlg 2",8,75));
    ui->pushButton_20->setFont(QFont("MS Shell Dlg 2",8,50));
    ui->pushButton_17->setFont(QFont("MS Shell Dlg 2",8,50));
    seriesA_exp->hide();
    seriesB_exp->show();
    seriesC_exp->hide();
    seriesB_exp->attachAxis(axisY_exp);
    seriesB_exp->attachAxis(axisX_exp);

    axisX_exp->setRange(vec_t_exp.first(),vec_t_exp.at(vec_t_exp.size()-4));
    if(vec_t_exp.at(2)<vec_t_exp.at(vec_t_exp.size()-2))
        axisY_exp->setRange(vec_t_exp.at(2),vec_t_exp.at(vec_t_exp.size()-2));
    else
        axisY_exp->setRange(vec_t_exp.at(vec_t_exp.size()-2),vec_t_exp.at(2));

    if(o_exp){
        axisX_exp->setTitleText("Скорость крышки, м/с");
    }else{
       axisX_exp->setTitleText("Температура крышки, °С");
    }
    axisY_exp->setTitleText("Температура продукта, °С");
}

void MainWindow::on_pushButton_17_clicked()//вяз
{
    p_t_n = 3;
    ui->pushButton_17->clearFocus();
    ui->pushButton_17->setFont(QFont("MS Shell Dlg 2",8,75));
    ui->pushButton_20->setFont(QFont("MS Shell Dlg 2",8,50));
    ui->pushButton_16->setFont(QFont("MS Shell Dlg 2",8,50));
    seriesA_exp->hide();
    seriesB_exp->hide();
    seriesC_exp->show();
    seriesC_exp->attachAxis(axisY_exp);
    seriesC_exp->attachAxis(axisX_exp);

    axisX_exp->setRange(vec_t_exp.first(),vec_t_exp.at(vec_t_exp.size()-4));
    if(vec_t_exp.at(3)<vec_t_exp.at(vec_t_exp.size()-1))
        axisY_exp->setRange(vec_t_exp.at(3),vec_t_exp.at(vec_t_exp.size()-1));
    else
        axisY_exp->setRange(vec_t_exp.at(vec_t_exp.size()-1),vec_t_exp.at(3));

    if(o_exp){
        axisX_exp->setTitleText("Скорость крышки, м/с");
    }else{
       axisX_exp->setTitleText("Температура крышки, °С");
    }
    axisY_exp->setTitleText("Вязкость продукта, Па·с");
}

void MainWindow::on_pushButton_19_clicked()//эксперимент
{
    dialog_experiment *edit = new dialog_experiment(ui->lineEdit_5->text().toDouble(),ui->lineEdit_4->text().toDouble(),this);
    edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    edit->setWindowTitle("Диапазоны изменения параметров для вычислительного эксперимента");
    connect(edit,SIGNAL(experement(double, double, double, double, double, bool,bool)),this,SLOT(experement(double, double, double, double, double, bool,bool)));
    edit->exec();
}
