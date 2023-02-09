#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "helpers/servercommunicator.h"
#include "helpers/usermetainfo.h"
#include "userinterface/menuwindow.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    void sharePointers(ServerCommunicator *pServer, UserMetaInfo *pUser);

private slots:
    void googleLogin();
    void vkLogin();
    void quitApp();

private:
    Ui::LoginWindow *ui;

    UserMetaInfo *pUser = nullptr;
    ServerCommunicator *pServer = nullptr;

    void openMenuWindow();

};
#endif // LOGINWINDOW_H
