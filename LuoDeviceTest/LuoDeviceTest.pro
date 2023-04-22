QT       += core gui serialport printsupport charts network

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
    src/Communicate/comcommunicate.cpp \
    src/Communicate/sigtcpexport.cpp \
    src/Communicate/tcpservercommunicate.cpp \
    src/FileSave/BDFHeader.cpp \
    src/FileSave/filesave.cpp \
    src/Filter/DesignButter/filterDesign.cpp \
    src/Filter/DesignButter/src/DesignButter.c \
    src/Filter/DesignButter/src/DesignButter_emxutil.c \
    src/Filter/DesignButter/src/rtGetInf.c \
    src/Filter/DesignButter/src/rtGetNaN.c \
    src/Filter/DesignButter/src/rt_nonfinite.c \
    src/Filter/filtersetting.cpp \
    src/Filter/filterwork.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/Chart/chartsetting.h \
    src/Chart/chartwork.h \
    src/Chart/qcustomplot.h \
    src/Communicate/comcommunicate.h \
    src/Communicate/sigtcpexport.h \
    src/Communicate/tcpservercommunicate.h \
    src/ConstData.h \
    src/FileSave/BDFHeader.h \
    src/FileSave/filesave.h \
    src/Filter/DesignButter/filterDesign.h \
    src/Filter/DesignButter/src/DesignButter.h \
    src/Filter/DesignButter/src/DesignButter_emxutil.h \
    src/Filter/DesignButter/src/DesignButter_types.h \
    src/Filter/DesignButter/src/rtGetInf.h \
    src/Filter/DesignButter/src/rtGetNaN.h \
    src/Filter/DesignButter/src/rt_defines.h \
    src/Filter/DesignButter/src/rt_nonfinite.h \
    src/Filter/DesignButter/src/rtwtypes.h \
    src/Filter/filter.h \
    src/Filter/filterbank.h \
    src/Filter/filtersetting.h \
    src/Filter/filterwork.h \
    src/mainwindow.h

INCLUDEPATH += \
    src/Chart/ \
    D:\Polyspace\R2021a\extern\include

FORMS += \
    src/Chart/chartsetting.ui \
    src/Communicate/sigtcpexport.ui \
    src/Filter/filtersetting.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
