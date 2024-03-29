QT       += core gui quick network networkauth

LIBS += -L"../Client-Monopoly/protocol_buffers/protobuf_essentials/libs"

INCLUDEPATH  += "../Client-Monopoly/protocol_buffers/protobuf_essentials/include"

LIBS += -llibprotobuf -llibprotoc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game/gamehelpers/gamealgo.cpp \
    game/gamehelpers/gamestructs.cpp \
    game/gamehelpers/gametransmitterobject.cpp \
    game/models/fieldsgridmodel.cpp \
    game/models/lists/cellslist.cpp \
    game/models/lists/playergameinfolist.cpp \
    game/models/playersinfomodel.cpp \
    helpers/common/filemanager.cpp \
    userinterface/lobbywindow.cpp \
    main.cpp \
    servercomm/servercommunicator.cpp \
    userinterface/loginwindow.cpp \
    userinterface/menuwindow.cpp \
    userinterface/subinterfaces/gamemanagerwindow.cpp \
    userinterface/subinterfaces/menusubdialog.cpp \
    protocol_buffers/protobuf_generated/MonopolyGameMessages.pb.cc

HEADERS += \
    game/gamehelpers/gamealgo.h \
    game/gamehelpers/gamestructs.h \
    game/gamehelpers/gametransmitterobject.h \
    game/gamehelpers/modelsroles.h \
    game/models/fieldsgridmodel.h \
    game/models/lists/cellslist.h \
    game/models/lists/playergameinfolist.h \
    game/models/playersinfomodel.h \
    helpers/basewin.h \
    helpers/common/filemanager.h \
    helpers/common/sourcestrings.h \
    helpers/common/sourcestructs.h \
    helpers/usermetainfo.h \
    servercomm/servercommhelper.h \
    userinterface/lobbywindow.h \
    userinterface/loginwindow.h \
    servercomm/servercommunicator.h \
    userinterface/menuwindow.h \
    userinterface/subinterfaces/gamemanagerwindow.h \
    userinterface/subinterfaces/menusubdialog.h \
    protocol_buffers/protobuf_generated/MonopolyGameMessages.pb.h

FORMS += \
    userinterface/lobbywindow.ui \
    userinterface/loginwindow.ui \
    userinterface/menuwindow.ui \
    userinterface/subinterfaces/gamemanagerwindow.ui \
    userinterface/subinterfaces/menusubdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    html.qrc \
    media.qrc \
    qml.qrc
