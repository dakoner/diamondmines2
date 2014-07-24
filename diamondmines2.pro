#-------------------------------------------------
#
# Project created by QtCreator 2014-07-20T18:43:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = diamondmines210
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp \
    box2dengine.cpp

HEADERS  += \
    MotionFilter.h \
    box2dengine.h \
    UpdateReceiver.h

FORMS    +=

LIBS +=  -lBox2D

