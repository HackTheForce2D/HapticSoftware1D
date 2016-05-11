#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T19:34:50
#
#-------------------------------------------------

QT       += core gui \
            network \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Haptics1D
TEMPLATE = app


CONFIG(release, debug|release): LIBS += -lBox2D -lsfml-graphics -lsfml-window -lsfml-system

SOURCES += main.cpp\
        mainwindow.cpp \
    physics.cpp \
    visual.cpp \
    hapticinterface.cpp \
    qsfmlcanvas.cpp \
    body.cpp \
    effector.cpp \
    connectiondialog.cpp

HEADERS  += mainwindow.h \
    physics.h \
    visual.h \
    hapticinterface.h \
    qsfmlcanvas.h \
    body.h \
    effector.h \
    connectiondialog.h

FORMS    += mainwindow.ui \
    connectiondialog.ui

unix:!macx: LIBS += -lBox2D -lsfml-graphics -lsfml-window
