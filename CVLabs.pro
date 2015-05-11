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
    dmatch.h \
    phasespace.h

FORMS    += mainwindow.ui



#INCLUDEPATH += C:/downloads/GnuWin32/include

#DEPENDPATH += C:/downloads/GnuWin32/lib
#DEPENDPATH += C:/downloads/GnuWin32/include


# LIBS += C:/downloads/GnuWin32/lib/libgsl.lib
# LIBS += C:/downloads/GnuWin32/lib/libgslcblas.lib
# LIBS += -LC:/downloads/GnuWin32/lib/libgsl.dll.a
# LIBS += -LC:/downloads/GnuWin32/lib/libgslcblas.dll.a

#LIBS += -L$$PWD/../../../../../downloads/GnuWin32/lib/ -llibgsl

INCLUDEPATH += $$PWD/../../../../../downloads/GSL/include
DEPENDPATH += $$PWD/../../../../../downloads/GSL/include


LIBS += -LC:/downloads/GSL/lib -lgsl -lgslcblas

# LIBS += -L$$PWD/../../../../../downloads/GSL/lib/ -lgsl


#PRE_TARGETDEPS += $$PWD/../../../../../downloads/GSL/lib/libgsl.a

#LIBS += -L$$PWD/../../../../../downloads/GSL/lib/ -lgslcblas


#PRE_TARGETDEPS += $$PWD/../../../../../downloads/GSL/lib/libgslcblas.a





# LIBS += -L$$PWD/../../../../../downloads/GSL/lib/ -llibgsl.dll



# LIBS += -L$$PWD/../../../../../downloads/GSL/lib/ -llibgslcblas.dll

# PRE_TARGETDEPS += $$PWD/../../../../../downloads/GSL/lib/libgslcblas.dll.a
