QT += core gui opengl openglwidgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += "D:\Qt\glew-2.1.0\include"

LIBS += \
	Glu32.lib \
	OpenGL32.lib
LIBS += glew32.lib

SOURCES += \
    main.cpp \
    myglwidget.cpp

HEADERS += \
    myglwidget.h