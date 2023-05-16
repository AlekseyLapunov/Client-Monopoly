QT += testlib
QT += gui core quick networkauth widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

HEADERS += \
    ../helpers/common/filemanager.h \
    ../helpers/common/sourcestructs.h \
    ../helpers/usermetainfo.h \
    tst_modules/tst_filemanager.h \
    tst_modules/tst_sourcestructs.h \
    tst_modules/tst_usermetainfo.h \
    testrunner.h

SOURCES +=  \
    ../helpers/common/filemanager.cpp \
    main.cpp
