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

INCLUDEPATH += /home/dek/Box2D/v2.3.1/Box2D

SOURCES += main.cpp \
    box2dengine.cpp


HEADERS  += \
    MotionFilter.h \
    box2dengine.h \
    UpdateReceiver.h


FORMS    +=

#LIBS += -L/home/dek/Box2D/v2.3.1/build-Box2D-Android_for_armeabi_v7a_GCC_4_8_Qt_5_3_1-Release -lBox2D

LIBS += -L/home/dek/Box2D/v2.3.1/build-Box2D-Desktop_Qt_5_3_GCC_64bit-Debug -lBox2D
