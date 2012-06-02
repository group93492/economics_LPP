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
    FinishDialog.cpp

HEADERS  += mainwindow.h \
    ConditionDialog.h \
    ExpressionsDialog.h \
    GaussJordano.h \
    SolvedSystemDialog.h \
    FinishDialog.h

FORMS    += mainwindow.ui \
    ConditionDialog.ui \
    ExpressionsDialog.ui \
    SolvedSystemDialog.ui \
    FinishDialog.ui
