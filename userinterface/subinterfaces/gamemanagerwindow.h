#ifndef GAMEMANAGERWINDOW_H
#define GAMEMANAGERWINDOW_H

#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "helpers/basewin.h"

namespace Ui {
class GameManagerWindow;
}

class GameManagerWindow : public QWidget, BaseWin
{
    Q_OBJECT

public:
    explicit GameManagerWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                               unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                               QWidget *parent = nullptr);
    ~GameManagerWindow();

    void show();
    void quitAppDialog();
    void closeEvent(QCloseEvent *event);

private:
    Ui::GameManagerWindow *ui;
    QQmlApplicationEngine* qmlEngine = nullptr;
};

#endif // GAMEMANAGERWINDOW_H
