#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

#include "subinterfaces/lobbiessubdialog.h"
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
    void lobbyClicked(QTableWidgetItem *itemClicked);
    void applyLobbyFilter(QString textChanged);
    void joinToLobby(QTableWidgetItem *itemActivated);
    void joinToLobby();
    void joinIdDialog();

private:
    void setupLobbiesTable();
    void setupLobbiesFilter();
    void displayHostShortInfo();
    void setButtonsState(bool makeEnabled);
    void tableClear(QTableWidget &table);
    void tableSetupFill(QTableWidget &table, const vector<LobbyShortInfo> &contentVec, const QString &filter = "");
    void joinById(int uniqueId);

private:
    Ui::MenuWindow *ui;
    unique_ptr<LobbiesSubDialog> pSubDialog;
    int curUniqueId;
};

#endif // MENUWINDOW_H
