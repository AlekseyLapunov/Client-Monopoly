#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

#include "subinterfaces/lobbiessubdialog.h"
#include "helpers/basewin.h"

typedef short dialogCode;

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
    void showAbout();

private:
    void setupLobbiesTable();
    void setupLobbiesFilter();
    void displayHostShortInfo();
    void setButtonsState(const bool makeEnabled);
    void tableClear(QTableWidget &table);
    void tableSetupFill(QTableWidget &table, const vector<LobbyShortInfo> &contentVec, const QString &filter = "");
    void switchJoinByItem(const QTableWidgetItem &item);

    enum DialogCodes { NoPassword, PassRejected, PassEntered };
    dialogCode checkIfPassworded(const QTableWidgetItem &item);

private:
    Ui::MenuWindow *ui;
    unique_ptr<LobbiesSubDialog> pSubDialog;
    int curUniqueId = -1;
};

#endif // MENUWINDOW_H
