#ifndef BASEWIN_H
#define BASEWIN_H

#include <memory>

#include <QString>
#include <QMessageBox>

#include "servercommunicator.h"
#include "usermetainfo.h"
#include "helpers/common/sourcestrings.h"
#include "helpers/common/sourcestructs.h"

using std::unique_ptr;

typedef short dialogBoxRole;

class BaseWin
{

public:
    BaseWin() {}
    ~BaseWin() {}

    virtual void windowDataRefresh() {}

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

    int makeDialog(dialogBoxRole role, const QString &carrier = "")
    {
        unique_ptr<QMessageBox> qmb;
        switch (role)
        {
        case QuitApp:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Выход из приложения",
                                       "Вы уверены, что хотите выйти из приложения?",
                                       {"Да", "Нет"});
            break;
        case ChangeAcc:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Смена аккаунта",
                                       "Вы уверены, что хотите сменить аккаунт?",
                                       {"Да", "Нет"});
            break;
        case JoinLobby:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Подключение",
                                       "Подключиться к лобби \"" + carrier + "\"?",
                                       {"Да", "Нет"});
            break;
        case LeaveLobby:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Выход из лобби",
                                       "Вы уверены, что хотите покинуть лобби?",
                                       {"Да", "Нет"});
            break;
        case RestoreSettings:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Восстановление настроек",
                                       "Восстановить последние\nзаписанные настройки?",
                                       {"Да", "Нет"});
            break;
        case StartGameNotReady:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Запуск матча",
                                       "Не все игроки готовы.\nЗапустить матч?",
                                       {"Да", "Нет"});
            break;
        case StartGameSettingsNotApplied:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Запуск матча",
                                       "Замечены неприменённые настройки.\nЗапустить матч на последних\n"
                                       "записанных настройках?",
                                       {"Да", "Нет"});
            break;
        case ExportSettingsNotApplied:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Экспорт настроек",
                                       "Замечены неприменённые настройки.\nСделать экспорт последних\n"
                                       "записанных настроек?",
                                       {"Да", "Нет"});
            break;
        case PlayerSelected:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Применить действие",
                                       "Выбран игрок \"" + carrier + "\"",
                                       {"Исключить", "Сделать владельцем", "Закрыть"});
            break;
        case PlayerPromoteConfirmation:
            qmb = dialogBoxConstructor(QMessageBox::Question,
                                       "Сделать владельцем",
                                       "Вы уверены, что хотите сделать\nигрока \"" + carrier + "\" владельцем лобби?",
                                       {"Да", "Нет"});
            break;
        default:
            break;
        }
        return qmb->exec();
    }

    void execErrorBox(QString body)
    {
        QMessageBox qmb(QMessageBox::Warning, ssErrorTitle, body);
        qmb.exec();
    }

protected:
    enum DialogBoxVariations { QuitApp, ChangeAcc, JoinLobby,
                               LeaveLobby, RestoreSettings, StartGameNotReady,
                               StartGameSettingsNotApplied, ExportSettingsNotApplied,
                               PlayerSelected, PlayerPromoteConfirmation };

private:
    unique_ptr<QMessageBox> dialogBoxConstructor(QMessageBox::Icon icon,
                              const QString &boxName,
                              const QString &boxBodyText,
                              const QStringList buttonsText)
    {
        unique_ptr<QMessageBox> qmb (new QMessageBox(icon, boxName, boxBodyText));
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
