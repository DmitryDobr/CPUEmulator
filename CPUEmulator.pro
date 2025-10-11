#-------------------------------------------------
#
# Project created by QtCreator 2025-04-09T20:11:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPUEmulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cpu/cpu.cpp \
    instruction/instruction.cpp \
    instruction/instructionset.cpp \
    memory/memory.cpp \
    instruction/arithmetic.cpp

HEADERS  += mainwindow.h \
    cpu/cpu.h \
    instruction/instruction.h \
    instruction/instructionset.h \
    memory/memory.h \
    instruction/arithmetic.h

FORMS    += mainwindow.ui
