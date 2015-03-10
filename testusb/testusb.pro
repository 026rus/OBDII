#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T13:13:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = testusb
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -L./bin/ -lftdi

CONFIG += serialport

SOURCES += main.cpp \
    connectobd.cpp

HEADERS += \
    connectobd.h
