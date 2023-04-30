#include "usermetainfo.h"

UserMetaInfo::UserMetaInfo()
{
    resetHostInfo();
}

HostUserData UserMetaInfo::getHostInfo()
{
    return m_hostInfo;
}

void UserMetaInfo::setHostInfo(HostUserData newHostInfo)
{
    this->m_hostInfo.uniqueId = newHostInfo.uniqueId;
    this->m_hostInfo.nickname = newHostInfo.nickname;
    this->m_hostInfo.rpCount = newHostInfo.rpCount;
    this->m_hostInfo.isGuest = newHostInfo.isGuest;
}

void UserMetaInfo::resetHostInfo()
{
    m_hostInfo.uniqueId = 0;
    m_hostInfo.nickname = "Nickname";
    m_hostInfo.rpCount = 0;
    m_hostInfo.isGuest = true;
}
