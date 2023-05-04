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
#include <QJsonArray>

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
    LobbyFullInfo createLobby(LobbySettings priorSettings, bool &ok, uint8_t localCounter = 0);
    LobbyFullInfo connectToRankedLobby(bool &ok, uint8_t localCounter = 0);
    LobbyFullInfo getInfoLobby(bool &ok, uint8_t localCounter = 0);
    void switchReadiness(bool &ok, uint8_t localCounter = 0);
    void updateLobbySettings(LobbySettings newSettings, bool &ok, uint8_t localCounter = 0);
    void runGame(bool &ok, uint8_t localCounter = 0);
    void kickPlayer(const int playerUniqueId, bool &ok, uint8_t localCounter = 0);
    void raisePlayer(const int playerUniqueId, bool &ok, uint8_t localCounter = 0);
    void disconnectFromLobby(bool &ok, uint8_t localCounter = 0);
    void deleteLobby(const int lobbyUniqueId, bool &ok, uint8_t localCounter = 0);

signals:
    void authorizationProcessOver();
    void getUserInfoProcessOver();
    void refreshTokenProcessOver();
    void changeNicknameProcessOver();
    void getLobbiesShortInfoProcessOver();
    void switchReadinessProcessOver();
    void deleteLobbyProcessOver();
    void createLobbyProcessOver();
    void connectLobbyProcessOver();
    void getInfoLobbyProcessOver();
    void connectRankedLobbyProcessOver();
    void updateLobbySettingsProcessOver();
    void raisePlayerProcessOver();
    void kickPlayerProcessOver();
    void disconnectLobbyProcessOver();
    void activeCheckProcessOver();
    void lobbyRunProcessOver();

private slots:
    void catchReplyAuth(QNetworkReply *reply);
    void catchReplyGetUserInfo(QNetworkReply *reply);
    void catchReplyRefreshAccessToken(QNetworkReply *reply);
    void catchReplyChangeNickname(QNetworkReply *reply);
    void catchReplyLobbiesGetList(QNetworkReply *reply);
    void catchReplySwitchReadiness(QNetworkReply *reply);
    void catchReplyDeleteLobby(QNetworkReply *reply);
    void catchReplyCreateLobby(QNetworkReply *reply);
    void catchReplyConnectLobby(QNetworkReply *reply);
    void catchReplyGetInfoLobby(QNetworkReply *reply);
    void catchReplyConnectRankedLobby(QNetworkReply *reply);
    void catchReplyUpdateLobbySettings(QNetworkReply *reply);
    void catchReplyRaisePlayer(QNetworkReply *reply);
    void catchReplyKickPlayer(QNetworkReply *reply);
    void catchReplyDisconnectLobby(QNetworkReply *reply);
    void catchReplyActiveCheck(QNetworkReply *reply);
    void catchReplyLobbyRun(QNetworkReply *reply);

private:
    void oauthConfigure(uint8_t authType);
    QUrl makeAddress(QString host, int port, QString additionalParameters = {});
    bool doRefreshAccessToken();

    enum HttpMethodType { HttpGet, HttpPost, HttpDelete, SpecAuth };
    enum RequestManagerAnswer { RequestNeedToAbort, RequestTimedOut, RequestAllGood };
    uint8_t basicRequestManage(uint8_t subModuleId, QString method, uint8_t methodType,
                               QString headerName, QString headerValue, QString requestBody,
                               bool isWaiting = true);

    void makeConnectionBySubModuleId(uint8_t subModuleId, QNetworkAccessManager *localManager,
                                     QEventLoop *localEventLoop);

    enum ReplyManagerAnswer { ReplyNeedToAbort, ReplyNeedToRepeatMethod, ReplyAllGood };
    uint8_t basicReplyManage(QNetworkReply *pReply, uint8_t serverCommSubModuleId,
                             bool canCallRefreshToken = true, bool showReplyBody = true);

    void emitSignalBySubModuleId(uint8_t subModuleId);

    QString makeServerFullLobbyJson(LobbySettings &lobbySettingsBase);
    LobbyFullInfo parseLobbyFullInfoFromServer(QJsonObject &jsonMainObject);

private:
    QNetworkAccessManager        *m_oauthNetworkManager;
    QOAuth2AuthorizationCodeFlow *m_oauthCodeFlow;
    QOAuthHttpServerReplyHandler *m_oauthReplyHandler;

    HostUserData m_temporaryHostData;
    LobbyFullInfo m_temporaryLobbyFullInfo;
    vector<LobbyShortInfo> m_lobbiesShortInfoVec;

    bool globalIsPassworded = false;
};

#endif // SERVERCOMMUNICATOR_H
