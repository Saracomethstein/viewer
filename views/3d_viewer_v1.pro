QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include(../qtgifimage/gifimage/qtgifimage.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = 3d_viewer_c
# DESTDIR = ../build

SOURCES += \
    ../backend/model.c \
    ../backend/parser.c \
    main_window.cc \
    widget.cc \
    main.cc 

HEADERS += \
    ../backend/model.h \
    ../backend/parser.h \
    main_window.h \
    widget.h 

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
