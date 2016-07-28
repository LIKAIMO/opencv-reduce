#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T21:17:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_color_ui
TEMPLATE = app

INCLUDEPATH += D:\install\opencv\build\include\opencv
INCLUDEPATH += D:\install\opencv\build\include\opencv2
INCLUDEPATH += D:\install\opencv\build\include
LIBS += D:\install\opencv\build\x64\vc12\lib\*.lib

SOURCES += main.cpp\
        colorreduce.cpp

HEADERS  += colorreduce.h

FORMS    += colorreduce.ui
