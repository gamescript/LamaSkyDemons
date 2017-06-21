TARGET = lamaskydemons

LIBS += ../LamaSkyDemons/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

QMAKE_CXXFLAGS += -std=c++1y

INCLUDEPATH += \
    ../LamaSkyDemons/Urho3D/include \
    ../LamaSkyDemons/Urho3D/include/Urho3D/ThirdParty \

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
