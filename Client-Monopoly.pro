QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    helpers/common/filemanager.cpp \
    helpers/usermetainfo.cpp \
    userinterface/lobbywindow.cpp \
    main.cpp \
    helpers/servercommunicator.cpp \
    userinterface/loginwindow.cpp \
    userinterface/menuwindow.cpp \
    userinterface/subinterfaces/gamemanagerwindow.cpp \
    userinterface/subinterfaces/lobbiessubdialog.cpp

HEADERS += \
    helpers/basewin.h \
    helpers/common/filemanager.h \
    helpers/common/sourcestrings.h \
    helpers/common/sourcestructs.h \
    helpers/usermetainfo.h \
    userinterface/lobbywindow.h \
    userinterface/loginwindow.h \
    helpers/servercommunicator.h \
    userinterface/menuwindow.h \
    userinterface/subinterfaces/gamemanagerwindow.h \
    userinterface/subinterfaces/lobbiessubdialog.h

FORMS += \
    userinterface/lobbywindow.ui \
    userinterface/loginwindow.ui \
    userinterface/menuwindow.ui \
    userinterface/subinterfaces/gamemanagerwindow.ui \
    userinterface/subinterfaces/lobbiessubdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
