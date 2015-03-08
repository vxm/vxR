#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T20:58:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vxR
TEMPLATE = app


SOURCES += main.cpp\
        vxrendermain.cpp \
    vxVector.cpp \
    vxstatus.cpp \
    vxShader.cpp \
    vxScene.cpp \
    vxrmainui.cpp \
    vxrenderprocess.cpp \
    vxprocess.cpp \
    vxObject.cpp \
    vxmatrix.cpp \
    vxLight.cpp \
    vxGlobal.cpp \
    vxCamera.cpp \
    vxBoxMatrix.cpp \
    vxBoxCreator.cpp \
    vxBox.cpp

HEADERS  += vxrendermain.h \
    vxVector.h \
    vxstatus.h \
    vxShader.h \
    vxScene.h \
    vxrmainui.h \
    vxrenderprocess.h \
    vxprocess.h \
    vxObject.h \
    vxmatrix.h \
    vxLight.h \
    vxGlobal.h \
    vxCamera.h \
    vxBoxMatrix.h \
    vxBoxCreator.h \
    vxBox.h

FORMS    += vxrendermain.ui

CONFIG += c++11
