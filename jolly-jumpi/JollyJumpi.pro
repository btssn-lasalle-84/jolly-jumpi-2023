QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    IHM.cpp \
    course.cpp \
    main.cpp \
    statistiques.cpp

HEADERS += \
    IHM.h \
    course.h \
    statistiques.h

FORMS += \
    IHM.ui
