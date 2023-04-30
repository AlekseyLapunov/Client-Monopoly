#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <vector>
#include <stdexcept>

#include <QString>
#include <QNetworkAccessManager>
#include <QOAuthHttpServerReplyHandler>
#include <QOAuth2AuthorizationCodeFlow>
#include <QDesktopServices>
#include <QHttpPart>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>

#include "common/filemanager.h"
#include "common/sourcestructs.h"
#include "common/sourcestrings.h"

//#define AUTH_STUB
#define LOBBIES_STUB
#define LOBBIES_INSIDE_STUB

// HTTP codes
#define CODE_BAD_REQUEST             400
#define CODE_NOT_AUTHORIZED          401
#define CODE_METHOD_NOT_ALLOWED      405
#define CODE_INTERNAL_SERVER_ERROR   500
#define CODE_SUCCESS                 200

// Event timer timeout
#define MS_TIMEOUT                   2500

class ServerCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit ServerCommunicator(QObject* parent = {});
    ~ServerCommunicator();

    // Logins
    HostUserData tryVkLogin();
    HostUserData tryGoogleLogin();
    HostUserData tryIfNoNeedToAuth();

    // Host info
    HostUserData getCurrentHostInfo();

    // Lobbies
    vector<LobbyShortInfo>& getLobbiesShortInfo();
    LobbyFullInfo tryJoinById(const int lobbyUniqueId);
    LobbyFullInfo tryJoinById(const int lobbyUniqueId, const QString &enteredPassword);
    LobbyFullInfo tryCreateLobby(const int hostUserId, LobbySettings priorSettings);
    LobbyFullInfo tryRankedQueue(const int hostUserId);
    void tryToggleReady(const int lobbyUniqueId);
    void tryLobbySettingsApply(const int lobbyUniqueId, LobbySettings newSettings);
    void tryStartGame(const int lobbyUniqueId);
    void tryKickPlayer(const int lobbyUniqueId, const int playerUniqueId);
    void tryPromotePlayer(const int lobbyUniqueId, const int playerUniqueId);
    void deleteLobby(const int lobbyUniqueId);

    // Miscelanious
    void changeNickname(const QString newNickname);

signals:
    void authorizationProcessOver();
    void getInfoProcessOver();
    void refreshTokenProcessOver();
    void accessTokenRefreshed();

private slots:
    void parseAuthReply(QNetworkReply *reply);
    void parseGetInfo(QNetworkReply *reply);
    void parseRefreshAccessToken(QNetworkReply *reply);

private:
    void oauthConfigure(uint8_t authType);
    QUrl makeAddress(QString host, int port, QString additionalParameters = {});
    void doRefreshAccessToken();

private:
    QNetworkAccessManager *m_networkManager;
    QOAuth2AuthorizationCodeFlow *m_oauth;
    QOAuthHttpServerReplyHandler* m_replyHandler;
    QTimer *m_eventTimer;
    QEventLoop *m_eventLoop;

    QString m_host = "ppcd.fun";
    int m_port = 6543;

    // Auth constants
    enum AuthType {VK, Google};

    const QUrl m_authUrl[2]       = { {"https://oauth.vk.com/authorize"},
                                      {"https://accounts.google.com/o/oauth2/auth"} };
    const QUrl m_tokenUrl[2]      = { {"https://oauth.vk.com/access_token"},
                                      {"https://oauth2.googleapis.com/token"} };
    const QUrl m_redirectUri[2]   = { {"/verifyVK"},
                                      {"/verifyGoogle"} };
    const QString m_clientId[2]   = { {"51617193"},
                                      {"213758799186-64fvk6kgmol67a69du9mkddaccd71lb1.apps.googleusercontent.com"} };
    const uint32_t m_scopeMask[2] = { 0, 0 };

    const QString m_authMethod[2] = { {"auth/authFromVK"},
                                      {"auth/authFromGoogle"} };

    const QString m_getInfoMethodPrefix      = { "users/" };
    const QString m_getInfoMethodPostfix     = { "/getInfo" };
    const QString m_refreshAccessTokenMethod = { "users/refreshAccessToken" };

    const QString jsonContentType = "application/json";

    HostUserData m_temporaryHostData;
    vector<LobbyShortInfo> m_lobbiesShortInfoVec;

    bool retryIfNotNeedToAuth = false;
};

#endif // SERVERCOMMUNICATOR_H
