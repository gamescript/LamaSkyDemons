LIBS += ../Brixtuff/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++1y

INCLUDEPATH += \
    ../Brixtuff/Urho3D/include \
    ../Brixtuff/Urho3D/include/Urho3D/ThirdParty \

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    luckey.h \
    sceneobject.h \
    mastercontrol.h \
    inputmaster.h \
    spawnmaster.h \
    brixtuffcam.h \
    player.h \
    controllable.h \
    lama.h \
    platform.h \
    spit.h \
    gravity.h

SOURCES += \
    luckey.cpp \
    sceneobject.cpp \
    mastercontrol.cpp \
    inputmaster.cpp \
    spawnmaster.cpp \
    brixtuffcam.cpp \
    player.cpp \
    controllable.cpp \
    lama.cpp \
    platform.cpp \
    spit.cpp \
    gravity.cpp
