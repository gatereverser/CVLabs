#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T11:23:35
#
#-------------------------------------------------
CONFIG += console
QT       += core gui
QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CVLabs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cvimage.cpp \
    cv.cpp \
    cvkernel.cpp \
    pyramid.cpp

HEADERS  += mainwindow.h \
    cvimage.h \
    cv.h \
    cvkernel.h \
    pyramid.h \
    featurepoint.h \
    dmatch.h

FORMS    += mainwindow.ui



INCLUDEPATH += C:/downloads/GnuWin32/include

DEPENDPATH += C:/downloads/GnuWin32/lib


 LIBS += C:/downloads/GnuWin32/lib/libgsl.a
 LIBS += C:/downloads/GnuWin32/lib/libgslcblas.a
 LIBS += C:/downloads/GnuWin32/lib/libgsl.dll.a
 LIBS += C:/downloads/GnuWin32/lib/libgslcblas.dll.a
