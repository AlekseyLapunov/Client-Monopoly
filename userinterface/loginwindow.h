#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QCoreApplication>

#include "helpers/basewin.h"
#include "menuwindow.h"

namespace Ui { class LoginWindow; }

typedef short serviceFlag;

class LoginWindow : public QWidget, public BaseWin
{
    Q_OBJECT

public:
    LoginWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void googleLogin();
    void vkLogin();
    void quitApp();

private:
    enum ServiceFlags { Google, Vk };
    void baseLogin(serviceFlag flag);
    void switchToMenuWindow();

private:
    Ui::LoginWindow *ui;

    unique_ptr<MenuWindow> pMenuWindow;

};
#endif // LOGINWINDOW_H
