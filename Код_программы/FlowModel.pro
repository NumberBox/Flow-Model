QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++11 qaxcontainer

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_LFLAGS_RELEASE += -static -static-libgcc
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog_experiment.cpp \
    dialog_material.cpp \
    dialog_parametr.cpp \
    dialog_user.cpp \
    dialog_value.cpp \
    main.cpp \
    mainwindow.cpp \
    my_excel_thread.cpp

HEADERS += \
    dialog_experiment.h \
    dialog_material.h \
    dialog_parametr.h \
    dialog_user.h \
    dialog_value.h \
    mainwindow.h \
    my_excel_thread.h

FORMS += \
    dialog_experiment.ui \
    dialog_material.ui \
    dialog_parametr.ui \
    dialog_user.ui \
    dialog_value.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resurs.qrc
