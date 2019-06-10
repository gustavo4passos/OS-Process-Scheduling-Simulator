HEADERS += src/gui/mainwindow.h
HEADERS += src/gui/proccessbox.h
HEADERS += src/core/operatingsystem.h
HEADERS += src/core/proccess.h
HEADERS += src/core/scheduler.h
HEADERS += src/gui/proccesslist.h
HEADERS += src/gui/proccesslistloader.h
HEADERS += src/gui/visualizationwindow.h
HEADERS += src/core/memory.h
HEADERS += src/core/memorymanager.h
HEADERS += src/gui/rect.hpp

SOURCES += src/core/main.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/gui/proccessbox.cpp
SOURCES += src/core/operatingsystem.cpp
SOURCES += src/core/proccess.cpp
SOURCES += src/core/scheduler.cpp
SOURCES += src/gui/proccesslist.cpp
SOURCES += src/gui/proccesslistloader.cpp
SOURCES += src/gui/visualizationwindow.cpp
SOURCES += src/core/memorymanager.cpp

QT += core
QT += widgets

TARGET = os-scheduling-simulator
DESTDIR = ./bin
OBJECTS_DIR = ./build
