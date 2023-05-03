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
#include <QMetaMethod>

#include "helpers/common/filemanager.h"
#include "helpers/common/sourcestructs.h"
#include "helpers/common/sourcestrings.h"
#include "servercommstrings.h"

//#define AUTH_STUB
//#define USERS_STUB
//#define LOBBIES_STUB
//#define LOBBIES_INSIDE_STUB

class ServerCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit ServerCommunicator(QObject* parent = {});
    ~ServerCommunicator();

    // Logins
    HostUserData doVkLogin(bool &ok);
    HostUserData doGoogleLogin(bool &ok);
    HostUserData doGuestLogin(bool &ok);
    HostUserData checkIfNoNeedToAuth(bool &ok, uint8_t localCounter = 0);

    // Host info
    HostUserData getCurrentHostInfo(bool &ok, bool retryCheckIfNoNeedToAuth, uint8_t localCounter = 0);
    void changeNickname(const QString newNickname, uint8_t localCounter = 0);
    void clearTemporaryHostData();

    // Lobbies
    vector<LobbyShortInfo>& getLobbiesShortInfo(bool &ok, uint8_t localCounter = 0);
    vector<LobbyShortInfo>& getStableLobbiesList();
    LobbyFullInfo connectToLobby(const int lobbyUniqueId, bool &ok, bool &isPassworded, uint8_t localCounter = 0);
    LobbyFullInfo connectToLobby(const int lobbyUniqueId, const QString &enteredPassword, bool &ok, uint8_t localCounter = 0);
    LobbyFullInfo createLobby(const int hostUserId, LobbySettings priorSettings, bool &ok, uint8_t localCounter = 0);
    LobbyFullInfo connectToRankedLobby(const int hostUserId, bool &ok, uint8_t localCounter = 0);
    void switchReadiness(const int lobbyUniqueId, bool &ok, uint8_t localCounter = 0);
    void updateLobbySettings(const int lobbyUniqueId, LobbySettings newSettings, bool &ok, uint8_t localCounter = 0);
    void runGame(const int lobbyUniqueId, bool &ok, uint8_t localCounter = 0);
    void kickPlayer(const int lobbyUniqueId, const int playerUniqueId, bool &ok, uint8_t localCounter = 0);
    void raisePlayer(const int lobbyUniqueId, const int playerUniqueId, bool &ok, uint8_t localCounter = 0);
    void deleteLobby(const int lobbyUniqueId, bool &ok, uint8_t localCounter = 0);

signals:
    void authorizationProcessOver();
    void getInfoProcessOver();
    void refreshTokenProcessOver();
    void changeNicknameProcessOver();
    void getLobbiesShortInfoProcessOver();

private slots:
    void catchReplyAuth(QNetworkReply *reply);
    void catchReplyGetInfo(QNetworkReply *reply);
    void catchReplyRefreshAccessToken(QNetworkReply *reply);
    void catchReplyChangeNickname(QNetworkReply *reply);
    void catchReplyLobbiesGetList(QNetworkReply *reply);

private:
    void oauthConfigure(uint8_t authType);
    QUrl makeAddress(QString host, int port, QString additionalParameters = {});
    bool doRefreshAccessToken();

    enum HttpMethodType { HttpGet, HttpPost };
    enum RequestManagerAnswer { RequestNeedToAbort, RequestTimedOut, RequestAllGood };
    uint8_t basicRequestManage(uint8_t subModuleId, QString method, uint8_t methodType,
                               QString headerName, QString headerValue, QString requestBody);

    void makeConnectionBySubModuleId(uint8_t subModuleId, QNetworkAccessManager *localManager,
                                     QEventLoop *localEventLoop);

    enum ReplyManagerAnswer { ReplyNeedToAbort, ReplyNeedToRepeatMethod, ReplyAllGood };
    uint8_t basicReplyManage(QNetworkReply *pReply, uint8_t serverCommSubModuleId,
                             bool canCallRefreshToken = true, bool showReplyBody = true);

    void emitSignalBySubModuleId(uint8_t subModuleId);

private:
    QNetworkAccessManager        *m_oauthNetworkManager;
    QOAuth2AuthorizationCodeFlow *m_oauthCodeFlow;
    QOAuthHttpServerReplyHandler *m_oauthReplyHandler;

    HostUserData m_temporaryHostData;
    vector<LobbyShortInfo> m_lobbiesShortInfoVec;
};

#endif // SERVERCOMMUNICATOR_H
