#ifndef SERVERCOMMHELPER_H
#define SERVERCOMMHELPER_H

#include <QString>
#include <QUrl>
#include <QMap>

// HTTP possible codes
#define CODE_BAD_REQUEST             400
#define CODE_NOT_AUTHORIZED          401
#define CODE_FORBIDDEN               403
#define CODE_METHOD_NOT_ALLOWED      405
#define CODE_INTERNAL_SERVER_ERROR   500
#define CODE_SUCCESS                 200

#define MS_TIMEOUT                   2500
#define AUTH_ADDITIONAL_TIME_COEFF   6
#define LOCAL_COUNTER_MAX            2
#define MIN_VALID_UNIQUE_ID          1
#define SHOW_FIRST_N_OF_REPLY        1000

enum ServerCommResponseFlag
{
    UnknownRf = 0,
    GeneralFailRf,
    TimedOutRf,
    UnauthorizeRf,
    WrongLobbyPasswordRf,
    LobbyHasPasswordRf,
    AllGoodRf
};

template <typename Payload>
struct ResponseFromServerComm
{
    uint8_t responseFlag;
    Payload payload;
};

static const QString host = "ppcd.fun";
static const int port = 6543;

enum AuthType {VK, Google};

static const QUrl authUrl[2]       = { {"https://oauth.vk.com/authorize"},
                                  {"https://accounts.google.com/o/oauth2/auth"} };
static const QUrl tokenUrl[2]      = { {"https://oauth.vk.com/access_token"},
                                  {"https://oauth2.googleapis.com/token"} };
static const QUrl redirectUri[2]   = { {"/verifyVK"},
                                  {"/verifyGoogle"} };
static const QString clientId[2]   = { {"51617193"},
                                  {"213758799186-64fvk6kgmol67a69du9mkddaccd71lb1.apps.googleusercontent.com"} };
static const uint32_t scopeMask[2] = { 0, 0 };

enum MethodKey { PostAuthVK, PostAuthGoogle, PostAuthAsGuest, PostAuthRefreshAccessToken,
                 PostUsersMeChangeNickname, GetUsersGetInfoByUserId,
                 PostLobbiesCreate, GetLobbiesGetList, GetLobbiesActiveCheck,
                 PostLobbiesConnectById, PostLobbiesRankedConnect, GetLobbiesCurrentGetInfo,
                 PostLobbiesCurrentUpdateSettings, PostLobbiesCurrentDisconnect,
                 PostLobbiesCurrentSwitchReadiness, PostLobbiesCurrentPlayersRaiseById,
                 PostLobbiesCurrentPlayersKickById, PostLobbiesCurrentRun,
                 DeleteLobbiesCurrent };

static const QMap<uint8_t, QString> httpMethods = {
                                                        {PostAuthVK,                            "auth/authFromVK"},
                                                        {PostAuthGoogle,                        "auth/authFromGoogle"},
                                                        {PostAuthAsGuest,                       "auth/authAsGuest"},
                                                        {PostAuthRefreshAccessToken,            "auth/refreshAccessToken"},

                                                        {PostUsersMeChangeNickname,             "users/me/changeNickname"},
                                                        {GetUsersGetInfoByUserId,               "users/%1/getInfo"},

                                                        {PostLobbiesCreate,                     "lobbies/create"},
                                                        {GetLobbiesGetList,                     "lobbies/getList"},
                                                        {PostLobbiesConnectById,                "lobbies/%1/connect"},
                                                        {PostLobbiesRankedConnect,              "lobbies/ranked/connect"},
                                                        {GetLobbiesActiveCheck,                 "lobbies/active/check"},
                                                        {GetLobbiesCurrentGetInfo,              "lobbies/current/getInfo"},
                                                        {PostLobbiesCurrentUpdateSettings,      "lobbies/current/updateSettings"},
                                                        {PostLobbiesCurrentDisconnect,          "lobbies/current/disconnect"},
                                                        {PostLobbiesCurrentSwitchReadiness,     "lobbies/current/switchReadiness"},
                                                        {PostLobbiesCurrentPlayersRaiseById,    "lobbies/current/players/%1/raise"},
                                                        {PostLobbiesCurrentPlayersKickById,     "lobbies/current/players/%1/kick"},
                                                        {PostLobbiesCurrentRun,                 "lobbies/current/run"},
                                                        {DeleteLobbiesCurrent,                  "lobbies/current"}
                                                    };

static const QString jsonContentType            = "application/json";
static const QString authorizationRawHeader     = "Authorization";
static const QString authorizationHeaderContent = "Bearer %1";

enum HttpCodes { BadRequest = 400, NotAuthorized = 401, MethodNotAllowed = 405,
                 InternalServerError = 500, CodeSuccess = 200, Forbidden = 403 };
static const QMap<short, QString> codeShortDescription =  {
                                                                {BadRequest,            "bad request"},
                                                                {NotAuthorized,         "not authorized"},
                                                                {MethodNotAllowed ,     "method not allowed"},
                                                                {InternalServerError,   "internal server error"},
                                                                {CodeSuccess,           "success"},
                                                                {Forbidden,             "forbidden"}
                                                          };

enum ServerCommSubModule { AuthSubModule, GetUserInfoSubModule, GetLobbiesListSubModule, ChangeNicknameSubModule,
                           RefreshTokenSubModule, SwitchReadinessSubModule, DeleteLobbySubModule,
                           CreateLobbySubModule, ConnectLobbySubModule, GetInfoLobbySubModule,
                           ConnectRankedLobbySubModule, UpdateLobbySettingsSubModule,
                           KickPlayerSubModule, RaisePlayerSubModule, DisconnectLobbySubModule,
                           ActiveCheckSubModule, LobbyRunSubModule };
static const QMap<uint8_t, QString> serverCommSubModule = {
                                                                {AuthSubModule,                 "Auth: "},
                                                                {GetUserInfoSubModule,          "Get User Info: "},
                                                                {GetLobbiesListSubModule,       "Get Lobbies List: "},
                                                                {ChangeNicknameSubModule,       "Change Nickname: "},
                                                                {RefreshTokenSubModule,         "Refresh Token: "},
                                                                {SwitchReadinessSubModule,      "Switch Readiness: "},
                                                                {DeleteLobbySubModule,          "Delete Lobby: "},
                                                                {CreateLobbySubModule,          "Create Lobby: "},
                                                                {ConnectLobbySubModule,         "Connect Lobby: "},
                                                                {GetInfoLobbySubModule,         "Get Info Lobby: "},
                                                                {ConnectRankedLobbySubModule,   "Connect Ranked Lobby: "},
                                                                {UpdateLobbySettingsSubModule,  "Update Lobby Settings: "},
                                                                {KickPlayerSubModule,           "Kick Player: "},
                                                                {RaisePlayerSubModule,          "Raise Player: "},
                                                                {DisconnectLobbySubModule,      "Disconnect Player: "},
                                                                {ActiveCheckSubModule,          "Active Check: "},
                                                                {LobbyRunSubModule,             "Lobby Run: "}
                                                          };

struct ServerCommSubModuleBase
{
    bool repeatRequest;
    uint8_t currentResponceStorage;

    ServerCommSubModuleBase& resetBase()
    {
        this->repeatRequest = false;
        this->currentResponceStorage = UnknownRf;
        return *this;
    }
};

static QMap<uint8_t, ServerCommSubModuleBase>
serverCommSubModuleBase = {
                                {AuthSubModule,                 {false, UnknownRf}},
                                {GetUserInfoSubModule,          {false, UnknownRf}},
                                {GetLobbiesListSubModule,       {false, UnknownRf}},
                                {ChangeNicknameSubModule,       {false, UnknownRf}},
                                {RefreshTokenSubModule,         {false, UnknownRf}},
                                {SwitchReadinessSubModule,      {false, UnknownRf}},
                                {DeleteLobbySubModule,          {false, UnknownRf}},
                                {CreateLobbySubModule,          {false, UnknownRf}},
                                {ConnectLobbySubModule,         {false, UnknownRf}},
                                {GetInfoLobbySubModule,         {false, UnknownRf}},
                                {ConnectRankedLobbySubModule,   {false, UnknownRf}},
                                {UpdateLobbySettingsSubModule,  {false, UnknownRf}},
                                {KickPlayerSubModule,           {false, UnknownRf}},
                                {RaisePlayerSubModule,          {false, UnknownRf}},
                                {DisconnectLobbySubModule,      {false, UnknownRf}},
                                {ActiveCheckSubModule,          {false, UnknownRf}},
                                {LobbyRunSubModule,             {false, UnknownRf}}
                            };

#endif // SERVERCOMMHELPER_H
