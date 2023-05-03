#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator(QObject* parent) : QObject(parent)
{
    m_oauthCodeFlow = new QOAuth2AuthorizationCodeFlow(this);
    m_oauthNetworkManager = new QNetworkAccessManager(this);
    m_oauthReplyHandler = new QOAuthHttpServerReplyHandler(port, this);
}

ServerCommunicator::~ServerCommunicator()
{
    m_lobbiesShortInfoVec.clear();
    m_lobbiesShortInfoVec.resize(0);
    delete m_oauthCodeFlow;
    m_oauthCodeFlow = nullptr;
    delete m_oauthNetworkManager;
    m_oauthNetworkManager = nullptr;
    delete m_oauthReplyHandler;
    m_oauthReplyHandler = nullptr;
}

HostUserData ServerCommunicator::doVkLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {11, "VK STUB", 1110};
    return m_temporaryHostData;
#else
    m_oauthNetworkManager->disconnect();
    oauthConfigure(AuthType::VK);

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

    eventTimer.start(MS_TIMEOUT*6);

    m_oauthCodeFlow->grant();

    eventLoop.exec();

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        qDebug().noquote() << "VK Login: timed out";
        ok = false;
        return {};
    }
#endif
}

HostUserData ServerCommunicator::doGoogleLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {13, "GOOGLE STUB", 1200};
    return m_temporaryHostData;
#else
    m_oauthNetworkManager->disconnect();
    oauthConfigure(AuthType::Google);

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

    eventTimer.start(MS_TIMEOUT*6);

    m_oauthCodeFlow->grant();

    eventLoop.exec();

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        qDebug().noquote() << "Google Login: timed out";
        ok = false;
        return {};
    }
#endif
}

HostUserData ServerCommunicator::doGuestLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {21, "GUEST STUB", 0};
#else
    QNetworkAccessManager localNetworkAccessManager(this);

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

    eventTimer.start(MS_TIMEOUT);

    connect(&localNetworkAccessManager, &QNetworkAccessManager::finished,
            this, &ServerCommunicator::catchReplyAuth);

    QNetworkRequest request(makeAddress(host, port, httpMethods[PostAuthAsGuest]));

    localNetworkAccessManager.post(request, "");

    eventLoop.exec();

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        qDebug().noquote() << "Guest Login: timed out";
        ok = false;
        return {};
    }
#endif

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::checkIfNoNeedToAuth(bool &ok, uint8_t localCounter)
{
#ifdef AUTH_STUB
    ok = true; Q_UNUSED(localCounter);
    m_temporaryHostData = {17, "NO AUTH STUB", 2200};
    return m_temporaryHostData;
#else

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << "Check If No Need To Auth: Reached max local counter";
        ok = false;
        return {};
    }

    if(!doRefreshAccessToken())
    {
        ok = false;
        return {};
    }

    m_temporaryHostData = getCurrentHostInfo(ok, true, localCounter);

    if(ok)
        return m_temporaryHostData;
    else
        return {};

#endif
}

HostUserData ServerCommunicator::getCurrentHostInfo(bool &ok, bool retryCheckIfNoNeedToAuth, uint8_t localCounter)
{
#ifdef USERS_STUB
    ok = true; Q_UNUSED(retryCheckIfNoNeedToAuth); Q_UNUSED(localCounter);
    m_temporaryHostData = {17, "NO AUTH STUB", 2200, false};
    return m_temporaryHostData;
#else
    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter").arg(serverCommSubModule[GetInfoSubModule]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[GetInfoSubModule] = false;

    QString gotAccessToken = FileManager::getToken(TokenType::Access);
    QString gotHostUniqueId = FileManager::getHostUniqueId();

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return {};
        }

    if(gotHostUniqueId.isEmpty() || !(gotHostUniqueId.toInt() >= MIN_VALID_UNIQUE_ID))
    {
        qDebug().noquote() << QString("%1: Sending not valid Host Unique ID is not allowed")
                              .arg(serverCommSubModule[GetInfoSubModule]);
        ok = false;
        return {};
    }

    if(basicRequestManage(GetInfoSubModule, httpMethods[GetUsersGetInfoById].arg(gotHostUniqueId),
                          HttpMethodType::HttpGet, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken), "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[GetInfoSubModule] && retryCheckIfNoNeedToAuth)
        {
            localCounter++;
            return m_temporaryHostData = checkIfNoNeedToAuth(ok, localCounter);
        }
        if(serverCommSubModuleRepeat[GetInfoSubModule] && !retryCheckIfNoNeedToAuth)
        {
            localCounter++;
            return getCurrentHostInfo(ok, retryCheckIfNoNeedToAuth, localCounter);
        }
        return m_temporaryHostData;
    }
    else
        return {};
#endif
}

bool ServerCommunicator::doRefreshAccessToken()
{
    QString gotRefreshToken = FileManager::getToken(TokenType::Refresh);

    if(gotRefreshToken.isEmpty())
        return false;

    if(basicRequestManage(RefreshTokenSubModule, httpMethods[PostAuthRefreshAccessToken],
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
    if(localCounter >= LOCAL_COUNTER_MAX)
        return;

    serverCommSubModuleRepeat[ChangeNicknameSubModule] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(basicRequestManage(ChangeNicknameSubModule, httpMethods[PostUsersChangeNickname],
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          QString("{\"newNickname\": \"%1\"}")
                          .arg(newNickname)) == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[ChangeNicknameSubModule])
        {
            localCounter++;
            return changeNickname(newNickname, localCounter);
        }
        return;
    }
    else
        return;
}

vector<LobbyShortInfo> &ServerCommunicator::getLobbiesShortInfo(bool &ok, uint8_t localCounter)
{
    m_lobbiesShortInfoVec.clear();
    m_lobbiesShortInfoVec.resize(0);

#ifdef LOBBIES_STUB
    LobbyShortInfo stubLobbiesShortInfo[]
            = {
                {0, "A STUB LOBBY", NO_PASSWORD, 3, 6},
                {1, "Poison", NO_PASSWORD, 1, 6},
                {2, "horseeeee", HAS_PASSWORD, 3, 5},
                {3, "Лабби1234", HAS_PASSWORD, 2, 4},
                {4, "JoinMePls", NO_PASSWORD, 1, 2},
                {5, "SartaSMV", HAS_PASSWORD, 1, 3},
                {6, "Uncut", NO_PASSWORD, 6, 6},
                {7, "Кукуруза", HAS_PASSWORD, 2, 2},
                {8, "123456789", NO_PASSWORD, 2, 3},
                {9, "professionals", HAS_PASSWORD, 3, 5},
              };

    for(int i = 0; i < 10; i++)
        m_lobbiesShortInfoVec.push_back(stubLobbiesShortInfo[i]);

    return m_lobbiesShortInfoVec;
#else
    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[GetLobbiesListSubModule]);
        ok = false;
        return m_lobbiesShortInfoVec;
    }

    serverCommSubModuleRepeat[GetLobbiesListSubModule] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return m_lobbiesShortInfoVec;
        }

    if(basicRequestManage(GetLobbiesListSubModule, httpMethods[PostUsersChangeNickname],
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[GetLobbiesListSubModule])
        {
            localCounter++;
            return m_lobbiesShortInfoVec = getLobbiesShortInfo(ok, localCounter);
        }
        return m_lobbiesShortInfoVec;
    }
    else
        return m_lobbiesShortInfoVec;
#endif
}

vector<LobbyShortInfo> &ServerCommunicator::getStableLobbiesList()
{
    return m_lobbiesShortInfoVec;
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int lobbyUniqueId)
{
#ifdef LOBBIES_STUB
    if(lobbyUniqueId != 0)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[LobbyNotFound]);

    return
    {
        {
            0, "A STUB LOBBY", "", 6, 23, false, "", 0,
            8, 1000, IS_INFINITE, 72, NOT_INFINITE, LOBBY_TYPE_PUBLIC
        },
        // vector<UserShortInfo>
        {
          {"monk", 800, IS_READY, 23},
          {"Kisl Jsji", 2400, IS_READY, 67},
          {"Роман Заглушевич", 11111, NOT_READY, 63},
        }
    };
#else
    LobbyFullInfo lfiObject;

    return lfiObject;
#endif
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int lobbyUniqueId, const QString &enteredPassword)
{
    // Make request
#ifdef LOBBIES_STUB
    throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[LobbyNotFound]);
#endif
}

LobbyFullInfo ServerCommunicator::tryCreateLobby(const int hostUserId, LobbySettings priorSettings)
{
#ifdef LOBBIES_STUB
    // Need to check if user already has created lobby. If does - throw exception
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[AlreadyHasLobby]);

    return
    {
        {
            1, "My Lobby", "", 6, hostUserId, false, "", 0,
            8, 1000, NOT_INFINITE, 72, NOT_INFINITE, LOBBY_TYPE_PUBLIC
        },
        // vector<UserShortInfo>
        {}
    };
#else
    return {};
#endif
}

LobbyFullInfo ServerCommunicator::tryRankedQueue(const int hostUserId)
{
    // Need to check if user already in the queue

#ifdef LOBBIES_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[AlreadyInQueue]);

    return
    {
        {
            -1, "RANKED STUB", "", 4, -1, false, "", 0,
            8, 2000, IS_INFINITE, 72, NOT_INFINITE, LOBBY_TYPE_RANKED
        },
        // vector<UserShortInfo>
        {
          {"Максим Баландин", 1435, IS_READY, 92},
          {"Nevada", 1400, IS_READY, 34},
          {"PPCD", 1215, IS_READY, 77}
        }
    };
#endif
}

void ServerCommunicator::deleteLobby(const int lobbyUniqueId)
{
}

void ServerCommunicator::tryToggleReady(const int lobbyUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[ToggleReadyFail]);
#endif
}

void ServerCommunicator::tryLobbySettingsApply(const int lobbyUniqueId, LobbySettings newSettings)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[ApplySettingsFail]);
#endif
}

void ServerCommunicator::tryStartGame(const int lobbyUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[StartGameFail]);
#endif
}

void ServerCommunicator::tryKickPlayer(const int lobbyUniqueId, const int playerUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[KickPlayerFail]);
#endif
}

void ServerCommunicator::tryPromotePlayer(const int lobbyUniqueId, const int playerUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssErrorsContent[PromotePlayerFail]);
#endif
}

void ServerCommunicator::catchReplyAuth(QNetworkReply *reply)
{
    if(basicReplyManage(reply, AuthSubModule) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QByteArray bytes = reply->readAll();

    FileManager::commitTokens(bytes.data());

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes.data()).object();

    QJsonValue jsonValue = jsonObj.value(ssJsonUserMeta[UserInfoObj]);

    if(!jsonValue.isObject())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "userInfo object is fractured";
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

void ServerCommunicator::catchReplyGetInfo(QNetworkReply *reply)
{
    if(basicReplyManage(reply, GetInfoSubModule) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QByteArray bytes = reply->readAll();

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes.data()).object();

    QJsonValue jsonValue = jsonObj.value(ssJsonUserMeta[UserInfoObj]);

    if(!jsonValue.isObject())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "userInfo object is fractured";
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

    emit getInfoProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyRefreshAccessToken(QNetworkReply *reply)
{
    if(basicReplyManage(reply, RefreshTokenSubModule) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QByteArray bytes = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes.data());
    QJsonObject jsonObj = jsonDoc.object();

    QString accessToken = jsonObj[ssJsonUserMeta[AccessToken]].toString();
    FileManager::commitToken(TokenType::Access, accessToken);

    emit refreshTokenProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::catchReplyChangeNickname(QNetworkReply *reply)
{
    if(basicReplyManage(reply, ChangeNicknameSubModule) != ReplyManagerAnswer::ReplyAllGood)
        return;

    reply->deleteLater();
}

void ServerCommunicator::catchReplyLobbiesGetList(QNetworkReply *reply)
{
    if(basicReplyManage(reply, GetLobbiesListSubModule) != ReplyManagerAnswer::ReplyAllGood)
        return;

    QByteArray bytes = reply->readAll();



    reply->deleteLater();
}

uint8_t ServerCommunicator::basicReplyManage(QNetworkReply *pReply, uint8_t serverCommSubModuleId,
                                             bool canCallRefreshToken, bool showReplyBody)
{
    QVariant httpReplyCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(!httpReplyCode.isValid())
    {
        serverCommSubModuleRepeat[serverCommSubModuleId] = false;
        qDebug().noquote() << QString("%1%2 HTTP code is not valid")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModule[serverCommSubModuleId]);
        return ReplyManagerAnswer::ReplyNeedToAbort;
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
        QByteArray bytes = pReply->readAll();
        QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());
        qDebug().noquote() << QString("%1%2 reply body (shorted): %3")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModule[serverCommSubModuleId],
                                   (replyBodyString.length() > SHOW_FIRST_N_OF_REPLY)
                                   ? (replyBodyString.first(SHOW_FIRST_N_OF_REPLY) + "...")
                                   : (replyBodyString));
    }

    switch (code)
    {
        case CODE_BAD_REQUEST:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            return ReplyManagerAnswer::ReplyNeedToAbort;
        }
        case CODE_NOT_AUTHORIZED:
        {
            if(canCallRefreshToken)
            {
                if(doRefreshAccessToken())
                {
                    serverCommSubModuleRepeat[serverCommSubModuleId] = true;
                    emitSignalBySubModuleId(serverCommSubModuleId);
                    return ReplyManagerAnswer::ReplyNeedToRepeatMethod;
                }
                else
                {
                    serverCommSubModuleRepeat[serverCommSubModuleId] = false;
                    return ReplyManagerAnswer::ReplyNeedToAbort;
                }
            }
            else
            {
                serverCommSubModuleRepeat[serverCommSubModuleId] = true;
                emitSignalBySubModuleId(serverCommSubModuleId);
                return ReplyManagerAnswer::ReplyNeedToRepeatMethod;
            }
        }
        case CODE_METHOD_NOT_ALLOWED:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            return ReplyManagerAnswer::ReplyNeedToAbort;
        }
        case CODE_INTERNAL_SERVER_ERROR:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            return ReplyManagerAnswer::ReplyNeedToAbort;
        }
        case CODE_SUCCESS:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            emitSignalBySubModuleId(serverCommSubModuleId);
            return ReplyManagerAnswer::ReplyAllGood;
        }
        default:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            return ReplyManagerAnswer::ReplyNeedToAbort;
        }
    }
}

uint8_t ServerCommunicator::basicRequestManage(uint8_t subModuleId, QString method, uint8_t methodType,
                                               QString headerName, QString headerValue, QString requestBody)
{
    QNetworkAccessManager localNetworkAccessManager(this);

    QTimer localTimer;
    QEventLoop localEventLoop;

    makeConnectionBySubModuleId(subModuleId, &localNetworkAccessManager, &localEventLoop);
    connect(&localTimer, &QTimer::timeout, &localEventLoop, &QEventLoop::quit);

    localTimer.start(MS_TIMEOUT);

    QNetworkRequest request(makeAddress(host, port, method));

    request.setRawHeader(headerName.toUtf8(), headerValue.toUtf8());

    if(methodType == HttpMethodType::HttpPost)
        localNetworkAccessManager.post(request, requestBody.toUtf8());
    else
        localNetworkAccessManager.get(request);

    localEventLoop.exec();

    if(localTimer.isActive())
        return RequestManagerAnswer::RequestAllGood;
    else
    {
        qDebug().noquote() << QString("%1%2 request timed out").arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                                                    serverCommSubModule[subModuleId]);
        return RequestManagerAnswer::RequestTimedOut;
    }
}

void ServerCommunicator::makeConnectionBySubModuleId(uint8_t subModuleId, QNetworkAccessManager *localManager,
                                                     QEventLoop *localEventLoop)
{
    switch (subModuleId)
    {
    case AuthSubModule:
        return;
    case GetInfoSubModule:
        connect(this, &ServerCommunicator::getInfoProcessOver, localEventLoop, &QEventLoop::quit);
        connect(localManager, &QNetworkAccessManager::finished,
                this, &ServerCommunicator::catchReplyGetInfo);
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
    default:
        return;
    }
}

void ServerCommunicator::emitSignalBySubModuleId(uint8_t subModuleId)
{
    switch (subModuleId)
    {
    case AuthSubModule:
        return;
    case GetInfoSubModule:
        emit getInfoProcessOver();
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
    default:
        return;
    }
}

void ServerCommunicator::oauthConfigure(uint8_t authType)
{
    m_oauthReplyHandler->setCallbackPath(redirectUri[authType].toString());
    QString callbackText = FileManager::getPageAsCallbackText();
    m_oauthReplyHandler->setCallbackText(callbackText);
    m_oauthCodeFlow->setReplyHandler(m_oauthReplyHandler);
    m_oauthCodeFlow->setAccessTokenUrl(tokenUrl[authType]);
    m_oauthCodeFlow->setAuthorizationUrl(authUrl[authType]);
    m_oauthCodeFlow->setClientIdentifier(clientId[authType]);
    m_oauthCodeFlow->setScope(QString::number(scopeMask[authType]));
    QObject::connect(m_oauthCodeFlow, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(m_oauthCodeFlow, &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived, [=](const QVariantMap data)
    {
        qDebug().noquote() << "Got auth-code from service: " << data.value("code");
        if(data.value("code").toString().isEmpty())
        {
            qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                               << "Got empty code";
            return;
        }

        QJsonObject codeObj;
        codeObj.insert("code", data.value("code").toString());
        QJsonDocument codeDoc(codeObj);
        codeDoc.toJson(QJsonDocument::Compact);

        connect(m_oauthNetworkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyAuth);

        QNetworkRequest request(makeAddress(host, port,
                                            httpMethods[authType]));
        request.setHeader(QNetworkRequest::ContentTypeHeader, jsonContentType.toUtf8());

        m_oauthNetworkManager->post(request, codeDoc.toJson());
    });
}

QUrl ServerCommunicator::makeAddress(QString host, int port, QString additionalParameters)
{
    return QUrl(QString("https://%1:%2").arg(host, QString::number(port))
                + QString(!additionalParameters.isEmpty() ? "/%1" : "").arg(additionalParameters));
}
