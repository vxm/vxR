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
    vxGeometry.cpp \
    vxGeometryImporter.cpp \
    vxDome.cpp \
    vxTriRef.cpp \
    vxBoundingBox.cpp \
    vxGeoGrid.cpp \
    vxList.cpp \
    vxBroadPhase.cpp \
    vxTriangleMesh.cpp \
    vxClock.cpp \
    VectorUtils.cpp \
    vxNonSequentalPool.cpp

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
    vxGeometry.h \
    vxGeometryImporter.h \
    vxDome.h \
    vxTriRef.h \
    vxBoundingBox.h \
    vxGeoGrid.h \
    vxList.h \
    vxBroadPhase.h \
    vxTriangleMesh.h \
    vxClock.h \
    VectorUtils.h \
    vxNonSequentalPool.h

OTHER_FILES += \
    todo.txt \
    demo.vx \
    juan_0.ply

CONFIG(debug, release|debug):DEFINES += _DEBUG
    
CONFIG += c++14

@
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast -fomit-frame-pointer -ffast-math -march=native \
-mtune=native -flto -msse -mrecip -mfpmath=sse -msse2 -mssse3 -lSDL \
-lpthread -lstdc++
QMAKE_LFLAGS_RELEASE += -O1
@

QMAKE_CXXFLAGS += -fopenmp

#
