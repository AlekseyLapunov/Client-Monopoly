#ifndef BASEWIN_H
#define BASEWIN_H

#include <memory>

#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

#include "servercomm/servercommunicator.h"
#include "usermetainfo.h"
#include "helpers/common/sourcestrings.h"
#include "helpers/common/sourcestructs.h"

using std::unique_ptr;

typedef short dialogBoxRole;

#define REFRESH_LOBBY_INSIDE_DATA_EVERY_N_MS 5000
#define REFRESH_LOBBIES_LIST_EVERY_N_MS 8000

class BaseWin
{

public:
    BaseWin() {}
    ~BaseWin() {}

    virtual void windowDataRefresh() {}
    virtual void quitAppDialog() {}

protected:
    void setupPointers(unique_ptr<ServerCommunicator> &newServerPtr,
                       unique_ptr<UserMetaInfo> &newMetaInfoPtr)
    {
        this->m_pServer = &newServerPtr;
        this->m_pUserMetaInfo = &newMetaInfoPtr;
    }

    unique_ptr<ServerCommunicator>* pServer()
    {
        return m_pServer;
    }

    unique_ptr<UserMetaInfo>* pUserMetaInfo()
    {
        return m_pUserMetaInfo;
    }

    QTimer refreshDataTimer;

    int makeDialog(dialogBoxRole role, const QString &carrier = "", QWidget *parent = nullptr)
    {
        unique_ptr<QMessageBox> qmb;
        switch (role)
        {
        case QuitApp:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Выход из приложения",
                                       "Вы уверены, что хотите выйти из приложения?",
                                       {"Да", "Нет"}, parent);
            break;
        case ChangeAcc:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Смена аккаунта",
                                       "Вы уверены, что хотите сменить аккаунт?",
                                       {"Да", "Нет"}, parent);
            break;
        case JoinLobby:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Подключение",
                                       "Подключиться к лобби \"" + carrier + "\"?",
                                       {"Да", "Нет"}, parent);
            break;
        case LeaveLobby:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Выход из лобби",
                                       "Вы уверены, что хотите покинуть лобби?",
                                       {"Да", "Нет"}, parent);
            break;
        case RestoreSettings:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Восстановление настроек",
                                       "Восстановить последние\nзаписанные настройки?",
                                       {"Да", "Нет"}, parent);
            break;
        case StartGameNotReady:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Запуск матча",
                                       "Не все игроки готовы.\nЗапустить матч?",
                                       {"Да", "Нет"}, parent);
            break;
        case StartGameSettingsNotApplied:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Запуск матча",
                                       "Замечены неприменённые настройки.\nЗапустить матч на последних\n"
                                       "записанных настройках?",
                                       {"Да", "Нет"}, parent);
            break;
        case ExportSettingsNotApplied:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Экспорт настроек",
                                       "Замечены неприменённые настройки.\nСделать экспорт последних\n"
                                       "записанных настроек?",
                                       {"Да", "Нет"}, parent);
            break;
        case ActionOnPlayerSelected:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Применить действие",
                                       "Выбран игрок \"" + carrier + "\"",
                                       {"Исключить", "Сделать владельцем", "Закрыть"}, parent);
            break;
        case PlayerPromoteConfirmation:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Сделать владельцем",
                                       "Вы уверены, что хотите сделать\nигрока \"" + carrier + "\" владельцем лобби?",
                                       {"Да", "Нет"}, parent);
            break;
        case PlayerKickConfirmation:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Исключить из лобби",
                                       "Вы уверены, что хотите исключить\nигрока \"" + carrier + "\" из лобби?",
                                       {"Да", "Нет"}, parent);
            break;
        default:
            break;
        }
        return qmb->exec();
    }

    void execErrorBox(QString body, QWidget *parent = nullptr)
    {
        QMessageBox qmb(QMessageBox::Warning, ssErrorTitle, body, QMessageBox::NoButton, parent);
        qmb.exec();
    }

protected:
    enum DialogBoxVariations { QuitApp, ChangeAcc, JoinLobby,
                               LeaveLobby, RestoreSettings, StartGameNotReady,
                               StartGameSettingsNotApplied, ExportSettingsNotApplied,
                               ActionOnPlayerSelected, PlayerPromoteConfirmation,
                               PlayerKickConfirmation };

private:
    unique_ptr<QMessageBox> dialogBoxConstructor(QMessageBox::Icon icon,
                                                 const QString &boxName,
                                                 const QString &boxBodyText,
                                                 const QStringList buttonsText,
                                                 QWidget *parent)
    {
        unique_ptr<QMessageBox> qmb (new QMessageBox(icon, boxName, boxBodyText, QMessageBox::NoButton, parent));
        for(auto buttonName : buttonsText)
        {
            qmb->addButton(buttonName, QMessageBox::HelpRole);
        }
        return qmb;
    }

private:
    unique_ptr<ServerCommunicator> *m_pServer;
    unique_ptr<UserMetaInfo> *m_pUserMetaInfo;
};

#endif // BASEWIN_H
