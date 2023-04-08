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
    void apply3dDiceChoise(bool isUsing3dDice);

private:
    void resetHostInfo();

private:
    HostUserData m_hostInfo;
    bool m_isUsing3dDice;

};

#endif // USERMETAINFO_H
