#-------------------------------------------------
#
# sajat szoveg
#
#-------------------------------------------------

QT       += widgets core gui
QT       += xml

TEMPLATE = app
TARGET = PVRDecrypt

RCC_DIR = "build"
UI_DIR = "build"
MOC_DIR = "build"
OBJECTS_DIR = "build"

CONFIG(debug, debug|release) {
    DESTDIR = "debug"
}

CONFIG(release, debug|release) {
    DESTDIR = "release"
}

INCLUDEPATH += "../../include"

#Standard module files
FORMS    += mainwindow.ui \
    qmetainfodialog.ui
SOURCES  += main.cpp\
        mainwindow.cpp \
    aes.c \
    qmetainforeader.cpp \
    qmetainfodialog.cpp \
    mdb.cpp \
    qdrmdecrypt.cpp \
    readinf.cpp \
    tdetails.cpp
HEADERS  += mainwindow.h \
    qmetainforeader.h \
    qmetainfodialog.h \
    mdb.h \
    qdrmdecrypt.h \
    readinf.h \
    tdetails.h
#---------------------------------------------------

#BASE module files
SOURCES  +=
HEADERS  +=

#---------------------------------------------------

RC_FILE += \
    PVRDecrypt.rc

RESOURCES += \
    PVRDecrypt.qrc

