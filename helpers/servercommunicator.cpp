#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator(QObject* parent) : QObject(parent)
{
    m_oauthCodeFlow = new QOAuth2AuthorizationCodeFlow(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_replyHandler = new QOAuthHttpServerReplyHandler(m_port, this);
}

ServerCommunicator::~ServerCommunicator()
{
    m_lobbiesShortInfoVec.clear();
    m_lobbiesShortInfoVec.resize(0);
    delete m_oauthCodeFlow;
    m_oauthCodeFlow = nullptr;
    delete m_networkManager;
    m_networkManager = nullptr;
    delete m_replyHandler;
    m_replyHandler = nullptr;
}

HostUserData ServerCommunicator::doVkLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {11, "VK STUB", 1110};
#else
    m_networkManager->disconnect();

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

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::doGoogleLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {13, "GOOGLE STUB", 1200};
#else
    m_networkManager->disconnect();

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

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::doGuestLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {21, "GUEST STUB", 0};
#else
    m_networkManager->disconnect();

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

    eventTimer.start(MS_TIMEOUT);

    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ServerCommunicator::catchReplyAuth);

    QNetworkRequest request(makeAddress(m_host, m_port, m_httpMethods[PostAuthAsGuest]));

    m_networkManager->post(request, "");

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
        qDebug().noquote() << "Get Host Info: Reached max local counter";
        ok = false;
        return {};
    }

    m_networkManager->disconnect();
    retryMethod = false;

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
        qDebug().noquote() << "Get Host Info: Sending not valid Host Unique ID is not allowed";
        ok = false;
        return {};
    }

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::getInfoProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    eventTimer.start(MS_TIMEOUT);

    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ServerCommunicator::catchReplyGetInfo);

    QNetworkRequest request(makeAddress(m_host, m_port,
                                        m_httpMethods[GetUsersGetInfoById].arg(gotHostUniqueId)));
    request.setRawHeader(m_authorizationRawHeader.toUtf8(), m_authorizationHeaderContent.arg(gotAccessToken).toUtf8());

    m_networkManager->get(request);

    eventLoop.exec();

    if(retryMethod && retryCheckIfNoNeedToAuth)
    {
        localCounter++;
        return m_temporaryHostData = checkIfNoNeedToAuth(ok, localCounter);
    }

    if(retryMethod && !retryCheckIfNoNeedToAuth)
    {
        localCounter++;
        return getCurrentHostInfo(ok, retryCheckIfNoNeedToAuth, localCounter);
    }

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        qDebug().noquote() << "Get Current Host Info: timed out";
        ok = false;
        return {};
    }
#endif
}

void ServerCommunicator::clearTemporaryHostData()
{
    m_temporaryHostData.uniqueId = -1;
    m_temporaryHostData.nickname = "";
    m_temporaryHostData.rpCount = 0;
    m_temporaryHostData.isGuest = false;
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
        qDebug().noquote() << "Get Lobbies Short Info: Reached max local counter";
        ok = false;
        return m_lobbiesShortInfoVec;
    }

    m_networkManager->disconnect();
    retryMethod = false;

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return m_lobbiesShortInfoVec;
        }

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::getLobbiesShortInfoProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    eventTimer.start(MS_TIMEOUT);

    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ServerCommunicator::catchReplyLobbiesGetList);

    QNetworkRequest request(makeAddress(m_host, m_port, m_httpMethods[GetLobbiesGetList]));
    request.setRawHeader(m_authorizationRawHeader.toUtf8(), m_authorizationHeaderContent.arg(gotAccessToken).toUtf8());

    m_networkManager->get(request);

    eventLoop.exec();

    if(retryMethod)
    {
        localCounter++;
        return m_lobbiesShortInfoVec = getLobbiesShortInfo(ok, localCounter);
    }

    ok = eventTimer.isActive();
    if(!ok)
        qDebug().noquote() << "Get Lobbies Short Info: timed out";
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

void ServerCommunicator::changeNickname(const QString newNickname, uint8_t localCounter)
{
    if(localCounter >= LOCAL_COUNTER_MAX)
        return;

    m_networkManager->disconnect();
    retryMethod = false;

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::changeNicknameProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    eventTimer.start(MS_TIMEOUT);

    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ServerCommunicator::catchReplyChangeNickname);

    QNetworkRequest request(makeAddress(m_host, m_port,
                                        m_httpMethods[PostUsersChangeNickname]));
    request.setRawHeader(m_authorizationRawHeader.toUtf8(), m_authorizationHeaderContent.arg(gotAccessToken).toUtf8());

    m_networkManager->post(request, QString("{\"newNickname\": \"%1\"}").arg(newNickname).toUtf8());

    eventLoop.exec();

    if(retryMethod)
    {
        localCounter++;
        return changeNickname(newNickname, localCounter);
    }
}

void ServerCommunicator::oauthConfigure(uint8_t authType)
{
    m_replyHandler->setCallbackPath(m_redirectUri[authType].toString());
    QString callbackText = FileManager::getPageAsCallbackText();
    m_replyHandler->setCallbackText(callbackText);
    m_oauthCodeFlow->setReplyHandler(m_replyHandler);
    m_oauthCodeFlow->setAccessTokenUrl(m_tokenUrl[authType]);
    m_oauthCodeFlow->setAuthorizationUrl(m_authUrl[authType]);
    m_oauthCodeFlow->setClientIdentifier(m_clientId[authType]);
    m_oauthCodeFlow->setScope(QString::number(m_scopeMask[authType]));
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

        connect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyAuth);

        QNetworkRequest request(makeAddress(m_host, m_port,
                                            m_httpMethods[authType]));
        request.setHeader(QNetworkRequest::ContentTypeHeader, m_jsonContentType.toUtf8());

        m_networkManager->post(request, codeDoc.toJson());
    });
}

void ServerCommunicator::catchReplyAuth(QNetworkReply *reply)
{
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyAuth);

    switch (basicReplyManage(reply, ssServerCommSubModule[AuthSubModule], false))
    {
    case ReplyParserAnswer::NeedToAbort:
        retryMethod = false;
        return;
    case ReplyParserAnswer::NeedToRepeatMethod:
        retryMethod = true;
        emit authorizationProcessOver();
        return;
    case ReplyParserAnswer::AllGood:
        retryMethod = false;
        break;
    default:
        retryMethod = false;
        return;
    }

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
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyGetInfo);

    switch (basicReplyManage(reply, ssServerCommSubModule[GetInfoSubModule]))
    {
    case ReplyParserAnswer::NeedToAbort:
        retryMethod = false;
        return;
    case ReplyParserAnswer::NeedToRepeatMethod:
        retryMethod = true;
        emit getInfoProcessOver();
        return;
    case ReplyParserAnswer::AllGood:
        retryMethod = false;
        break;
    default:
        retryMethod = false;
        return;
    }

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
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyRefreshAccessToken);

    switch (basicReplyManage(reply, ssServerCommSubModule[RefreshTokenSubModule], false))
    {
    case ReplyParserAnswer::NeedToAbort:
        retryMethod = false;
        return;
    case ReplyParserAnswer::NeedToRepeatMethod:
        retryMethod = true;
        emit refreshTokenProcessOver();
        return;
    case ReplyParserAnswer::AllGood:
        retryMethod = false;
        break;
    default:
        retryMethod = false;
        return;
    }

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
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyChangeNickname);

    switch (basicReplyManage(reply, ssServerCommSubModule[ChangeNicknameSubModule]))
    {
    case ReplyParserAnswer::NeedToAbort:
        retryMethod = false;
        return;
    case ReplyParserAnswer::NeedToRepeatMethod:
        retryMethod = true;
        emit changeNicknameProcessOver();
        return;
    case ReplyParserAnswer::AllGood:
        retryMethod = false;
        break;
    default:
        retryMethod = false;
        return;
    }

    reply->deleteLater();
}

void ServerCommunicator::catchReplyLobbiesGetList(QNetworkReply *reply)
{
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ServerCommunicator::catchReplyLobbiesGetList);

    switch (basicReplyManage(reply, ssServerCommSubModule[GetLobbiesListSubModule]))
    {
    case ReplyParserAnswer::NeedToAbort:
        retryMethod = false;
        return;
    case ReplyParserAnswer::NeedToRepeatMethod:
        retryMethod = true;
        emit getLobbiesShortInfoProcessOver();
        return;
    case ReplyParserAnswer::AllGood:
        retryMethod = false;
        break;
    default:
        retryMethod = false;
        return;
    }

    QByteArray bytes = reply->readAll();



    reply->deleteLater();
}

QUrl ServerCommunicator::makeAddress(QString host, int port, QString additionalParameters)
{
    return QUrl(QString("https://%1:%2").arg(host, QString::number(port))
                + QString(!additionalParameters.isEmpty() ? "/%1" : "").arg(additionalParameters));
}

bool ServerCommunicator::doRefreshAccessToken()
{
    m_networkManager->disconnect();

    QTimer localTimer;
    QEventLoop localEventLoop;

    connect(this, &ServerCommunicator::refreshTokenProcessOver, &localEventLoop, &QEventLoop::quit);
    connect(&localTimer, &QTimer::timeout, &localEventLoop, &QEventLoop::quit);

    localTimer.start(MS_TIMEOUT);
    connect(m_networkManager, &QNetworkAccessManager::finished,
           this, &ServerCommunicator::catchReplyRefreshAccessToken);

    QString gotRefreshToken = FileManager::getToken(TokenType::Refresh);

    if(gotRefreshToken.isEmpty())
        return false;

    QNetworkRequest request(makeAddress(m_host, m_port, m_httpMethods[PostAuthRefreshAccessToken]));
    request.setRawHeader(m_authorizationRawHeader.toUtf8(),
                         m_authorizationHeaderContent.arg(gotRefreshToken).toUtf8());
    m_networkManager->post(request, "");

    localEventLoop.exec();

    if(localTimer.isActive())
        return true;
    else
        return false;
}

uint8_t ServerCommunicator::basicReplyManage(QNetworkReply *pReply, QString serverCommSubModuleName,
                                             bool canCallRefreshToken, bool showReplyBody)
{
    QVariant httpReplyCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(!httpReplyCode.isValid())
    {
        qDebug().noquote() << QString("%1%2 HTTP code is not valid")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModuleName);
        return ReplyParserAnswer::NeedToAbort;
    }
    else
    {
        short code = httpReplyCode.toInt();

        qDebug().noquote() << QString("%1%2 HTTP code %3 (%4)")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                   serverCommSubModuleName,
                                   QString::number(code),
                                   (ssCodeShortDescription[code].isEmpty() ? "not specified"
                                                                           : ssCodeShortDescription[code]));

        if(showReplyBody)
        {
            QByteArray bytes = pReply->readAll();
            qDebug().noquote() << QString("%1%2 reply body (shorted): %3")
                                  .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                       serverCommSubModuleName,
                                       (QString::fromUtf8(bytes.data(), bytes.size()).first(SHOW_FIRST_N_OF_REPLY) + "..."));
        }

        switch (code)
        {
        case CODE_BAD_REQUEST:
            return ReplyParserAnswer::NeedToAbort;

        case CODE_NOT_AUTHORIZED:
        {
            if(canCallRefreshToken)
            {
                if(!doRefreshAccessToken())
                    return ReplyParserAnswer::NeedToAbort;
            }
            else return ReplyParserAnswer::NeedToRepeatMethod;
        }
        case CODE_METHOD_NOT_ALLOWED:
            return ReplyParserAnswer::NeedToAbort;

        case CODE_INTERNAL_SERVER_ERROR:
            return ReplyParserAnswer::NeedToAbort;

        case CODE_SUCCESS:
            return ReplyParserAnswer::AllGood;

        default:
            return ReplyParserAnswer::NeedToAbort;
        }
    }
}
