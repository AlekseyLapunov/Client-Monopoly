#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator(QObject* parent) : QObject(parent)
{
}

ServerCommunicator::~ServerCommunicator()
{
}

ResponseFromServerComm<HostUserData>
ServerCommunicator::doVkLogin()
{
    oauthConfigure(AuthType::VK);

    uint8_t thisSubModuleId = AuthSubModule;
    QString usingHttpMethod = "";

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::SpecAuth, "",
                                                      "", "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryHostData};
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<HostUserData>
ServerCommunicator::doGoogleLogin()
{
    oauthConfigure(AuthType::Google);

    uint8_t thisSubModuleId = AuthSubModule;
    QString usingHttpMethod = "";

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::SpecAuth,
                                                      "", "", "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryHostData};
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<HostUserData>
ServerCommunicator::doGuestLogin()
{
    uint8_t thisSubModuleId = AuthSubModule;
    QString usingHttpMethod = httpMethods[PostAuthAsGuest];

    m_oauthNetworkManager = new QNetworkAccessManager(this);

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, "",
                                                      "", "");

    m_oauthNetworkManager->deleteLater();

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryHostData};
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<HostUserData>
ServerCommunicator::checkIfNoNeedToAuth(uint8_t localCounter)
{
    ResponseFromServerComm<HostUserData> returningToClient;

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << "Check If No Need To Auth: Reached max local counter";
        returningToClient.responseFlag = UnauthorizeRf;
        returningToClient.payload = {};
        return returningToClient;
    }

    if(!doRefreshAccessToken())
    {
        returningToClient.responseFlag = UnauthorizeRf;
        returningToClient.payload = {};
        return returningToClient;
    }

    returningToClient = getCurrentHostInfo(true, localCounter);

    switch (returningToClient.responseFlag)
    {
    case AllGoodRf:
        return {AllGoodRf, m_temporaryHostData};
    case UnauthorizeRf:
        return {UnauthorizeRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<HostUserData>
ServerCommunicator::getCurrentHostInfo(bool retryCheckIfNoNeedToAuth, uint8_t localCounter)
{
    uint8_t thisSubModuleId = GetUserInfoSubModule;

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter").arg(serverCommSubModule[GetUserInfoSubModule]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);
    QString gotHostUniqueId = FileManager::getHostUniqueId();

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, {}};

    if(gotHostUniqueId.isEmpty() || !(gotHostUniqueId.toInt() >= MIN_VALID_UNIQUE_ID))
    {
        qDebug().noquote() << QString("%1: Sending not valid Host Unique ID is not allowed")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString usingHttpMethod = httpMethods[GetUsersGetInfoByUserId].arg(gotHostUniqueId);

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpGet, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken), "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
         return {AllGoodRf, m_temporaryHostData};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        if(retryCheckIfNoNeedToAuth)
            return checkIfNoNeedToAuth(localCounter);
        else
            return getCurrentHostInfo(retryCheckIfNoNeedToAuth, localCounter);
        break;
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

bool ServerCommunicator::doRefreshAccessToken()
{
    uint8_t thisSubModuleId = RefreshTokenSubModule;
    QString usingHttpMethod = httpMethods[PostAuthRefreshAccessToken];

    QString gotRefreshToken = FileManager::getToken(TokenType::Refresh);

    if(gotRefreshToken.isEmpty())
        return false;

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotRefreshToken), "") == RequestManagerAnswer::RequestAllGood)
        return true;
    else
        return false;
}

void ServerCommunicator::clearTemporaryHostData()
{
    m_temporaryHostData.uniqueId = -1;
    m_temporaryHostData.nickname = "";
    m_temporaryHostData.rpCount = 0;
    m_temporaryHostData.isGuest = false;
}

void ServerCommunicator::changeNickname(const QString newNickname, uint8_t localCounter)
{
    uint8_t thisSubModuleId = ChangeNicknameSubModule;
    QString usingHttpMethod = httpMethods[PostUsersMeChangeNickname];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return;
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return;

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      QString("{\"newNickname\": \"%1\"}")
                                                      .arg(newNickname));

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return changeNickname(newNickname, localCounter);
    default:
        return;
    }
}

ResponseFromServerComm<vector<LobbyShortInfo>&>
ServerCommunicator::getLobbiesShortInfo(uint8_t localCounter)
{
    m_lobbiesShortInfoVec.clear();
    m_lobbiesShortInfoVec.resize(0);

    uint8_t thisSubModuleId = GetLobbiesListSubModule;
    QString usingHttpMethod = httpMethods[GetLobbiesGetList];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, m_lobbiesShortInfoVec};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, m_lobbiesShortInfoVec};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpGet, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");


    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
         return {AllGoodRf, m_lobbiesShortInfoVec};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return getLobbiesShortInfo(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, m_lobbiesShortInfoVec};
    default:
        return {GeneralFailRf, m_lobbiesShortInfoVec};
    }
}

vector<LobbyShortInfo> &ServerCommunicator::getStableLobbiesList()
{
    return m_lobbiesShortInfoVec;
}

ResponseFromServerComm<LobbyFullInfo>
ServerCommunicator::createLobby(LobbySettings priorSettings,
                                uint8_t localCounter)
{
    uint8_t thisSubModuleId = CreateLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCreate];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {};

    QString requestBody = makeServerFullLobbyJson(priorSettings);

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      requestBody);

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryLobbyFullInfo};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return createLobby(priorSettings, localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<LobbyFullInfo>
ServerCommunicator::connectToLobby(const int lobbyUniqueId, uint8_t localCounter)
{
    uint8_t thisSubModuleId = ConnectLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesConnectById].arg(QString::number(lobbyUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, {}};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "{}");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryLobbyFullInfo};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return connectToLobby(lobbyUniqueId, localCounter);
    case RequestManagerAnswer::RequestLobbyIsPassworded:
        return {LobbyHasPasswordRf, {}};
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<LobbyFullInfo>
ServerCommunicator::connectToLobby(const int lobbyUniqueId, const QString &enteredPassword,
                                   uint8_t localCounter)
{
    uint8_t thisSubModuleId = ConnectLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesConnectById].arg(QString::number(lobbyUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, {}};

    QString requestBody = QString("{\"password\": \"%1\"}").arg(enteredPassword);

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      requestBody);

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryLobbyFullInfo};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return connectToLobby(lobbyUniqueId, enteredPassword, localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<LobbyFullInfo>
ServerCommunicator::connectToRankedLobby(uint8_t localCounter)
{
    uint8_t thisSubModuleId = ConnectRankedLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesRankedConnect];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, {}};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryLobbyFullInfo};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return connectToRankedLobby(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<LobbyFullInfo>
ServerCommunicator::getInfoLobby(uint8_t localCounter)
{
    uint8_t thisSubModuleId = GetInfoLobbySubModule;
    QString usingHttpMethod = httpMethods[GetLobbiesCurrentGetInfo];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, {}};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpGet, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryLobbyFullInfo};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return getInfoLobby(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::deleteLobby(uint8_t localCounter)
{
    uint8_t thisSubModuleId = DeleteLobbySubModule;
    QString usingHttpMethod = httpMethods[DeleteLobbiesCurrent];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpDelete, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return deleteLobby(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

void ServerCommunicator::clearLobbyFullInfoTemporaries()
{
    m_temporaryLobbyFullInfo.settings = {};
    m_temporaryLobbyFullInfo.usersInLobby.clear();
    m_temporaryConnections = {};
}

ResponseFromServerComm<ConnectionsFromServer>
ServerCommunicator::activeCheck(uint8_t localCounter)
{
    uint8_t thisSubModuleId = ActiveCheckSubModule;
    QString usingHttpMethod = httpMethods[GetLobbiesActiveCheck];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, {}};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, {}};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpGet, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, m_temporaryConnections};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return activeCheck(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, {}};
    default:
        return {GeneralFailRf, {}};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::switchReadiness(uint8_t localCounter)
{
    uint8_t thisSubModuleId = SwitchReadinessSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentSwitchReadiness];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return switchReadiness(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::updateLobbySettings(LobbySettings newSettings,
                                        uint8_t localCounter)
{
    uint8_t thisSubModuleId = UpdateLobbySettingsSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentUpdateSettings];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    QString requestBody = makeServerFullLobbyJson(newSettings);

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      requestBody);

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return updateLobbySettings(newSettings, localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::runGame(uint8_t localCounter)
{
    uint8_t thisSubModuleId = LobbyRunSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentRun];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return runGame(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::kickPlayer(const int playerUniqueId,
                               uint8_t localCounter)
{
    uint8_t thisSubModuleId = KickPlayerSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentPlayersKickById]
                              .arg(QString::number(playerUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return kickPlayer(playerUniqueId, localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::raisePlayer(const int playerUniqueId,
                                uint8_t localCounter)
{
    uint8_t thisSubModuleId = RaisePlayerSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentPlayersRaiseById]
                              .arg(QString::number(playerUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return raisePlayer(playerUniqueId, localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

ResponseFromServerComm<void*>
ServerCommunicator::disconnectFromLobby(uint8_t localCounter)
{
    uint8_t thisSubModuleId = DisconnectLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentDisconnect];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        return {UnauthorizeRf, nullptr};
    }

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return {UnauthorizeRf, nullptr};

    uint8_t requestManagerAnswer = basicRequestManage(thisSubModuleId, usingHttpMethod,
                                                      HttpMethodType::HttpPost, authorizationRawHeader,
                                                      authorizationHeaderContent.arg(gotAccessToken),
                                                      "");

    switch (requestManagerAnswer)
    {
    case RequestManagerAnswer::RequestAllGood:
        return {AllGoodRf, nullptr};
    case RequestManagerAnswer::RequestNeedToRepeat:
        localCounter++;
        return disconnectFromLobby(localCounter);
    case RequestManagerAnswer::RequestTimedOut:
        return {TimedOutRf, nullptr};
    default:
        return {GeneralFailRf, nullptr};
    }
}

void ServerCommunicator::catchReplyAuthFromService(const QVariantMap data)
{
    qDebug().noquote() << QString("Got auth-code from service: %1..." ).arg((data.value("code").toString()).first(6));


    if(data.value("code").toString().isEmpty())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "Got empty code";
        m_oauthCodeFlow->deleteLater();
        m_oauthReplyHandler->deleteLater();
        return;
    }

    QJsonObject codeObj;
    codeObj.insert("code", data.value("code").toString());
    QJsonDocument codeDoc(codeObj);
    codeDoc.toJson(QJsonDocument::Compact);

    QNetworkRequest request(makeAddress(host, port,
                                        httpMethods[globalAuthType]));
    request.setHeader(QNetworkRequest::ContentTypeHeader, jsonContentType.toUtf8());

    m_oauthNetworkManager->post(request, codeDoc.toJson());
}

void ServerCommunicator::catchReplyAuth(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    m_oauthNetworkManager->deleteLater();
    m_oauthCodeFlow->deleteLater();
    m_oauthReplyHandler->deleteLater();

    if(basicReplyManage(reply, AuthSubModule, replyBodyString, false) != ReplyManagerAnswer::ReplyAllGood)
        return;

    FileManager::commitTokens(bytes);

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    QJsonValue jsonValue = jsonObj.value(ssJsonUserMeta[UserInfoObj]);

    if(!jsonValue.isObject())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << QString("%1 object is fractured").arg(ssJsonUserMeta[UserInfoObj]);
        return;
    }

    QJsonObject jsonUserInfoObj = jsonValue.toObject();

    m_temporaryHostData.uniqueId = jsonUserInfoObj[ssJsonUserMeta[HostId]].toInt();
    m_temporaryHostData.nickname = jsonUserInfoObj[ssJsonUserMeta[HostNickname]].toString();
    m_temporaryHostData.rpCount  = jsonUserInfoObj[ssJsonUserMeta[HostRpCount]].toInt();
    m_temporaryHostData.isGuest  = jsonUserInfoObj[ssJsonUserMeta[HostIsGuest]].toBool();

    FileManager::commitHostData(m_temporaryHostData.uniqueId,
                                m_temporaryHostData.nickname,
                                m_temporaryHostData.rpCount,
                                m_temporaryHostData.isGuest);

    emit authorizationProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyGetUserInfo(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, GetUserInfoSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    QJsonValue jsonValue = jsonObj.value(ssJsonUserMeta[UserInfoObj]);

    if(!jsonValue.isObject())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << QString("%1 object is fractured").arg(ssJsonUserMeta[UserInfoObj]);
        return;
    }

    QJsonObject jsonUserInfoObj = jsonValue.toObject();

    m_temporaryHostData.uniqueId = jsonUserInfoObj[ssJsonUserMeta[HostId]].toInt();
    m_temporaryHostData.nickname = jsonUserInfoObj[ssJsonUserMeta[HostNickname]].toString();
    m_temporaryHostData.rpCount  = jsonUserInfoObj[ssJsonUserMeta[HostRpCount]].toInt();
    m_temporaryHostData.isGuest  = jsonUserInfoObj[ssJsonUserMeta[HostIsGuest]].toBool();

    FileManager::commitHostData(m_temporaryHostData.uniqueId,
                                m_temporaryHostData.nickname,
                                m_temporaryHostData.rpCount,
                                m_temporaryHostData.isGuest);

    emit getUserInfoProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyRefreshAccessToken(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, RefreshTokenSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObj = jsonDoc.object();

    QString accessToken = jsonObj[ssJsonUserMeta[AccessToken]].toString();
    FileManager::commitToken(TokenType::Access, accessToken);

    emit refreshTokenProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyChangeNickname(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, ChangeNicknameSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit getUserInfoProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyLobbiesGetList(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, GetLobbiesListSubModule, replyBodyString, true, true) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    QJsonArray jsonArray = jsonObj[ssJsonServerLobbiesKeys[ServLobbiesListObj]].toArray();

    for(const QJsonValue &jsonLobbyValue : jsonArray) //foreach (const QJsonValue &jsonLobbyValue, jsonArray)
    {
        if(!jsonLobbyValue.isObject())
        {
            qDebug().noquote() << QString("%1 is fractured").arg(ssJsonServerLobbiesKeys[ServLobbiesListObj]);
            continue;
        }

        QJsonObject obj = jsonLobbyValue.toObject();

        m_lobbiesShortInfoVec.push_back
        (
            {
                obj[ssJsonServerLobbiesKeys[ServLobbyID]].toInt(),
                obj[ssJsonServerLobbiesKeys[ServLobbyName]].toString(),
                obj[ssJsonServerLobbiesKeys[ServLobbyIsPrivate]].toBool(),
                static_cast<int8_t>(obj[ssJsonServerLobbiesKeys[ServLobbyPlayersNow]].toInt()),
                static_cast<int8_t>(obj[ssJsonServerLobbiesKeys[ServLobbyMaxPlayers]].toInt())
            }
        );
    }

    emit getLobbiesShortInfoProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplySwitchReadiness(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, SwitchReadinessSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit switchReadinessProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyDeleteLobby(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, DeleteLobbySubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit deleteLobbyProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyCreateLobby(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, CreateLobbySubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    m_temporaryLobbyFullInfo.usersInLobby.clear();
    m_temporaryLobbyFullInfo.usersInLobby.resize(0);
    m_temporaryLobbyFullInfo = parseLobbyFullInfoFromServer(jsonObj);

    emit createLobbyProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyConnectLobby(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, ConnectLobbySubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    m_temporaryLobbyFullInfo.usersInLobby.clear();
    m_temporaryLobbyFullInfo.usersInLobby.resize(0);
    m_temporaryLobbyFullInfo = parseLobbyFullInfoFromServer(jsonObj);

    emit connectLobbyProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyGetInfoLobby(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, GetInfoLobbySubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    m_temporaryLobbyFullInfo.usersInLobby.clear();
    m_temporaryLobbyFullInfo.usersInLobby.resize(0);
    m_temporaryLobbyFullInfo = parseLobbyFullInfoFromServer(jsonObj);

    emit getInfoLobbyProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyConnectRankedLobby(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, ConnectRankedLobbySubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    m_temporaryLobbyFullInfo.usersInLobby.clear();
    m_temporaryLobbyFullInfo.usersInLobby.resize(0);
    m_temporaryLobbyFullInfo = parseLobbyFullInfoFromServer(jsonObj);

    emit connectRankedLobbyProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyUpdateLobbySettings(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, UpdateLobbySettingsSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit updateLobbySettingsProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyRaisePlayer(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, RaisePlayerSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit raisePlayerProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyKickPlayer(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, KickPlayerSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit kickPlayerProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyDisconnectLobby(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, DisconnectLobbySubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit disconnectLobbyProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyActiveCheck(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, ActiveCheckSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

    m_temporaryConnections = parseConnections(jsonObj);

    emit activeCheckProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyLobbyRun(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, LobbyRunSubModule, replyBodyString) != ReplyManagerAnswer::ReplyAllGood)
        return;

    emit lobbyRunProcessOver();

    reply->deleteLater();
}

uint8_t ServerCommunicator::basicRequestManage(uint8_t subModuleId, QString method, uint8_t methodType,
                                               QString headerName, QString headerValue, QString requestBody)
{
    QNetworkAccessManager localNetworkAccessManager(this);
    serverCommSubModuleBase[subModuleId].resetBase();
    globalLobbyIsPassworded = false;

    QTimer localTimer;
    localTimer.setSingleShot(true);
    QEventLoop localEventLoop;

    makeConnectionBySubModuleId(subModuleId, &localNetworkAccessManager, &localEventLoop);
    connect(&localTimer, &QTimer::timeout, &localEventLoop, &QEventLoop::quit);

    localTimer.start(MS_TIMEOUT * ((methodType == HttpMethodType::SpecAuth) ? AUTH_ADDITIONAL_TIME_COEFF
                                                                            : 1));

    QNetworkRequest request(makeAddress(host, port, method));

    request.setRawHeader(headerName.toUtf8(), headerValue.toUtf8());

    switch (methodType)
    {
    case HttpMethodType::HttpPost:
        localNetworkAccessManager.post(request, requestBody.toUtf8());
        break;
    case HttpMethodType::HttpGet:
        localNetworkAccessManager.get(request);
        break;
    case HttpMethodType::HttpDelete:
        localNetworkAccessManager.deleteResource(request);
        break;
    case HttpMethodType::SpecAuth:
        m_oauthCodeFlow->grant();
        break;
    default:
        return RequestManagerAnswer::RequestTimedOut;
    }

    localEventLoop.exec();

    localTimer.disconnect();
    localEventLoop.disconnect();

    if(localTimer.isActive())
    {
        localNetworkAccessManager.disconnect();

        uint8_t returningAnswer = 0;

        if(serverCommSubModuleBase[subModuleId].needToAbort)
            returningAnswer = RequestManagerAnswer::RequestNeedToAbort;
        else if(serverCommSubModuleBase[subModuleId].repeatRequest)
            returningAnswer = RequestManagerAnswer::RequestNeedToRepeat;
        else if(globalLobbyIsPassworded)
            returningAnswer = RequestManagerAnswer::RequestLobbyIsPassworded;
        else
            returningAnswer = RequestManagerAnswer::RequestAllGood;

        serverCommSubModuleBase[subModuleId].resetBase();
        globalLobbyIsPassworded = false;
        return returningAnswer;
    }
    else
    {
        localNetworkAccessManager.disconnect();
        qDebug().noquote() << QString("%1%2 request timed out")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModule[subModuleId]);
        serverCommSubModuleBase[subModuleId].resetBase();
        return RequestManagerAnswer::RequestTimedOut;
    }
}

uint8_t ServerCommunicator::basicReplyManage(QNetworkReply *pReply, uint8_t serverCommSubModuleId, QString replyBody,
                                             bool canCallRefreshToken, bool showReplyBody)
{
    QVariant httpReplyCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    uint8_t returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;

    if(!httpReplyCode.isValid())
    {
        serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
        serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
        qDebug().noquote() << QString("%1%2 HTTP code is not valid")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModule[serverCommSubModuleId]);
        emitSignalBySubModuleId(serverCommSubModuleId);
        return returningAnswer;
    }

    short code = httpReplyCode.toInt();

    qDebug().noquote() << QString("%1%2 HTTP code %3 (%4)")
                          .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                               serverCommSubModule[serverCommSubModuleId],
                               QString::number(code),
                               (codeShortDescription[code].isEmpty() ? "not specified"
                                                                       : codeShortDescription[code]));

    if(showReplyBody)
    {
        qDebug().noquote() << QString("%1%2 reply body: %3")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModule[serverCommSubModuleId],
                                   (replyBody.length() > SHOW_FIRST_N_OF_REPLY)
                                   ? (replyBody.first(SHOW_FIRST_N_OF_REPLY) + "...")
                                   : (replyBody));
    }

    switch (code)
    {
        case CODE_BAD_REQUEST:
        {
            serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
            serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
            returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;
            break;
        }
        case CODE_NOT_AUTHORIZED:
        {
            if(canCallRefreshToken)
            {
                if(doRefreshAccessToken())
                {
                    serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = true;
                    serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = false;
                    returningAnswer = ReplyManagerAnswer::ReplyNeedToRepeatMethod;
                    break;
                }
                else
                {
                    serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
                    serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
                    returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;
                    break;
                }
            }
            else
            {
                serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = true;
                serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = false;
                returningAnswer = ReplyManagerAnswer::ReplyNeedToRepeatMethod;
                break;
            }
        }
        case CODE_METHOD_NOT_ALLOWED:
        {
            serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
            serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
            returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;
            break;
        }
        case CODE_INTERNAL_SERVER_ERROR:
        {
            serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
            serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
            returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;
            break;
        }
        case CODE_SUCCESS:
        {
            serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
            serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = false;
            returningAnswer = ReplyManagerAnswer::ReplyAllGood;
            break;
        }
        case CODE_FORBIDDEN:
        {
            serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
            serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
            if(serverCommSubModuleId == ConnectLobbySubModule)
            {
                serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
                serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = false;
                globalLobbyIsPassworded = true;
                returningAnswer = ReplyManagerAnswer::ReplyLobbyIsPassworded;
                break;
            }
            returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;
            break;
        }
        default:
        {
            serverCommSubModuleBase[serverCommSubModuleId].repeatRequest = false;
            serverCommSubModuleBase[serverCommSubModuleId].needToAbort   = true;
            returningAnswer = ReplyManagerAnswer::ReplyNeedToAbort;
            break;
        }
    }

    emitSignalBySubModuleId(serverCommSubModuleId);
    return returningAnswer;
}

void ServerCommunicator::makeConnectionBySubModuleId(uint8_t subModuleId, QNetworkAccessManager *localManager,
                                                     QEventLoop *localEventLoop)
{
    switch (subModuleId)
    {
    case AuthSubModule:
        connect(this, &ServerCommunicator::authorizationProcessOver, localEventLoop, &QEventLoop::quit);
        connect(m_oauthNetworkManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyAuth);
        return;
    case GetUserInfoSubModule:
        connect(this, &ServerCommunicator::getUserInfoProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyGetUserInfo);
        return;
    case GetLobbiesListSubModule:
        connect(this, &ServerCommunicator::getLobbiesShortInfoProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyLobbiesGetList);
        return;
    case ChangeNicknameSubModule:
        connect(this, &ServerCommunicator::changeNicknameProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyChangeNickname);
        return;
    case RefreshTokenSubModule:
        connect(this, &ServerCommunicator::refreshTokenProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyRefreshAccessToken);
        return;
    case SwitchReadinessSubModule:
        connect(this, &ServerCommunicator::switchReadinessProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplySwitchReadiness);
        return;
    case DeleteLobbySubModule:
        connect(this, &ServerCommunicator::deleteLobbyProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyDeleteLobby);
        return;
    case CreateLobbySubModule:
        connect(this, &ServerCommunicator::createLobbyProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyCreateLobby);
        return;
    case ConnectLobbySubModule:
        connect(this, &ServerCommunicator::connectLobbyProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyConnectLobby);
        return;
    case GetInfoLobbySubModule:
        connect(this, &ServerCommunicator::getInfoLobbyProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyGetInfoLobby);
        return;
    case ConnectRankedLobbySubModule:
        connect(this, &ServerCommunicator::connectRankedLobbyProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyConnectRankedLobby);
        return;
    case UpdateLobbySettingsSubModule:
        connect(this, &ServerCommunicator::updateLobbySettingsProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyUpdateLobbySettings);
        return;
    case RaisePlayerSubModule:
        connect(this, &ServerCommunicator::raisePlayerProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyRaisePlayer);
        return;
    case KickPlayerSubModule:
        connect(this, &ServerCommunicator::kickPlayerProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyKickPlayer);
        return;
    case DisconnectLobbySubModule:
        connect(this, &ServerCommunicator::disconnectLobbyProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyDisconnectLobby);
        return;
    case ActiveCheckSubModule:
        connect(this, &ServerCommunicator::activeCheckProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyActiveCheck);
        return;
    case LobbyRunSubModule:
        connect(this, &ServerCommunicator::lobbyRunProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyLobbyRun);
        return;
    default:
        return;
    }
}

void ServerCommunicator::emitSignalBySubModuleId(uint8_t subModuleId)
{
    switch (subModuleId)
    {
    case AuthSubModule:
        emit authorizationProcessOver();
        return;
    case GetUserInfoSubModule:
        emit getUserInfoProcessOver();
        return;
    case GetLobbiesListSubModule:
        emit getLobbiesShortInfoProcessOver();
        return;
    case ChangeNicknameSubModule:
        emit changeNicknameProcessOver();
        return;
    case RefreshTokenSubModule:
        emit refreshTokenProcessOver();
        return;
    case SwitchReadinessSubModule:
        emit switchReadinessProcessOver();
        return;
    case DeleteLobbySubModule:
        emit deleteLobbyProcessOver();
        return;
    case CreateLobbySubModule:
        emit createLobbyProcessOver();
        return;
    case ConnectLobbySubModule:
        emit connectLobbyProcessOver();
        return;
    case GetInfoLobbySubModule:
        emit getInfoLobbyProcessOver();
        return;
    case ConnectRankedLobbySubModule:
        emit connectRankedLobbyProcessOver();
        return;
    case UpdateLobbySettingsSubModule:
        emit updateLobbySettingsProcessOver();
        return;
    case RaisePlayerSubModule:
        emit raisePlayerProcessOver();
        return;
    case KickPlayerSubModule:
        emit kickPlayerProcessOver();
        return;
    case DisconnectLobbySubModule:
        emit disconnectLobbyProcessOver();
        return;
    case ActiveCheckSubModule:
        emit activeCheckProcessOver();
        return;
    case LobbyRunSubModule:
        emit lobbyRunProcessOver();
        return;
    default:
        return;
    }
}

void ServerCommunicator::oauthConfigure(uint8_t authType)
{
    m_oauthCodeFlow = new QOAuth2AuthorizationCodeFlow(this);
    m_oauthNetworkManager = new QNetworkAccessManager(this);
    m_oauthReplyHandler = new QOAuthHttpServerReplyHandler(port, this);

    globalAuthType = authType;

    m_oauthReplyHandler->setCallbackPath(redirectUri[authType].toString());
    QString callbackText = FileManager::getPageAsCallbackText();
    m_oauthReplyHandler->setCallbackText(callbackText);
    m_oauthCodeFlow->setReplyHandler(m_oauthReplyHandler);
    m_oauthCodeFlow->setAccessTokenUrl(tokenUrl[authType]);
    m_oauthCodeFlow->setAuthorizationUrl(authUrl[authType]);
    m_oauthCodeFlow->setClientIdentifier(clientId[authType]);
    m_oauthCodeFlow->setScope(QString::number(scopeMask[authType]));
    QObject::connect(m_oauthCodeFlow, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(m_oauthCodeFlow, &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived,
            this, &ServerCommunicator::catchReplyAuthFromService);
}

QUrl ServerCommunicator::makeAddress(QString host, int port, QString additionalParameters)
{
    return QUrl(QString("https://%1:%2").arg(host, QString::number(port))
                + QString(!additionalParameters.isEmpty() ? QString("/%1").arg(additionalParameters)
                                                          : ""));
}

QString ServerCommunicator::makeServerFullLobbyJson(LobbySettings &lobbySettingsBase)
{
    return QString("{"
                   "\"settings\":{"
                     "\"name\": \"%1\","
                     "\"type\": %2,"
                     "\"password\": \"%3\","
                     "\"maxPlayers\": %4,"
                     "\"timeForTurn\": %5,"
                     "\"victoryType\": %6,"
                     "\"scoreVictoryValue\": %7,"
                     "\"turnVictoryValue\": %8"
                   "}"
                  "}").arg(lobbySettingsBase.lobbyName,
                           QString::number(lobbySettingsBase.defineLobbyTypeForServer()),
                           lobbySettingsBase.lobbyPassword,
                           QString::number(lobbySettingsBase.maxPlayersCount),
                           QString::number(lobbySettingsBase.turnTime),
                           QString::number(lobbySettingsBase.defineVictoryTypeForServer()),
                           QString::number(lobbySettingsBase.maxMoney),
                           QString::number(lobbySettingsBase.maxTurns));
}

LobbyFullInfo ServerCommunicator::parseLobbyFullInfoFromServer(QJsonObject &jsonMainObject)
{
    LobbyFullInfo returningLFI;
    returningLFI.usersInLobby.clear();
    returningLFI.usersInLobby.resize(0);

    QJsonValue jsonLobbyInfoValue = jsonMainObject.value(ssJsonServerLobbiesKeys[ServLobbyInfoObj]);

    if(!jsonLobbyInfoValue.isObject())
    {
        qDebug().noquote() << QString("%1 is fractured").arg(ssJsonServerLobbiesKeys[ServLobbyInfoObj]);
        return {};
    }

    QJsonObject jsonLobbyInfoObject = jsonLobbyInfoValue.toObject();

    returningLFI.settings.uniqueId      = jsonLobbyInfoObject[ssJsonServerLobbiesKeys[ServLobbyID]].toInt();
    returningLFI.settings.ownerUniqueId = jsonLobbyInfoObject[ssJsonServerLobbiesKeys[ServOwnerID]].toInt();

    QJsonValue jsonLobbySettingsValue = jsonLobbyInfoObject.value(ssJsonServerLobbiesKeys[ServSettingsObj]);

    if(!jsonLobbySettingsValue.isObject())
    {
        qDebug().noquote() << QString("%1 is fractured").arg(ssJsonServerLobbiesKeys[ServSettingsObj]);
        return {};
    }

    QJsonObject jsonLobbySettingsObject = jsonLobbySettingsValue.toObject();

    returningLFI.settings.lobbyName         = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServLobbyName]].toString();
    returningLFI.settings.type              = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServLobbyType]].toInt();
    returningLFI.settings.lobbyPassword     = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServLobbyPassword]].toString();
    returningLFI.settings.maxPlayersCount   = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServLobbyMaxPlayers]].toInt();
    returningLFI.settings.turnTime          = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServTimeForTurn]].toInt();
    returningLFI.settings.setupByVictoryType(jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServVictoryType]].toInt());
    returningLFI.settings.maxMoney          = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServScoreVictoryValue]].toInt();
    returningLFI.settings.maxTurns          = jsonLobbySettingsObject[ssJsonServerLobbiesKeys[ServTurnVictoryValue]].toInt();

    QJsonArray jsonPlayersArray = jsonLobbyInfoObject[ssJsonServerLobbiesKeys[ServPlayersObj]].toArray();

    for(const QJsonValue &jsonPlayerValue : jsonPlayersArray)  //foreach (const QJsonValue &jsonPlayerValue, jsonPlayersArray)
    {
        if(!jsonPlayerValue.isObject())
        {
            qDebug().noquote() << "jsonPlayerValue is fractured";
            continue;
        }

        QJsonObject jsonPlayerObj = jsonPlayerValue.toObject();

        QJsonValue jsonPlayerSubValue = jsonPlayerObj.value(ssJsonServerLobbiesKeys[ServUserEntityObj]);

        if(!jsonPlayerSubValue.isObject())
        {
            qDebug().noquote() << QString("%1 is fractured").arg(ssJsonServerLobbiesKeys[ServUserEntityObj]);
            continue;
        }

        QJsonObject jsonPlayerSubObj = jsonPlayerSubValue.toObject();

        returningLFI.usersInLobby.push_back
        (
            {
                jsonPlayerSubObj[ssJsonServerLobbiesKeys[ServUserNickname]].toString(),
                jsonPlayerSubObj[ssJsonServerLobbiesKeys[ServUserRpCount]].toInt(),
                jsonPlayerObj[ssJsonServerLobbiesKeys[ServUserIsReady]].toBool(),
                jsonPlayerSubObj[ssJsonServerLobbiesKeys[ServUserID]].toInt()
            }
        );
    }

    returningLFI.settings.isTimerActive     = jsonLobbyInfoObject[ssJsonServerLobbiesKeys[ServLobbyTimerIsActivate]].toBool();

    QJsonValue jsonConnectionValue = jsonLobbyInfoObject.value(ssJsonServerLobbiesKeys[ServConnectionObj]);

    if(!jsonConnectionValue.isObject() && returningLFI.settings.isTimerActive)
    {
        qDebug().noquote() << QString("%1 is fractured").arg(ssJsonServerLobbiesKeys[ServConnectionObj]);
        return {};
    }

    QJsonObject jsonConnectionObj = jsonConnectionValue.toObject();

    returningLFI.settings.sessionAddress    = jsonConnectionObj[ssJsonServerLobbiesKeys[ServSessionAddress]].toString();
    returningLFI.settings.sessionPort       = jsonConnectionObj[ssJsonServerLobbiesKeys[ServSessionPort]].toInt();

    return returningLFI;
}

ConnectionsFromServer ServerCommunicator::parseConnections(QJsonObject &jsonMainObject)
{
    ConnectionsFromServer returningConnections;

    QJsonValue jsonConnectionValue = jsonMainObject.value(ssJsonServerLobbiesKeys[ServConnectionObj]);

    if(!jsonConnectionValue.isObject())
    {
        qDebug().noquote() << QString("%1 is fractured").arg(ssJsonServerLobbiesKeys[ServConnectionObj]);
        return {};
    }

    QJsonObject jsonConnectionObj = jsonConnectionValue.toObject();

    returningConnections.sessionAddress = jsonConnectionObj[ssJsonServerLobbiesKeys[ServSessionAddress]].toString();
    returningConnections.sessionPort = jsonConnectionObj[ssJsonServerLobbiesKeys[ServSessionPort]].toInt();

    return returningConnections;
}


