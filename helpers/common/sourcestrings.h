#ifndef SOURCESTRINGS_H
#define SOURCESTRINGS_H

#include <string.h>

#include <QString>
#include <QStringList>

using std::string;

// Regular expression QStrings
static const QString ssLobbyRegExpString    = "^[a-zA-Zа-яА-ЯёЁ0-9]+?( ?[a-zA-Zа-яА-ЯёЁ0-9])*$";
static const QString ssUniqueIdRegExpString = "^[0-9]*$";

// Classes Names for the exception thrower specifying
static const string ssLoginWindowClassName      = "LoginWindow: ";
static const string ssMenuWindowClassName       = "MenuWindow: ";
static const string ssLobbiesSubDialogClassName = "LobbiesSubDialog: ";
static const string ssLobbyWindowClassName      = "LobbyWindow: ";
static const string ssServerCommClassName       = "ServerCommunicator: ";

// QMessageBox
static const QString ssErrorTitle       = "Ошибка";
static const QString ssErrorBody        = "Возникла ошибка при выполнении действия";
static const QString ssNothingEntered   = "Ничего не введено";
static const QString ssLobbyNotSelected = "Лобби не выбрано";

// MenuWindow
static const QString ssMenuWindowTitle      = "Главное меню";
static const QString ssPassColumnYes        = "Есть";
static const QString ssPassColumnNo         = "Нет";
static const QString ssPlayersColumnSlash   = " / ";
static const QString ssStatusBarSubMessage  = " | ID для подключения: ";
static const QString ssLobbiesInTotal       = "Всего лобби: ";
static const QString ssLobbiesNotPassworded = ". Открытых комнат: ";
static const QString ssAboutTitle           = "О приложении";
static const QString ssAboutBody            = "Приложение \"Игра Монополия\" разработано\n"
                                              "студентами ИКИТ СФУ Лапуновым А.Ю. и Звягиным С.А.\n"
                                              "в рамках выпускной квалификационной работы.\n"
                                              "Клиентское приложение: Лапунов А.Ю.\n"
                                              "Серверное приложение: Звягин С.А.\n"
                                              "Руководитель ВКР: ст. преподаватель каф. ВТ ИКИТ СФУ Васильев В.С.";
static const QStringList ssLobbyTableLabels { "Название",
                                              "Пароль",
                                              "Игроки"};

// LobbyWindow
static const QString ssDeleteLobbyText      = "Удалить лобби";
static const QString ssRankedLobby          = "Рейтинговое лобби";
static const QString ssAverageRp            = "Среднее RP: ";
static const QStringList ssUsersTableLabels { "Псевдоним",
                                              "RP",
                                              "Готов" };
static const QString ssUserIsReady          = "Да";
static const QString ssUserNotReady         = "Нет";
static const QString ssLobbyOfPlayer        = "Лобби игрока ";
static const QString ssMyLobby              = "Своё лобби ";
static const QString ssLobbyVisibility      = "Видимость лобби: ";
static const QString ssLobbyHidden          = "Скрытое";
static const QString ssLobbyVisible         = "Открытое";

// Sub-dialog strings
enum SdMode  { SdNickname, SdDirectJoin, SdPasswordJoin };
enum SdElems { SdWinTitle, SdApplyB, SdRejectB, SdInfoString, SdInputString };
/// Changing nickname
static const QStringList ssSubDialogChangeNickname = { "Сменить псевдоним", "Применить",
                                                       "Отмена", "Введите новый псевдоним:",
                                                       "Псевдоним" };
/// Joining by ID
static const QStringList ssSubDialogJoinById       = { "Подключение по ID", "Подключиться",
                                                       "Отмена", "Введите ID комнаты:", "ID" };
/// Joining with password
static const QStringList ssSubDialogPasswordedJoin = { "Ввод пароля к лобби", "Подключиться",
                                                       "Отмена", "Введите пароль комнаты \"",
                                                       "Пароль" };
static const QStringList ssSdStrings[]             = { ssSubDialogChangeNickname,
                                                       ssSubDialogJoinById,
                                                       ssSubDialogPasswordedJoin };

// filemanager.h
static const QString ssLocalDirPath           = "local/";
static const QString ssUserMetaFileName       = "usermeta.json";
static const QString ssRankedSettingsFileName = "default_ranked_settings.json";
static const QString ssLastSettingsFileName   = "last_settings.json";
static const QString ssCaptionImportSettings  = "Импортировать настройки лобби";
static const QString ssCaptionExportSettings  = "Экспортировать настройки лобби";
static const QString ssJsonFilter             = "JSON file (*.json)";

// JSON keys
enum JsonKeysIter           { LobbySystemIter, GameSettingsIter };
enum JsonKeysLobbySystemId  { UniqueId, LobbyName, LobbyPassword, MaxPlayersCount, OwnerUniqueId, IsPrivate };
enum JsonKeysGameSettingsId { TurnTime, MaxMoney, IsMaxMoneyInfinite, MaxTurns, AreMaxTurnsInfinite };
static const QStringList ssJsonKeysLobbySystem      = { "uniqueId", "lobbyName", "lobbyPassword", "maxPlayersCount",
                                                        "ownerUniqueId", "isPrivate" };
static const QStringList ssJsonKeysGameSettings     = { "turnTime", "maxMoney", "isMaxMoneyInfinite",
                                                        "maxTurns", "areMaxTurnsInfinite" };
static const QStringList ssJsonKeysLobbySettings[]  = { ssJsonKeysLobbySystem, ssJsonKeysGameSettings };
static const QStringList ssJsonObjectsId            = { "LobbySystemInfo", "GameSettingsInfo" };

// runtime_error
enum RunTimeErrorId { PtrLinkFail, LobbyNotFound, GoogleAuthFail, VkAuthFail,
                      AlreadyHasLobby, AlreadyInQueue, ApplyNicknameFail,
                      ApplySettingsFail, ToggleReadyFail, StartGameFail,
                      ToggleVisibilityFail, JsonParseError };
static const string ssRuntimeErrors[] = {
                                            "Проблема при передаче указателей",
                                            "Лобби не найдено",
                                            "Не удалось войти через Google аккаунт",
                                            "Не удалось войти через VK аккаунт",
                                            "Лобби уже существует",
                                            "Вы уже находитесь в очереди",
                                            "Не удалось сменить псевдоним",
                                            "Не удалось применить настройки",
                                            "Не удалось переключить готовность",
                                            "Не удалось запустить матч",
                                            "Не удалось переключить видимость лобби",
                                            "Ошибка при обработке представленного JSON-файла"
                                        };
#endif // SOURCESTRINGS_H








