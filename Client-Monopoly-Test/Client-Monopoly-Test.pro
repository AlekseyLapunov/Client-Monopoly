QT += testlib
QT += gui core quick networkauth widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

HEADERS += \
    ../helpers/common/filemanager.h

SOURCES +=  \
    tst_filemanagertest.cpp \
    ../helpers/common/filemanager.cpp
