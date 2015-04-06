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
    vxrMainUi.cpp \
    vxGrid.cpp \
    vxSampler.cpp \
    vxmanager.cpp \
    vxPlane.cpp \
    vxPoint.cpp \
    vxCollision.cpp \
    TimeUtils.cpp \
    FileUtils.cpp \
    MathUtils.cpp \
    vxColor.cpp

HEADERS  += \
    vxVector.h \
    vxShader.h \
    vxScene.h \
    vxObject.h \
    vxLight.h \
    vxGlobal.h \
    vxCamera.h \
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
    vxrMainUi.h \
    vxGrid.h \
    vxSampler.h \
    vxmanager.h \
    vxPlane.h \
    vxPoint.h \
    vxCollision.h \
    TimeUtils.h \
    MathUtils.h \
    FileUtils.h \
    vxColor.h

FORMS    += vxrendermain.ui

CONFIG(debug, release|debug):DEFINES += _DEBUG
CONFIG += c++11

OTHER_FILES += \
    todo.txt
