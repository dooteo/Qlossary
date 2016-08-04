#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T21:29:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlossary
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutqlossary.cpp \
    newproject.cpp \
    sqlite.cpp \
    viewlanguages.cpp \
    newlanguage.cpp \
    viewprojects.cpp \
    mainwindow_dck_project.cpp \
    mainwindow_dck_language.cpp \
    mainwindow_private_methods.cpp \
    mainwindow_private_slots.cpp \
    sqlite_tb_language.cpp \
    sqlite_tb_project.cpp \
    sqlite_tb_term.cpp \
    mainwindow_dck_newterm.cpp \
    sqlite_tb_type.cpp \
    mainwindow_centerarea.cpp

HEADERS  += mainwindow.h \
    aboutqlossary.h \
    newproject.h \
    sqlite.h \
    viewlanguages.h \
    newlanguage.h \
    viewprojects.h \
    global_defines.h

FORMS    += mainwindow.ui \
    aboutqlossary.ui \
    newproject.ui \
    viewlanguages.ui \
    newlanguage.ui \
    viewprojects.ui

RESOURCES +=
