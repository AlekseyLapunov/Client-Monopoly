#ifndef SERVERCOMMSTRINGS_H
#define SERVERCOMMSTRINGS_H

#include <QString>
#include <QUrl>
#include <QMap>

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
                 PostUsersChangeNickname, GetUsersGetInfoById,
                 PostLobbiesCreate, GetLobbiesGetList, GetLobbiesCheckMe,
                 PostLobbiesConnectById, PostLobbiesConnectToRanked, GetLobbiesGetInfoById,
                 PostLobbiesUpdateSettingsById, PostLobbiesDisconnectById,
                 PostLobbiesSwitchReadinessById, PostLobbiesTransferRightsById,
                 PostLobbiesKickPlayerById, PostLobbiesRunGameById,
                 DeleteLobbiesById };

static const QMap<uint8_t, QString> httpMethods = {
                                                        {PostAuthVK,                     "auth/authFromVK"},
                                                        {PostAuthGoogle,                 "auth/authFromGoogle"},
                                                        {PostAuthAsGuest,                "auth/authAsGuest"},
                                                        {PostAuthRefreshAccessToken,     "auth/refreshAccessToken"},

                                                        {PostUsersChangeNickname,        "users/changeNickname"},
                                                        {GetUsersGetInfoById,            "users/%1/getInfo"},

                                                        {PostLobbiesCreate,              "lobbies/create"},
                                                        {GetLobbiesGetList,              "lobbies/getList"},
                                                        {GetLobbiesCheckMe,              "lobbies/checkMeInActiveGame"},
                                                        {PostLobbiesConnectById,         "lobbies/%1/connect"},
                                                        {PostLobbiesConnectToRanked,     "lobbies/connectToRating"},
                                                        {GetLobbiesGetInfoById,          "lobbies/%1/getInfo"},
                                                        {PostLobbiesUpdateSettingsById,  "lobbies/%1/updateSettings"},
                                                        {PostLobbiesDisconnectById,      "lobbies/%1/disconnect"},
                                                        {PostLobbiesSwitchReadinessById, "lobbies/%1/switchReadiness"},
                                                        {PostLobbiesTransferRightsById,  "lobbies/%1/transferRights"},
                                                        {PostLobbiesKickPlayerById,      "lobbies/%1/kickPlayer"},
                                                        {PostLobbiesRunGameById,         "lobbies/%1/runGame"},
                                                        {DeleteLobbiesById,              "lobbies/%1"}
                                                    };

static const QString jsonContentType            = "application/json";
static const QString authorizationRawHeader     = "Authorization";
static const QString authorizationHeaderContent = "Bearer %1";

enum HttpCodes { BadRequest = 400, NotAuthorized = 401, MethodNotAllowed = 405,
                 InternalServerError = 500, CodeSuccess = 200 };
static const QMap<short, QString> codeShortDescription =  {
                                                                {BadRequest,            "bad request"},
                                                                {NotAuthorized,         "not authorized"},
                                                                {MethodNotAllowed ,     "method not allowed"},
                                                                {InternalServerError,   "internal server error"},
                                                                {CodeSuccess,           "success"}
                                                            };

enum ServerCommSubModule { AuthSubModule, GetInfoSubModule, GetLobbiesListSubModule, ChangeNicknameSubModule,
                           RefreshTokenSubModule };
static const QMap<uint8_t, QString> serverCommSubModule = {
                                                                {AuthSubModule,             "Auth: "},
                                                                {GetInfoSubModule,          "Get Info: "},
                                                                {GetLobbiesListSubModule,   "Get Lobbies List: "},
                                                                {ChangeNicknameSubModule,   "Change Nickname: "},
                                                                {RefreshTokenSubModule,     "Refresh Token: "}
                                                            };

static QMap<uint8_t, bool> serverCommSubModuleRepeat = {
                                                            {AuthSubModule,             false},
                                                            {GetInfoSubModule,          false},
                                                            {GetLobbiesListSubModule,   false},
                                                            {ChangeNicknameSubModule,   false},
                                                            {RefreshTokenSubModule,     false}
                                                       };

#endif // SERVERCOMMSTRINGS_H
