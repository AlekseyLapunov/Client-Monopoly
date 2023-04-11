#include "lobbiessubdialog.h"
#include "ui_lobbiessubdialog.h"

LobbiesSubDialog::LobbiesSubDialog(QWidget *parent) :
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
    case SdConfigFlags::ChangeNickname:
        setRegExps(LobbyRegExp);
        ui->leInput->setEchoMode(QLineEdit::Normal);
        ui->leInput->setMaxLength(NICKNAME_MAX_LEN);
        writeWidgetTexts(ssSdStrings[SdNickname][SdWinTitle], ssSdStrings[SdNickname][SdApplyB],
                         ssSdStrings[SdNickname][SdRejectB], ssSdStrings[SdNickname][SdInfoString],
                         ssSdStrings[SdNickname][SdInputString]);
        break;
    case SdConfigFlags::JoinById:
        setRegExps(UniqueIdRegExp);
        ui->leInput->setEchoMode(QLineEdit::Normal);
        ui->leInput->setMaxLength(UNIQUE_ID_MAX_LEN);
        writeWidgetTexts(ssSdStrings[SdDirectJoin][SdWinTitle], ssSdStrings[SdDirectJoin][SdApplyB],
                         ssSdStrings[SdDirectJoin][SdRejectB], ssSdStrings[SdDirectJoin][SdInfoString],
                         ssSdStrings[SdDirectJoin][SdInputString]);
        break;
    case SdConfigFlags::LobbyPasswordEnter:
        setRegExps(LobbyRegExp);
        ui->leInput->setEchoMode(QLineEdit::Password);
        ui->leInput->setMaxLength(LOBBY_PASSWORD_MAX_LEN);
        writeWidgetTexts(ssSdStrings[SdPasswordJoin][SdWinTitle], ssSdStrings[SdPasswordJoin][SdApplyB],
                         ssSdStrings[SdPasswordJoin][SdRejectB], ssSdStrings[SdPasswordJoin][SdInfoString] + carrier + "\":",
                         ssSdStrings[SdPasswordJoin][SdInputString]);
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
    case RegExpFlags::LobbyRegExp:
        pRegExp = new QRegularExpression(ssLobbyRegExpString);
        break;
    case RegExpFlags::UniqueIdRegExp:
        pRegExp = new QRegularExpression(ssUniqueIdRegExpString);
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
    setWindowTitle(windowTitle);
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
        execErrorBox(ssNothingEntered, this);
        return;
    }
    switch (m_interactionMode)
    {
    case SdConfigFlags::ChangeNickname:
        m_nicknameValue = ui->leInput->text();
        break;
    case SdConfigFlags::JoinById:
        m_uniqueIdValue = ui->leInput->text().toInt();
        break;
    case SdConfigFlags::LobbyPasswordEnter:
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
