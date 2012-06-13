#-------------------------------------------------
#
# Project created by QtCreator 2012-05-16T21:00:24
#
#-------------------------------------------------

QT       += core gui

TARGET = economic_LPP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ConditionDialog.cpp \
    ExpressionsDialog.cpp \
    GaussJordano.cpp \
    SolvedSystemDialog.cpp \
    FinishDialog.cpp \
    TwoDimensionalProblem.cpp \
    drawdialog.cpp \
    minmaxdrawdialog.cpp \
    GraphDialog2.cpp \
    GraphDialog1.cpp

HEADERS  += mainwindow.h \
    ConditionDialog.h \
    ExpressionsDialog.h \
    GaussJordano.h \
    SolvedSystemDialog.h \
    FinishDialog.h \
    TwoDimensionalProblem.h \
    drawdialog.h \
    minmaxdrawdialog.h \
    GraphDialog2.h \
    GraphDialog1.h

FORMS    += mainwindow.ui \
    ConditionDialog.ui \
    ExpressionsDialog.ui \
    SolvedSystemDialog.ui \
    FinishDialog.ui \
    TwoDimensionalProblem.ui \
    drawdialog.ui \
    minmaxdrawdialog.ui \
    GraphDialog2.ui \
    GraphDialog1.ui
