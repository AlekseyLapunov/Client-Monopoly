#ifndef TST_USERMETAINFO_H
#define TST_USERMETAINFO_H

#include <QtTest>

#include "../helpers/usermetainfo.h"

class UserMetaTest : public QObject
{
    Q_OBJECT

public:
    UserMetaTest()
    {

    }

    ~UserMetaTest()
    {

    }

private slots:

    void test_setGetHostInfo()
    {
        HostUserData temp = {1, "temp", 100, false};

        userMetaObject.setHostInfo(temp);

        HostUserData gotHostUserData = userMetaObject.getHostInfo();

        QVERIFY(temp == gotHostUserData);

        temp.isGuest = true;

        QVERIFY(temp != gotHostUserData);

        temp.isGuest = false;
        temp.nickname = "not_temp";

        userMetaObject.setHostInfo(temp);

        QVERIFY(userMetaObject.getHostInfo() == temp);
    }

private:
    UserMetaInfo userMetaObject;

};

#endif // TST_USERMETAINFO_H
