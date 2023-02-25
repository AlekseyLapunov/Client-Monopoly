QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    helpers/usermetainfo.cpp \
    main.cpp \
    helpers/servercommunicator.cpp \
    userinterface/loginwindow.cpp \
    userinterface/menuwindow.cpp

HEADERS += \
    helpers/basewin.h \
    helpers/helperstructs.h \
    helpers/usermetainfo.h \
    userinterface/loginwindow.h \
    helpers/servercommunicator.h \
    userinterface/menuwindow.h

FORMS += \
    userinterface/loginwindow.ui \
    userinterface/menuwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
