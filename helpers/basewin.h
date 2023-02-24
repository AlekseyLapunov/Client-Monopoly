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

    void sharePointers(unique_ptr<ServerCommunicator> &newServerPtr,
                       unique_ptr<UserMetaInfo> &newMetaInfoPtr)
    {
        this->pServer = &newServerPtr;
        this->pUserMetaInfo = &newMetaInfoPtr;
    }

protected:
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

protected:
    unique_ptr<ServerCommunicator> *pServer;
    unique_ptr<UserMetaInfo> *pUserMetaInfo;

};

#endif // BASEWIN_H
