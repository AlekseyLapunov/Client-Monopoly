#ifndef GAMEMANAGERWINDOW_H
#define GAMEMANAGERWINDOW_H

#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "helpers/basewin.h"
#include "helpers/gamemodels/fieldsgrid.h"
#include "helpers/gamemodels/gamestructs/gamestructs.h"

#define DEBUG

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
    void applyFirstGameContext(/*GameContext& gameContext*/);

private:
    Ui::GameManagerWindow *ui;
    QQmlApplicationEngine* qmlEngine = nullptr;
};

#endif // GAMEMANAGERWINDOW_H
