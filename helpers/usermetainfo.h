#ifndef USERMETAINFO_H
#define USERMETAINFO_H

#include <QString>

class UserMetaInfo
{
public:
    UserMetaInfo();


    QString name() const;
    void setName(const QString &newName);

    int rpCount() const;
    void setRpCount(int newRpCount);

private:
    QString m_name;
    int m_rpCount;

};

#endif // USERMETAINFO_H
