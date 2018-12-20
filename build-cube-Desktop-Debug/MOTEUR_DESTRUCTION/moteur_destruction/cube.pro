QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    quadtree.cpp \
    debris.cpp \
    gestionbatiment.cpp \
    immeuble.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    quadtree.h \
    debris.h \
    gestionbatiment.h \
    immeuble.h \
    destructmesh.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
