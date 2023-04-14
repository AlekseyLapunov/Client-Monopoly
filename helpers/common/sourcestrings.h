#ifndef SOURCESTRINGS_H
#define SOURCESTRINGS_H

#include <string.h>

#include <QString>
#include <QStringList>
#include <QMap>

using std::string;

// Regular expressions
enum RegExpFlags {LobbyNameRegExp, NicknameRegExp, LobbyPasswordRegExp, LobbyUniqueIdRegExp};
static const QMap<short, QString> ssRegExps = {
                                                {LobbyNameRegExp,       "^((\\S+)(\\s?(\\S+))*)$"},
                                                {NicknameRegExp,        "^((\\S+)(\\s?(\\S+))*)$"},
                                                {LobbyPasswordRegExp,   "^(\\S|\\s)+$"},
                                                {LobbyUniqueIdRegExp,   "^[a-zA-Z0-9]*$"}
                                              };

// QMessageBox
static const QString ssErrorTitle       = "Ошибка";
static const QString ssErrorBody        = "Возникла ошибка при выполнении действия";
static const QString ssNothingEntered   = "Ничего не введено";
static const QString ssLobbyNotSelected = "Лобби не выбрано";
static const QString ssNotEnoughPlayers = "Недостаточно игроков\nдля старта игры";

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
static const QString ssLobbyIdPrefix        = "ID ";

// Sub-dialog strings
enum SdMode  { SdNickname, SdDirectJoin, SdPasswordJoin };
struct SubDialogStrings
{
    QString winTitle;
    QString applyButtonText;
    QString rejectButtonText;
    QString infoText;
    QString hintText;
};
/// Changing nickname
static const SubDialogStrings ssSubDialogChangeNickname = {
                                                            "Сменить псевдоним",
                                                            "Применить",
                                                            "Отмена",
                                                            "Введите новый псевдоним:",
                                                            "Псевдоним"
                                                          };
/// Joining by ID
static const SubDialogStrings ssSubDialogJoinById       = {
                                                            "Подключение по ID",
                                                            "Подключиться",
                                                            "Отмена",
                                                            "Введите ID комнаты:",
                                                            "ID"
                                                          };
/// Joining with password
static const SubDialogStrings ssSubDialogPasswordedJoin = {
                                                            "Ввод пароля к лобби",
                                                            "Подключиться",
                                                            "Отмена",
                                                            "Введите пароль комнаты",
                                                            "Пароль"
                                                          };
static const QMap<short, SubDialogStrings> ssSdStrings  = {
                                                            {SdNickname, ssSubDialogChangeNickname},
                                                            {SdDirectJoin, ssSubDialogJoinById},
                                                            {SdPasswordJoin, ssSubDialogPasswordedJoin}
                                                          };

// filemanager.h
static const QString ssLocalDirPath           = "local/";
static const QString ssUserMetaFileName       = "usermeta.json";
static const QString ssRankedSettingsFileName = "default_ranked_settings.json";
static const QString ssLastSettingsFileName   = "last_settings.json";
static const QString ssCaptionImportSettings  = "Импортировать настройки лобби";
static const QString ssCaptionExportSettings  = "Экспортировать настройки лобби";
static const QString ssJsonFilter             = "JSON file (*.json)";

// JSON keys
enum JsonKeysUserMeta        { Uses3dDice };
enum JsonKeysLobbySettingsId { UniqueId, LobbyName, LobbyPassword, MaxPlayersCount, OwnerUniqueId, IsTimerActive,
                               SessionAddress, SessionPort, TurnTime, MaxMoney, IsMaxMoneyInfinite, MaxTurns, AreMaxTurnsInfinite };
static const QMap<short, QString> ssJsonKeysLobbySettingsIds  = {
                                                                    {UniqueId,              "uniqueId"},
                                                                    {LobbyName,             "lobbyName"},
                                                                    {LobbyPassword,         "lobbyPassword"},
                                                                    {MaxPlayersCount,       "maxPlayersCount"},
                                                                    {OwnerUniqueId,         "ownerUniqueId"},
                                                                    {IsTimerActive,         "isTimerActive"},
                                                                    {SessionAddress,        "sessionAddress"},
                                                                    {SessionPort,           "sessionPort"},
                                                                    {TurnTime,              "turnTime"},
                                                                    {MaxMoney,              "maxMoney"},
                                                                    {IsMaxMoneyInfinite,    "isMaxMoneyInfinite"},
                                                                    {MaxTurns,              "maxTurns"},
                                                                    {AreMaxTurnsInfinite,   "areMaxTurnsInfinite"}
                                                                };
static const QMap<short, QString> ssJsonUserMeta              = {
                                                                    {Uses3dDice, "uses3dDice"}
                                                                };


// Classes Names for the exception thrower specifying
enum ClassesNames { LoginWindowCN, MenuWindowCN, LobbiesSubDialogCN, LobbyWindowCN,
                    ServerCommCN, GameManagerCN, FileManagerCN };
static const QMap<short, string> ssClassNames = {
                                                    {LoginWindowCN,      "LoginWindow: "},
                                                    {MenuWindowCN,       "MenuWindow: "},
                                                    {LobbiesSubDialogCN, "LobbiesSubDialog: "},
                                                    {LobbyWindowCN,      "LobbyWindow: "},
                                                    {ServerCommCN,       "ServerCommunicator: "},
                                                    {GameManagerCN,      "GameManagerWindow: "},
                                                    {FileManagerCN,      "FileManager: "}
                                                };

// runtime_error
enum RunTimeErrorId { PtrLinkFail, LobbyNotFound, GoogleAuthFail, VkAuthFail,
                      AlreadyHasLobby, AlreadyInQueue, ApplySettingsFail,
                      ToggleReadyFail, StartGameFail, JsonParseError, KickPlayerFail,
                      PromotePlayerFail, LastSettingsFileDoesNotExist };
static const QMap<short, string> ssRuntimeErrors  = {
                                                        {PtrLinkFail,                   "Проблема при передаче указателей"},
                                                        {LobbyNotFound,                 "Лобби не найдено"},
                                                        {GoogleAuthFail,                "Не удалось войти через Google аккаунт"},
                                                        {VkAuthFail,                    "Не удалось войти через VK аккаунт"},
                                                        {AlreadyHasLobby,               "Лобби уже существует"},
                                                        {AlreadyInQueue,                "Вы уже находитесь в очереди"},
                                                        {ApplySettingsFail,             "Не удалось применить настройки"},
                                                        {ToggleReadyFail,               "Не удалось переключить готовность"},
                                                        {StartGameFail,                 "Не удалось запустить матч"},
                                                        {JsonParseError,                "Не удалось обработать JSON-файл"},
                                                        {KickPlayerFail,                "Не удалось исключить игрока"},
                                                        {PromotePlayerFail,             "Не удалось сделать игрока владельцем"},
                                                        {LastSettingsFileDoesNotExist,  "Файл с последними настройками не существует"}
                                                    };

#endif // SOURCESTRINGS_H
