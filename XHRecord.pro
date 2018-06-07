#-------------------------------------------------
#
# Project created by QtCreator 2018-05-17T10:03:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XHRecord
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    importdialog.cpp \
    fileutil.cpp

HEADERS  += widget.h \
    importdialog.h \
    fileutil.h

FORMS    += widget.ui \
    importdialog.ui

RC_ICONS = mPic.ico
