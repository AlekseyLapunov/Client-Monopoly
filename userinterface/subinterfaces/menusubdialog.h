#ifndef MENUSUBDIALOG_H
#define MENUSUBDIALOG_H

#include <memory>

#include <QDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "helpers/basewin.h"

using std::unique_ptr;

namespace Ui {
class MenuSubDialog;
}

class MenuSubDialog : public QDialog, public BaseWin
{
    Q_OBJECT

public:
    explicit MenuSubDialog(QWidget *parent = nullptr);
    ~MenuSubDialog();

    enum SdConfigFlags { ChangeNickname, JoinById, LobbyPasswordEnter };
    void selfConfig(const short configFlag, const QString &carrier = "");

    QString lobbyPasswordValue() const;
    QString nicknameValue() const;
    int uniqueIdValue() const;

private slots:
    void accept() override;

private:
    void setRegExps(const short regExpFlag);
    void writeWidgetTexts(const QString &windowTitle,
                          const QString &bApplyString,
                          const QString &bRejectString,
                          const QString &lInfoString,
                          const QString &leInputString);

private:
    int m_interactionMode;

    QString m_lobbyPasswordValue;
    QString m_nicknameValue;
    int m_uniqueIdValue;

    Ui::MenuSubDialog *ui;
};

#endif // MENUSUBDIALOG_H
