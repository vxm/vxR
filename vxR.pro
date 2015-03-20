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
    vxVector.cpp \
    vxShader.cpp \
    vxScene.cpp \
    vxObject.cpp \
    vxLight.cpp \
    vxGlobal.cpp \
    vxCamera.cpp \
    vxBoxMatrix.cpp \
    vxBoxCreator.cpp \
    vxBox.cpp \
    vxContactBuffer.cpp \
    vxBucketList.cpp \
    vxStatus.cpp \
    ImageProperties.cpp \
    vxMatrix.cpp \
    vxPixel.cpp \
    vxProcess.cpp \
    vxRayEmitter.cpp \
    vxRenderMain.cpp \
    vxRenderProcess.cpp \
    vxrMainUi.cpp

HEADERS  += \
    vxVector.h \
    vxShader.h \
    vxScene.h \
    vxObject.h \
    vxLight.h \
    vxGlobal.h \
    vxCamera.h \
    vxBoxMatrix.h \
    vxBoxCreator.h \
    vxBox.h \
    vxContactBuffer.h \
    vxBucketList.h \
    vxStatus.h \
    ImageProperties.h \
    vxMatrix.h \
    vxPixel.h \
    vxProcess.h \
    vxRayEmitter.h \
    vxRenderMain.h \
    vxRenderProcess.h \
    vxrMainUi.h

FORMS    += vxrendermain.ui

CONFIG += c++11
