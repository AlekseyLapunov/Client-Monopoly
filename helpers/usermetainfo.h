#ifndef USERMETAINFO_H
#define USERMETAINFO_H

#include <QString>

#include "common/sourcestructs.h"

class UserMetaInfo
{
public:
    UserMetaInfo() {};

    HostUserData getHostInfo()
    {
        return m_hostInfo;
    }

    void setHostInfo(HostUserData newHostInfo)
    {
        this->m_hostInfo.uniqueId = newHostInfo.uniqueId;
        this->m_hostInfo.nickname = newHostInfo.nickname;
        this->m_hostInfo.rpCount = newHostInfo.rpCount;
        this->m_hostInfo.isGuest = newHostInfo.isGuest;
    }

private:
    HostUserData m_hostInfo;
};

#endif // USERMETAINFO_H
