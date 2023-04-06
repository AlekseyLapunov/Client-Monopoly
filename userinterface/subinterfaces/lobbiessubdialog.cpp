#include "lobbiessubdialog.h"
#include "ui_lobbiessubdialog.h"

LobbiesSubDialog::LobbiesSubDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LobbiesSubDialog)
{
    m_interactionMode = -1;
    ui->setupUi(this);
}

LobbiesSubDialog::~LobbiesSubDialog()
{
    delete ui;
}

void LobbiesSubDialog::selfConfig(const short configFlag, const QString &carrier)
{
    ui->leInput->clear();
    m_interactionMode = configFlag;
    switch (configFlag)
    {
    case changeNickname:
        this->setRegExps(lobbyRegExp);
        ui->leInput->setEchoMode(QLineEdit::Normal);
        ui->leInput->setMaxLength(NICKNAME_MAX_LENGTH);
        writeWidgetTexts(sdStrings[sdNickname][sdWinTitle], sdStrings[sdNickname][sdApplyB],
                         sdStrings[sdNickname][sdRejectB], sdStrings[sdNickname][sdInfoString],
                         sdStrings[sdNickname][sdInputString]);
        break;
    case joinById:
        this->setRegExps(uniqueIdRegExp);
        ui->leInput->setEchoMode(QLineEdit::Normal);
        ui->leInput->setMaxLength(UNIQUE_ID_MAX_LENGTH);
        writeWidgetTexts(sdStrings[sdDirectJoin][sdWinTitle], sdStrings[sdDirectJoin][sdApplyB],
                         sdStrings[sdDirectJoin][sdRejectB], sdStrings[sdDirectJoin][sdInfoString],
                         sdStrings[sdDirectJoin][sdInputString]);
        break;
    case lobbyPasswordEnter:
        this->setRegExps(lobbyRegExp);
        ui->leInput->setEchoMode(QLineEdit::Password);
        ui->leInput->setMaxLength(LOBBY_PASSWORD_MAX_LENGTH);
        writeWidgetTexts(sdStrings[sdPasswordJoin][sdWinTitle], sdStrings[sdPasswordJoin][sdApplyB],
                         sdStrings[sdPasswordJoin][sdRejectB], sdStrings[sdPasswordJoin][sdInfoString] + carrier + "\":",
                         sdStrings[sdPasswordJoin][sdInputString]);
        break;
    default:
        QDialog::reject();
    }
}

void LobbiesSubDialog::setRegExps(const short regExpFlag)
{
    QRegularExpression* pRegExp;

    switch (regExpFlag)
    {
    case lobbyRegExp:
        pRegExp = new QRegularExpression(lobbyRegExpString);
        break;
    case uniqueIdRegExp:
        pRegExp = new QRegularExpression(uniqueIdRegExpString);
        break;
    default:
        pRegExp = nullptr;
        QDialog::reject();
    }

    QRegularExpressionValidator* pLobbyFilterValidator;

    pLobbyFilterValidator =  new QRegularExpressionValidator(*pRegExp, this);
    ui->leInput->setValidator(pLobbyFilterValidator);

}

void LobbiesSubDialog::writeWidgetTexts(const QString &windowTitle,
                                        const QString &bApplyString,
                                        const QString &bRejectString,
                                        const QString &lInfoString,
                                        const QString &leInputString)
{
    this->setWindowTitle(windowTitle);
    ui->bApply->setText(bApplyString);
    ui->bReject->setText(bRejectString);
    ui->lInfoText->setText(lInfoString);
    ui->leInput->setPlaceholderText(leInputString);
}

int LobbiesSubDialog::uniqueIdValue() const
{
    return m_uniqueIdValue;
}

void LobbiesSubDialog::accept()
{
    if(ui->leInput->text().isEmpty())
    {
        this->execErrorBox(nothingEntered);
        return;
    }
    switch (m_interactionMode)
    {
    case changeNickname:
        m_nicknameValue = ui->leInput->text();
        break;
    case joinById:
        m_uniqueIdValue = ui->leInput->text().toInt();
        break;
    case lobbyPasswordEnter:
        m_lobbyPasswordValue = ui->leInput->text();
        break;
    default:
        QDialog::reject();
    }
    QDialog::accept();
}

QString LobbiesSubDialog::nicknameValue() const
{
    return m_nicknameValue;
}

QString LobbiesSubDialog::lobbyPasswordValue() const
{
    return m_lobbyPasswordValue;
}
