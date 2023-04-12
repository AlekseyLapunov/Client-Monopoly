#ifndef LOBBIESSUBDIALOG_H
#define LOBBIESSUBDIALOG_H

#include <memory>

#include <QDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "helpers/basewin.h"

using std::unique_ptr;

namespace Ui {
class LobbiesSubDialog;
}

class LobbiesSubDialog : public QDialog, public BaseWin
{
    Q_OBJECT

public:
    explicit LobbiesSubDialog(QWidget *parent = nullptr);
    ~LobbiesSubDialog();

    enum SdConfigFlags { ChangeNickname, JoinById, LobbyPasswordEnter };
    void selfConfig(const short configFlag, const QString &carrier = "");

    QString lobbyPasswordValue() const;
    QString nicknameValue() const;
    QString uniqueIdValue() const;

private slots:
    void accept() override;

private:
    enum RegExpFlags { LobbyRegExp, UniqueIdRegExp };
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
    QString m_uniqueIdValue;

    Ui::LobbiesSubDialog *ui;
};

#endif // LOBBIESSUBDIALOG_H
