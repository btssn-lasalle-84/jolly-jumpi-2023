QT       += core gui \
         multimedia \
         bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 \
          file_copies

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    IHM.cpp \
    bluetooth.cpp \
    course.cpp \
    main.cpp \
    statistiques.cpp

HEADERS += \
    IHM.h \
    bluetooth.h \
    course.h \
    statistiques.h

FORMS += \
    IHM.ui

CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT

RESOURCES += \
    res.qrc


COPIES += images
images.files = Images
images.path = $$OUT_PWD/
images.base = $$PWD/
