#ifndef GAMEMANAGERWINDOW_H
#define GAMEMANAGERWINDOW_H

#include <QWidget>

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

private:
    Ui::GameManagerWindow *ui;
};

#endif // GAMEMANAGERWINDOW_H
