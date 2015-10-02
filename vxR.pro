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
    vxColor.cpp \
    vxStopwatch.cpp \
    vxStandardShader.cpp \
    vxMap.cpp \
    vxMap2d.cpp \
    vxCirclesMap.cpp \
    vxramp.cpp \
    renderPresset.cpp \
    vximporter.cpp \
    vxPLYImporter.cpp \
    vxBitMap2d.cpp \
    imageData.cpp \
    vxLegoBlock.cpp \
    sseUtils.cpp \
    vxRay.cpp \
    vxUnitTest.cpp \
    vxGridUnitTest.cpp \
    MathUtilsUnitTest.cpp \
    vxThreadPool.cpp \
    vxSceneParser.cpp \
    StringUtils.cpp \
    vxValue.cpp \
    vxNode.cpp \
    vxDom.cpp

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
    vxColor.h \
    vxStopwatch.h \
    vxStandardShader.h \
    vxMap.h \
    vxMap2d.h \
    vxCirclesMap.h \
    vxramp.h \
    renderPresset.h \
    vximporter.h \
    vxPLYImporter.h \
    vxBitMap2d.h \
    imageData.h \
    vxLegoBlock.h \
    sseUtils.h \
    vxRay.h \
    vxUnitTest.h \
    vxGridUnitTest.h \
    MathUtilsUnitTest.h \
    vxThreadPool.h \
    vxSceneParser.h \
    StringUtils.h \
    vxValue.h \
    vxNode.h \
    vxDom.h

FORMS    += vxrendermain.ui

CONFIG(debug, release|debug):DEFINES += _DEBUG
CONFIG += c++14

OTHER_FILES += \
    todo.txt \
    demo.vx
