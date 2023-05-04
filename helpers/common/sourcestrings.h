#ifndef SOURCESTRINGS_H
#define SOURCESTRINGS_H

#include <string.h>

#include <QString>
#include <QStringList>
#include <QMap>

using std::string;

// Regular expressions
enum RegExpFlags {LobbyNameRegExp, NicknameRegExp, LobbyPasswordRegExp, LobbyUniqueIdRegExp};
static const QMap<uint8_t, QString> ssRegExps = {
                                                    {LobbyNameRegExp,       "^((\\S+)(\\s?(\\S+))*)$"},
                                                    {NicknameRegExp,        "^((\\S+)(\\s?(\\S+))*)$"},
                                                    {LobbyPasswordRegExp,   "^(\\S|\\s)+$"},
                                                    {LobbyUniqueIdRegExp,   "^[0-9]*$"}
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
static const QMap<uint8_t, SubDialogStrings> ssSdStrings  = {
                                                                {SdNickname, ssSubDialogChangeNickname},
                                                                {SdDirectJoin, ssSubDialogJoinById},
                                                                {SdPasswordJoin, ssSubDialogPasswordedJoin}
                                                            };

// filemanager.h
enum TokenType { Access, Refresh };

static const QString ssLocalDirPath           = "local/";
static const QString ssUserMetaFileName       = "usermeta.json";
static const QString ssRankedSettingsFileName = "default_ranked_settings.json";
static const QString ssLastSettingsFileName   = "last_settings.json";
static const QString ssCaptionImportSettings  = "Импортировать настройки лобби";
static const QString ssCaptionExportSettings  = "Экспортировать настройки лобби";
static const QString ssJsonFilter             = "JSON file (*.json)";

// JSON keys
enum JsonKeysLobbySettingsId { UniqueId, LobbyName, LobbyPassword, MaxPlayersCount, OwnerUniqueId, IsTimerActive,
                               SessionAddress, SessionPort, TurnTime, MaxMoney, IsMaxMoneyInfinite, MaxTurns, AreMaxTurnsInfinite,
                               LobbyType };
static const QMap<uint8_t, QString> ssJsonLobbySettings  = {
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
                                                                {AreMaxTurnsInfinite,   "areMaxTurnsInfinite"},
                                                                {LobbyType,             "type"}
                                                           };

enum JsonKeysUserMeta        { AccessToken, RefreshToken, HostId, HostNickname, HostRpCount, HostIsGuest, Uses3dDice,
                               TokensObj, UserInfoObj };
static const QMap<uint8_t, QString> ssJsonUserMeta       = {
                                                                {AccessToken,   "accessToken"},
                                                                {RefreshToken,  "refreshToken"},
                                                                {HostId,        "ID"},
                                                                {HostNickname,  "nickname"},
                                                                {HostRpCount,   "rating"},
                                                                {HostIsGuest,   "isGuest"},
                                                                {Uses3dDice,    "uses3dDice"},
                                                                {TokensObj,     "tokens"},
                                                                {UserInfoObj,   "userInfo"}
                                                           };

enum JsonKeysServerLobbies         { ServLobbiesListObj, ServLobbyID, ServLobbyName, ServLobbyIsPrivate,
                                     ServLobbyMaxPlayers, ServLobbyPlayersNow, ServLobbyInfoObj,
                                     ServOwnerID, ServSettingsObj, ServLobbyType,
                                     ServLobbyPassword, ServTimeForTurn,
                                     ServVictoryType, ServScoreVictoryValue,
                                     ServTurnVictoryValue, ServPlayersObj,
                                     ServUserEntityObj, ServUserID, ServUserNickname,
                                     ServUserIsGuest, ServUserRpCount,
                                     ServUserIsReady, ServLobbyTimerIsActivate,
                                     ServConnectionObj, ServSessionAddress, ServSessionPort};
static const QMap<uint8_t, QString> ssJsonServerLobbiesKeys    = {
                                                                    {ServLobbiesListObj,        "lobbies"},
                                                                    {ServLobbyID,               "lobbyID"},
                                                                    {ServLobbyName,             "name"},
                                                                    {ServLobbyIsPrivate,        "isPrivate"},
                                                                    {ServLobbyMaxPlayers,       "maxPlayers"},
                                                                    {ServLobbyPlayersNow,       "playersNow"},
                                                                    {ServLobbyInfoObj,          "lobbyInfo"},
                                                                    {ServOwnerID,               "ownerID"},
                                                                    {ServSettingsObj,           "settings"},
                                                                    {ServLobbyType,             "type"},
                                                                    {ServLobbyPassword,         "password"},
                                                                    {ServTimeForTurn,           "timeForTurn"},
                                                                    {ServVictoryType,           "victoryType"},
                                                                    {ServScoreVictoryValue,     "scoreVictoryValue"},
                                                                    {ServTurnVictoryValue,      "turnVictoryValue"},
                                                                    {ServPlayersObj,            "players"},
                                                                    {ServUserEntityObj,         "userEntity"},
                                                                    {ServUserID,                "ID"},
                                                                    {ServUserNickname,          "nickname"},
                                                                    {ServUserIsGuest,           "isGuest"},
                                                                    {ServUserRpCount,           "rating"},
                                                                    {ServUserIsReady,           "isReady"},
                                                                    {ServLobbyTimerIsActivate,  "timerIsActivate"},
                                                                    {ServConnectionObj,         "connection"},
                                                                    {ServSessionAddress,        "address"},
                                                                    {ServSessionPort,           "port"}
                                                                 };


// Classes Names for the exception thrower specifying
enum ClassesNames { LoginWindowCN, MenuWindowCN, LobbiesSubDialogCN, LobbyWindowCN,
                    ServerCommCN, GameManagerCN, FileManagerCN };
static const QMap<uint8_t, string> ssClassNames = {
                                                        {LoginWindowCN,      "LoginWindow: "},
                                                        {MenuWindowCN,       "MenuWindow: "},
                                                        {LobbiesSubDialogCN, "LobbiesSubDialog: "},
                                                        {LobbyWindowCN,      "LobbyWindow: "},
                                                        {ServerCommCN,       "ServerCommunicator: "},
                                                        {GameManagerCN,      "GameManagerWindow: "},
                                                        {FileManagerCN,      "FileManager: "}
                                                  };

// runtime_error
enum RunTimeErrorId { PtrLinkFail, LobbyNotFound, GoogleAuthFail, VkAuthFail, GuestAuthFail,
                      LobbyCreateFail, RankedSearchFail, ApplySettingsFail,
                      ToggleReadyFail, StartGameFail, JsonParseError, KickPlayerFail,
                      PromotePlayerFail, LastSettingsFileDoesNotExist, GetHostInfoFail };
static const QMap<uint8_t, string> ssErrorsContent  = {
                                                          {PtrLinkFail,                   "Проблема при передаче указателей"},
                                                          {LobbyNotFound,                 "Лобби не найдено"},
                                                          {GoogleAuthFail,                "Не удалось войти через Google аккаунт"},
                                                          {VkAuthFail,                    "Не удалось войти через VK аккаунт"},
                                                          {GuestAuthFail,                 "Не удалось войти как гость"},
                                                          {LobbyCreateFail,               "Не удалось создать лобби"},
                                                          {RankedSearchFail,              "Не удалось выполнить поиск рейтинговой игры"},
                                                          {ApplySettingsFail,             "Не удалось применить настройки"},
                                                          {ToggleReadyFail,               "Не удалось переключить готовность"},
                                                          {StartGameFail,                 "Не удалось запустить матч"},
                                                          {JsonParseError,                "Не удалось обработать JSON-файл"},
                                                          {KickPlayerFail,                "Не удалось исключить игрока"},
                                                          {PromotePlayerFail,             "Не удалось сделать игрока владельцем"},
                                                          {LastSettingsFileDoesNotExist,  "Файл с последними настройками не существует"},
                                                          {GetHostInfoFail,               "Не удалось обновить данные пользователя"}
                                                      };

#endif // SOURCESTRINGS_H





