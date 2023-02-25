#ifndef BASEWIN_H
#define BASEWIN_H

#include <memory>

#include <QString>
#include <QMessageBox>

#include "servercommunicator.h"
#include "usermetainfo.h"

using std::unique_ptr;

class BaseWin
{

public:
    BaseWin() {}
    ~BaseWin() {}

    virtual void windowDataRefresh() {}

protected:
    void setupPointers(unique_ptr<ServerCommunicator> &newServerPtr,
                       unique_ptr<UserMetaInfo> &newMetaInfoPtr)
    {
        this->m_pServer = &newServerPtr;
        this->m_pUserMetaInfo = &newMetaInfoPtr;
    }

    unique_ptr<ServerCommunicator>* pServer()
    {
        return m_pServer;
    }

    unique_ptr<UserMetaInfo>* pUserMetaInfo()
    {
        return m_pUserMetaInfo;
    }

    QMessageBox* makeDialogBox(QMessageBox::Icon icon,
                              const QString &boxName,
                              const QString &boxBodyText,
                              const QStringList buttonsText)
    {
        QMessageBox* qmb = new QMessageBox(icon, boxName, boxBodyText);
        for(auto buttonName : buttonsText)
        {
            qmb->addButton(buttonName, QMessageBox::HelpRole);
        }
        return qmb;
    }

private:
    unique_ptr<ServerCommunicator> *m_pServer;
    unique_ptr<UserMetaInfo> *m_pUserMetaInfo;

};

#endif // BASEWIN_H
