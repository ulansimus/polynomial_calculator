#-------------------------------------------------
#
# Project created by QtCreator 2020-12-13T00:57:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = polinomials
TEMPLATE = app


SOURCES += main.cpp\
    automaton.cpp \
    polynomial.cpp \
    polynomials_calculator.cpp

HEADERS  += \
    automaton.hpp \
    polynomial.hpp \
    polynomials_calculator.h

FORMS    += mainwindow.ui
CONFIG += c++11
