#ifndef LOBBIESSUBDIALOG_H
#define LOBBIESSUBDIALOG_H

#include <memory>

#include <QDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "helpers/helperstructs.h"

using std::unique_ptr;

namespace Ui {
class LobbiesSubDialog;
}

class LobbiesSubDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LobbiesSubDialog(QDialog *parent = nullptr);
    ~LobbiesSubDialog();

    enum configFlags { changeNickname, joinById, lobbyPasswordEnter };
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

    Ui::LobbiesSubDialog *ui;

    enum regExpFlags { lobbyRegExp, uniqueIdRegExp };
};

#endif // LOBBIESSUBDIALOG_H
