TEMPLATE = app
CONFIG += console c11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c

LIBS += -lusb-1.0
QMAKE_CFLAGS += -pedantic -Wextra -Wcast-align -Winit-self -Wno-unused-parameter -std=c11
