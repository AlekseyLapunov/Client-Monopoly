#ifndef SOURCESTRINGS_H
#define SOURCESTRINGS_H

#include <string.h>

#include <QString>

using std::string;

// Regular expression QStrings
static const QString lobbyRegExpString = "^[a-zA-Zа-яА-ЯёЁ0-9]+?( ?[a-zA-Zа-яА-ЯёЁ0-9])*$";
static const QString uniqueIdRegExpString = "^[0-9]*$";

// Classes Names for the exception thrower specifying
static const string loginWindowClassName = "LoginWindow: ";
static const string menuWindowClassName = "MenuWindow: ";
static const string lSubDialogClassName = "LobbiesSubDialog: ";
static const string lobbyWindowClassName = "LobbyWindow: ";
static const string serverCommClassName = "ServerCommunicator: ";

// QMessageBox
static const QString errorTitle = "Ошибка";
static const QString errorBody = "Возникла ошибка при\nвыполнении действия";
static const QString nothingEntered = "Ничего не введено";
static const QString lobbyNotSelected = "Лобби не выбрано";

// MenuWindow
static const QString menuWindowTitle = "Главное меню";
static const QString passColumnYes = "Есть";
static const QString passColumnNo = "Нет";
static const QString playersColumnSlash = " / ";
static const QString statusBarSubMessage = " | ID для подключения: ";
static const QString lobbiesInTotal = "Всего лобби: ";
static const QString lobbiesNotPassworded = ". Открытых комнат: ";
static const QString aboutTitle = "О приложении";
static const QString aboutBody = "Приложение \"Игра Монополия\" разработано\n"
                                 "студентами ИКИТ СФУ Лапуновым А.Ю. и Звягиным С.А.\n"
                                 "в рамках выпускной квалификационной работы.\n"
                                 "Клиентское приложение: Лапунов А.Ю.\n"
                                 "Серверное приложение: Звягин С.А.\n"
                                 "Руководитель ВКР: ст. преподаватель каф. ВТ Васильев В.С.";
static const QStringList lobbyTableLabels {"Название",
                                           "Пароль",
                                           "Игроки"};

// runtime_error
static const string errorPtrLink = "Проблема при передаче указателей";
static const string lobbyNotFound = "Лобби не найдено";
static const string googleAuthFail = "Не удалось войти\nчерез Google аккаунт";
static const string vkAuthFail = "Не удалось войти\nчерез VK аккаунт";

#endif // SOURCESTRINGS_H
