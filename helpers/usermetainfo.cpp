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
    this->m_hostInfo.uniqueUserId = newHostInfo.uniqueUserId;
    this->m_hostInfo.userName = newHostInfo.userName;
    this->m_hostInfo.userRpCount = newHostInfo.userRpCount;
}

void UserMetaInfo::resetHostInfo()
{
    m_hostInfo.uniqueUserId = -1;
    m_hostInfo.userName = "Nickname";
    m_hostInfo.userRpCount = -1;
}
