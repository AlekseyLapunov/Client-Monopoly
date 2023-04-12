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
}

void UserMetaInfo::apply3dDiceChoise(bool isUsing3dDice)
{
    this->m_isUsing3dDice = isUsing3dDice;
}

bool UserMetaInfo::get3dDiceChoise()
{
    return this->m_isUsing3dDice;
}

void UserMetaInfo::resetHostInfo()
{
    m_hostInfo.uniqueId = 0;
    m_hostInfo.nickname = "Nickname";
    m_hostInfo.rpCount = 0;
}
