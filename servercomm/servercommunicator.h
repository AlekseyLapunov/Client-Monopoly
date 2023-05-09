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
#include "servercommhelper.h"

class ServerCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit ServerCommunicator(QObject* parent = {});
    ~ServerCommunicator();

    // Logins
    ResponseFromServerComm<HostUserData> doVkLogin();
    ResponseFromServerComm<HostUserData> doGoogleLogin();
    ResponseFromServerComm<HostUserData> doGuestLogin();
    ResponseFromServerComm<HostUserData> checkIfNoNeedToAuth(uint8_t localCounter = 0);

    // Host info
    ResponseFromServerComm<HostUserData> getCurrentHostInfo(bool retryCheckIfNoNeedToAuth = false,
                                                            uint8_t localCounter = 0);
    void changeNickname(const QString newNickname, uint8_t localCounter = 0);
    void clearTemporaryHostData();

    // Lobbies
    ResponseFromServerComm<vector<LobbyShortInfo>&> getLobbiesShortInfo(uint8_t localCounter = 0);
    vector<LobbyShortInfo>& getStableLobbiesList();
    ResponseFromServerComm<LobbyFullInfo> connectToLobby(const int lobbyUniqueId, uint8_t localCounter = 0);
    ResponseFromServerComm<LobbyFullInfo> connectToLobby(const int lobbyUniqueId, const QString &enteredPassword, uint8_t localCounter = 0);
    ResponseFromServerComm<LobbyFullInfo> createLobby(LobbySettings priorSettings, uint8_t localCounter = 0);
    ResponseFromServerComm<LobbyFullInfo> connectToRankedLobby(uint8_t localCounter = 0);
    ResponseFromServerComm<LobbyFullInfo> getInfoLobby(uint8_t localCounter = 0);
    ResponseFromServerComm<void*> switchReadiness(uint8_t localCounter = 0);
    ResponseFromServerComm<void*> updateLobbySettings(LobbySettings newSettings, uint8_t localCounter = 0);
    ResponseFromServerComm<void*> runGame(uint8_t localCounter = 0);
    ResponseFromServerComm<void*> kickPlayer(const int playerUniqueId, uint8_t localCounter = 0);
    ResponseFromServerComm<void*> raisePlayer(const int playerUniqueId, uint8_t localCounter = 0);
    ResponseFromServerComm<void*> disconnectFromLobby(uint8_t localCounter = 0);
    ResponseFromServerComm<void*> deleteLobby(uint8_t localCounter = 0);

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
    void catchReplyAuthFromService(const QVariantMap data);
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
    enum RequestManagerAnswer { RequestNeedToAbort, RequestTimedOut, RequestAllGood, RequestNeedToRepeat, RequestLobbyIsPassworded };
    uint8_t basicRequestManage(uint8_t subModuleId, QString method, uint8_t methodType,
                               QString headerName, QString headerValue, QString requestBody,
                               bool isWaiting = true);

    void makeConnectionBySubModuleId(uint8_t subModuleId, QNetworkAccessManager *localManager,
                                     QEventLoop *localEventLoop);

    enum ReplyManagerAnswer { ReplyNeedToAbort, ReplyNeedToRepeatMethod, ReplyAllGood, ReplyLobbyIsPassworded };
    uint8_t basicReplyManage(QNetworkReply *pReply, uint8_t serverCommSubModuleId, QString replyBody = "",
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

    bool globalLobbyIsPassworded = false;
    uint8_t globalAuthType = AuthType::VK;
};

#endif // SERVERCOMMUNICATOR_H
