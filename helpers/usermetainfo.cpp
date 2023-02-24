#include "usermetainfo.h"

UserMetaInfo::UserMetaInfo()
{

}

QString UserMetaInfo::name() const
{
    return m_name;
}

void UserMetaInfo::setName(const QString &newName)
{
    m_name = newName;
}

int UserMetaInfo::rpCount() const
{
    return m_rpCount;
}

void UserMetaInfo::setRpCount(int newRpCount)
{
    m_rpCount = newRpCount;
}
