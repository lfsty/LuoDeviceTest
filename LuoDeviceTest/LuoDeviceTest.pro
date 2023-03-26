QT       += core gui serialport printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

RC_ICONS = src/icon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Chart/chartsetting.cpp \
    src/Chart/chartwork.cpp \
    src/Chart/qcustomplot.cpp \
    src/Communicate/communicate.cpp \
    src/FileSave/filesave.cpp \
    src/Filter/filtersetting.cpp \
    src/Filter/filterwork.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/Chart/chartsetting.h \
    src/Chart/chartwork.h \
    src/Chart/qcustomplot.h \
    src/Communicate/communicate.h \
    src/ConstData.h \
    src/FileSave/filesave.h \
    src/Filter/FilterData/FilterData_1000Hz.h \
    src/Filter/FilterData/FilterData_250Hz.h \
    src/Filter/FilterData/FilterData_500Hz.h \
    src/Filter/filter.h \
    src/Filter/filterbank.h \
    src/Filter/filtersetting.h \
    src/Filter/filterwork.h \
    src/mainwindow.h

INCLUDEPATH += \
    src/Chart/

FORMS += \
    src/Chart/chartsetting.ui \
    src/Filter/filtersetting.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
