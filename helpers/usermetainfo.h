#ifndef USERMETAINFO_H
#define USERMETAINFO_H

#include <QString>

#include "common/sourcestructs.h"

class UserMetaInfo
{
public:
    UserMetaInfo();

    HostUserData getHostInfo();
    void setHostInfo(HostUserData newHostInfo);

private:
    HostUserData m_hostInfo;
};

#endif // USERMETAINFO_H
