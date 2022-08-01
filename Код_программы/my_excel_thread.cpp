#include "my_excel_thread.h"

#include <qaxobject.h>
#include <qaxwidget.h>
#include <qapplication.h>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTime>
#include <QMessageBox>

My_Excel_Thread::My_Excel_Thread(QString file1, double G_L1, double G_W1, double G_H1, double U_V1, double U_T1, double P_Z1, double P_P1, double P_C1, double P_T1,
                                 double K_U1, double K_B1, double K_N1, double K_T1, double K_A1, QVector<double> vec_m1, QVector<double> vec_t1, QVector<double> vec_u1, QString combo1, QString proi1, QString tem1, QString vyz1)
{

    file = file1;
    G_L = G_L1;
    G_W = G_W1;
    G_H = G_H1;
    U_T = U_T1;
    U_V = U_V1;
    P_Z = P_Z1;
    P_P = P_P1;
    P_C = P_C1;
    P_T = P_T1;
    K_U = K_U1;
    K_B = K_B1;
    K_N =K_N1;
    K_T= K_T1;
    K_A = K_A1;
    vec_m = vec_m1;
    vec_t = vec_t1;
    vec_u = vec_u1;
    combo = combo1;
    proi = proi1;
    tem = tem1;
    vyz = vyz1;
}

void My_Excel_Thread::doWork()
{
    emit on_progress();

    QAxObject *excel = new QAxObject("Excel.Application", this);
    excel->dynamicCall("SetVisible(bool)", false);//вимость документа
    QAxObject *workbooks = excel->querySubObject("Workbooks");//рабочая книга
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", file);
    QAxObject *sheets = workbook->querySubObject("Worksheets");

    int count = sheets->dynamicCall("Count()").toInt();
    QAxObject* sheet = sheets->querySubObject("Item( int )", 1 ); //где 1 - это открываемая страница (Лист 1)

    QAxObject* I4 = sheet->querySubObject("Range(const QVariant&)", QVariant("I4"));
    I4->dynamicCall( "SetValue (const QVariant&)", combo);
    delete I4;

    //зададим рабочую область
    QAxObject* G4 = sheet->querySubObject("Range(const QVariant&)", QVariant("G4"));
    G4->dynamicCall( "SetValue (const QVariant&)", QString::number(U_V).replace(".",","));
    delete G4;
    QAxObject* G5 = sheet->querySubObject("Range(const QVariant&)", QVariant("G5"));
    G5->dynamicCall( "SetValue (const QVariant&)", QString::number(U_T).replace(".",","));
    delete G5;
    emit on_progress();
    QAxObject* G8 = sheet->querySubObject("Range(const QVariant&)", QVariant("G8"));
    G8->dynamicCall( "SetValue (const QVariant&)", QString::number(G_W).replace(".",","));
    delete G8;
    QAxObject* G9 = sheet->querySubObject("Range(const QVariant&)", QVariant("G9"));
    G9->dynamicCall( "SetValue (const QVariant&)", QString::number(G_H).replace(".",","));
    delete G9;
    QAxObject* G10 = sheet->querySubObject("Range(const QVariant&)", QVariant("G10"));
    G10->dynamicCall( "SetValue (const QVariant&)", QString::number(G_L).replace(".",","));
    delete G10;

    QAxObject* M8 = sheet->querySubObject("Range(const QVariant&)", QVariant("M8"));
    M8->dynamicCall( "SetValue (const QVariant&)", QString::number(P_P).replace(".",","));
    delete M8;
    QAxObject* M9 = sheet->querySubObject("Range(const QVariant&)", QVariant("M9"));
    M9->dynamicCall( "SetValue (const QVariant&)", QString::number(P_C).replace(".",","));
    delete M9;
    QAxObject* M10 = sheet->querySubObject("Range(const QVariant&)", QVariant("M10"));
    M10->dynamicCall( "SetValue (const QVariant&)", QString::number(P_T).replace(".",","));
    delete M10;

    QAxObject* M13 = sheet->querySubObject("Range(const QVariant&)", QVariant("M13"));
    M13->dynamicCall( "SetValue (const QVariant&)", QString::number(K_T).replace(".",","));
    delete M13;
    QAxObject* M14 = sheet->querySubObject("Range(const QVariant&)", QVariant("M14"));
    M14->dynamicCall( "SetValue (const QVariant&)", QString::number(K_U).replace(".",","));
    delete M14;
    QAxObject* M15 = sheet->querySubObject("Range(const QVariant&)", QVariant("M15"));
    M15->dynamicCall( "SetValue (const QVariant&)", QString::number(K_B).replace(".",","));
    delete M15;
    QAxObject* M16 = sheet->querySubObject("Range(const QVariant&)", QVariant("M16"));
    M16->dynamicCall( "SetValue (const QVariant&)", QString::number(K_A).replace(".",","));
    delete M16;
    QAxObject* M17 = sheet->querySubObject("Range(const QVariant&)", QVariant("M17"));
    M17->dynamicCall( "SetValue (const QVariant&)", QString::number(K_N).replace(".",","));
    delete M17;
     emit on_progress();
    QAxObject* S4 = sheet->querySubObject("Range(const QVariant&)", QVariant("S4"));
    S4->dynamicCall( "SetValue (const QVariant&)", proi.replace(".",","));
    delete S4;
    QAxObject* S5 = sheet->querySubObject("Range(const QVariant&)", QVariant("S5"));
    S5->dynamicCall( "SetValue (const QVariant&)", tem.replace(".",","));
    delete S5;
    QAxObject* S6 = sheet->querySubObject("Range(const QVariant&)", QVariant("S6"));
    S6->dynamicCall( "SetValue (const QVariant&)", vyz.replace(".",","));
    delete S6;

    int shablon = 13;
    for (int i = 0;i<vec_m.size();i++) {     
            emit on_progress();

        QAxObject *rangec = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("O"+QString::number(shablon)+":P"+QString::number(shablon))));
        rangec->dynamicCall("Select()");
        rangec->dynamicCall("MergeCells",true);

        delete rangec;
        QAxObject* a = sheet->querySubObject("Range(const QVariant&)", QVariant("O"+QString::number(shablon)));
        a->dynamicCall( "SetValue (const QVariant&)", QString::number(vec_m.at(i)).replace(".",","));
        delete a;

        QAxObject *rangep = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("O"+QString::number(shablon))));
        rangep->dynamicCall("Select()");
        rangep->dynamicCall("HorizontalAlignment",-4131);
        delete rangep;

        QAxObject *rangea = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("O"+QString::number(shablon))));
        QAxObject *border = rangea->querySubObject("Borders(xlEdgeLeft)");
        border->setProperty("LineStyle",1);
        border->setProperty("Weight",2);
        delete border;
        delete rangea;


        QAxObject *rangec1 = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("Q"+QString::number(shablon)+":R"+QString::number(shablon))));
        rangec1->dynamicCall("Select()");
        rangec1->dynamicCall("MergeCells",true);
        delete rangec1;

        QAxObject* a1 = sheet->querySubObject("Range(const QVariant&)", QVariant("Q"+QString::number(shablon)));
        a1->dynamicCall( "SetValue (const QVariant&)", QString::number(vec_t.at(i)).replace(".",","));
        delete a1;

        QAxObject *rangep1 = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("Q"+QString::number(shablon))));
        rangep1->dynamicCall("Select()");
        rangep1->dynamicCall("HorizontalAlignment",-4131);
        delete rangep1;


        QAxObject *rangec2 = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("S"+QString::number(shablon)+":T"+QString::number(shablon))));
        rangec2->dynamicCall("Select()");
        rangec2->dynamicCall("MergeCells",true);
        delete rangec2;

        QAxObject* a2 = sheet->querySubObject("Range(const QVariant&)", QVariant("S"+QString::number(shablon)));
        a2->dynamicCall( "SetValue (const QVariant&)", QString::number(vec_u.at(i)).replace(".",","));
        delete a2;

        QAxObject *rangep2 = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("S"+QString::number(shablon))));
        rangep2->dynamicCall("Select()");
        rangep2->dynamicCall("HorizontalAlignment",-4131);
        delete rangep2;

        QAxObject *rangea2 = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("T"+QString::number(shablon))));
        QAxObject *border2 = rangea2->querySubObject("Borders(xlEdgeRight)");
        border2->setProperty("LineStyle",1);
        border2->setProperty("Weight",2);
        delete border2;
        delete rangea2;


        shablon++;
    }

    QAxObject *rang = sheet->querySubObject("Range(const QVariant&)",QVariant(QString("O"+QString::number(shablon)+":T"+QString::number(shablon))));
    QAxObject *bord = rang->querySubObject("Borders(xlEdgeTop)");
    bord->setProperty("LineStyle",1);
    bord->setProperty("Weight",2);
    delete bord;
    delete rang;

    QAxObject* G13 = sheet->querySubObject("Range(const QVariant&)", QVariant("G13"));
    G13->dynamicCall( "SetValue (const QVariant&)", QString::number(P_Z).replace(".",","));
    delete G13;

//    QAxObject* text1 = sheet->querySubObject( "Cells( int, int )", 2, 5);
//    text1->dynamicCall( "SetValue(const QVariant&)", "cк");

    workbook->dynamicCall("Save()");
    workbook->dynamicCall("Close");
    excel->dynamicCall("Quit()");

    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;

    QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    emit finish();
}

