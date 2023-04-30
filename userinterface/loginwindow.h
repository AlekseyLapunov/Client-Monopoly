#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QCoreApplication>

#include "helpers/basewin.h"
#include "menuwindow.h"

//#define DEBUG

namespace Ui { class LoginWindow; }

class LoginWindow : public QWidget, public BaseWin
{
    Q_OBJECT

public:
    LoginWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                QWidget *parent = nullptr);
    ~LoginWindow();

    void show();
    void hide();
    void quitAppDialog();

private slots:
    void googleLogin();
    void vkLogin();
    void quitApp();

private:
    void closeEvent(QCloseEvent *event);
    enum ServiceFlags { Google, Vk };
    void baseLogin(short flag);
    void switchToMenuWindow();

private:
    Ui::LoginWindow *ui;

    unique_ptr<MenuWindow> pMenuWindow;

};
#endif // LOGINWINDOW_H
