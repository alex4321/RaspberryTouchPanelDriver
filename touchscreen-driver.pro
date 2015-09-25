TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    func.cpp \
    x11func.cpp

HEADERS += \
    func.h \
    struct.h \
    x11func.h

LIBS += -lX11
