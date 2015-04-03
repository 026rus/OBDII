#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T14:12:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = untitled1
TEMPLATE = app


SOURCES += \
    mainwindow.cpp \
    qcustomplot.cpp \
    GUImain.cpp

HEADERS  += \
    mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui

SUBDIRS += \
    plot-examples.pro

DISTFILES +=
