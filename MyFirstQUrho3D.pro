# Add this to the constructor
# std::system("ln -s ../../../Source/Urho3D/bin/Data");
# std::system("ln -s ../../../Source/Urho3D/bin/CoreData");

LIBS += ../../../Source/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -lSDL \# -lSDL_image -lSDLttf \ # Might be obsolete
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++11



INCLUDEPATH += \
    ../../../Source/Urho3D/include \
    ../../../Source/Urho3D/Source/ThirdParty
    #../../../Source/Urho3D/Source
    #../../../Source/Urho3D/Source/Samples



TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    #../../../Source/Urho3D/Source/Samples/15_Navigation/Navigation.cpp \
    urho3dtemplate.cpp

HEADERS += \
    #../../../Source/Urho3D/Source/Samples/15_Navigation/Navigation.h \
    urho3dtemplate.h
    #../../../Source/Urho3D/Source/Samples/Sample.h
