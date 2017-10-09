#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T20:58:27
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vxR
TEMPLATE = app


SOURCES += main.cpp\
    ImageProperties.cpp \
    TimeUtils.cpp \
    FileUtils.cpp \
    MathUtils.cpp \
    renderPresset.cpp \
    imageData.cpp \
    sseUtils.cpp \
    MathUtilsUnitTest.cpp \
    StringUtils.cpp \
    VectorUtils.cpp \
    Matrix33.cpp \
    Matrix44.cpp \
    Quaternion.cpp \
    Light.cpp \
    Ramp.cpp \
    BoundingBox.cpp \
    Box.cpp \
    BoxCreator.cpp \
    BroadPhase.cpp \
    BucketList.cpp \
    Camera.cpp \
    CirclesMap.cpp \
    Clock.cpp \
    Collision.cpp \
    Color.cpp \
    ContactBuffer.cpp \
    Dome.cpp \
    GeoGrid.cpp \
    Geometry.cpp \
    GeometryImporter.cpp \
    Global.cpp \
    Grid.cpp \
    GridUnitTest.cpp \
    Image.cpp \
    IsoGeometry.cpp \
    LegoBlock.cpp \
    Manager.cpp \
    Map.cpp \
    Map2d.cpp \
    Node.cpp \
    NonSequentalPool.cpp \
    Pixel.cpp \
    Object.cpp \
    Plane.cpp \
    PLYImporter.cpp \
    Point.cpp \
    Process.cpp \
    Ray.cpp \
    RayEmitter.cpp \
    RenderMain.cpp \
    RenderProcess.cpp \
    rMainUi.cpp \
    Sampler.cpp \
    Scene.cpp \
    SceneParser.cpp \
    Shader.cpp \
    Vector.cpp \
    Value.cpp \
    UnitTest.cpp \
    TriRef.cpp \
    TriangleMesh.cpp \
    ThreadPool.cpp \
    Stopwatch.cpp \
    Status.cpp \
    StandardShader.cpp \
    Cylinder.cpp \
    visible.cpp \
    LightShader.cpp

HEADERS  += \
    ImageProperties.h \
    TimeUtils.h \
    MathUtils.h \
    FileUtils.h \
    renderPresset.h \
    imageData.h \
    sseUtils.h \
    MathUtilsUnitTest.h \
    StringUtils.h \
    VectorUtils.h \
    Matrix33.h \
    Matrix44.h \
    Quaternion.h \
    Light.h \
    BoundingBox.h \
    Box.h \
    Vector.h \
    Value.h \
    UnitTest.h \
    TriRef.h \
    TriangleMesh.h \
    ThreadPool.h \
    Stopwatch.h \
    Status.h \
    StandardShader.h \
    Shader.h \
    SceneParser.h \
    Scene.h \
    Sampler.h \
    rMainUi.h \
    RenderProcess.h \
    RenderMain.h \
    RayEmitter.h \
    Ray.h \
    Map2d.h \
    Node.h \
    NonSequentalPool.h \
    Object.h \
    Pixel.h \
    Plane.h \
    PLYImporter.h \
    Point.h \
    Process.h \
    Ramp.h \
    Map.h \
    Manager.h \
    LegoBlock.h \
    IsoGeometry.h \
    Image.h \
    GridUnitTest.h \
    Grid.h \
    Global.h \
    GeometryImporter.h \
    Geometry.h \
    GeoGrid.h \
    Dome.h \
    ContactBuffer.h \
    Color.h \
    Collision.h \
    Clock.h \
    CirclesMap.h \
    Camera.h \
    BucketList.h \
    BroadPhase.h \
    BoxCreator.h \
    Cylinder.h \
    visible.h \
    LightShader.h

OTHER_FILES += \
    todo.txt \
    demo.vx \
    juan_0.ply

CONFIG(debug, release|debug):DEFINES += _DEBUG

CONFIG += c++1z

@
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast -fomit-frame-pointer \
							-ffast-math -march=native \
							-mtune=native -flto -msse \
							-mrecip -mfpmath=sse -msse2 \
							-mssse3 -lSDL \
							-lpthread -lstdc++ -fopenacc
QMAKE_LFLAGS_RELEASE += -O1
@

QMAKE_CXXFLAGS += -fopenmp -fopenacc

#
