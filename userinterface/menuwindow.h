#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

#include "helpers/basewin.h"

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow, public BaseWin
{
    Q_OBJECT

public:
    explicit MenuWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                        unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                        QWidget *parent = nullptr);
    ~MenuWindow();

    void windowDataRefresh();

signals:
    void goToLoginWindow();

private slots:
    void quitApp();
    void changeAcc();
    void apply3dDiceState();

private:
    void setupLobbiesFilter();
    void setupHostShortInfo();
    void setButtonsState(bool areToBeAccessible);

private:
    Ui::MenuWindow *ui;
};

#endif // MENUWINDOW_H