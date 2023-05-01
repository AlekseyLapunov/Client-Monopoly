#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator(QObject* parent) : QObject(parent)
{
    m_oauth = new QOAuth2AuthorizationCodeFlow(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_replyHandler = new QOAuthHttpServerReplyHandler(m_port, this);
}

ServerCommunicator::~ServerCommunicator()
{
    m_lobbiesShortInfoVec.clear();
    m_lobbiesShortInfoVec.resize(0);
    delete m_oauth;
    m_oauth = nullptr;
    delete m_networkManager;
    m_networkManager = nullptr;
    delete m_replyHandler;
    m_replyHandler = nullptr;
}

HostUserData ServerCommunicator::doVkLogin(bool &ok)
{
#ifdef AUTH_STUB
    m_temporaryHostData = {11, "VK STUB", 1110};
#else
    oauthConfigure(AuthType::VK);

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

    eventTimer.start(MS_TIMEOUT);

    m_oauth->grant();

    eventLoop.exec();

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        ok = false;
        return {};
    }
#endif

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::doGoogleLogin(bool &ok)
{
#ifdef AUTH_STUB
    m_temporaryHostData = {13, "GOOGLE STUB", 1200};
#else
    oauthConfigure(AuthType::Google);

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit );

    eventTimer.start(MS_TIMEOUT);

    m_oauth->grant();

    eventLoop.exec();

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        ok = false;
        return {};
    }
#endif

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::checkIfNoNeedToAuth(bool &ok)
{
#ifdef AUTH_STUB
    m_temporaryHostData = {17, "NO AUTH STUB", 2200};
#else
    if(!doRefreshAccessToken())
    {
        ok = false;
        return {};
    }

    getCurrentHostInfo(ok);

    if(ok)
        return m_temporaryHostData;
    else
        return {};

#endif
    return m_temporaryHostData;
}

HostUserData ServerCommunicator::getCurrentHostInfo(bool &ok)
{
    retryMethod = false;

    QString gotAccessToken = FileManager::getToken(TokenType::Access);
    QString gotHostUniqueId = FileManager::getHostUniqueId();

    if(gotAccessToken.isEmpty() || gotHostUniqueId.isEmpty())
    {
        ok = false;
        return {};
    }

    QTimer eventTimer;
    QEventLoop eventLoop;

    eventTimer.setSingleShot(true);

    connect( this, &ServerCommunicator::getInfoProcessOver, &eventLoop, &QEventLoop::quit );
    connect( &eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    eventTimer.start(MS_TIMEOUT);

    auto status = connect(m_networkManager, &QNetworkAccessManager::finished,
                         this, &ServerCommunicator::parseGetInfo);

    QNetworkRequest request(makeAddress(m_host, m_port,
                                        m_httpMethods[GetUsersGetInfoById].arg(gotHostUniqueId)));
    request.setRawHeader(m_authorizationRawHeader.toUtf8(), m_authorizationHeaderContent.arg(gotAccessToken).toUtf8());

    m_networkManager->get(request);

    eventLoop.exec();

    if(retryMethod)
        return m_temporaryHostData = checkIfNoNeedToAuth(ok);

    if(eventTimer.isActive())
    {
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        ok = false;
        return {};
    }
}

vector<LobbyShortInfo> &ServerCommunicator::getLobbiesShortInfo()
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
#endif

    return m_lobbiesShortInfoVec;
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int lobbyUniqueId)
{
    // Make request

#ifdef LOBBIES_STUB
    if(lobbyUniqueId != 0)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[LobbyNotFound]);

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
#endif
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int lobbyUniqueId, const QString &enteredPassword)
{
    // Make request
#ifdef LOBBIES_STUB
    throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[LobbyNotFound]);
#endif
}

LobbyFullInfo ServerCommunicator::tryCreateLobby(const int hostUserId, LobbySettings priorSettings)
{
#ifdef LOBBIES_STUB
    // Need to check if user already has created lobby. If does - throw exception
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[AlreadyHasLobby]);

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
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[AlreadyInQueue]);

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
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[ToggleReadyFail]);
#endif
}

void ServerCommunicator::tryLobbySettingsApply(const int lobbyUniqueId, LobbySettings newSettings)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[ApplySettingsFail]);
#endif
}

void ServerCommunicator::tryStartGame(const int lobbyUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[StartGameFail]);
#endif
}

void ServerCommunicator::tryKickPlayer(const int lobbyUniqueId, const int playerUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[KickPlayerFail]);
#endif
}

void ServerCommunicator::tryPromotePlayer(const int lobbyUniqueId, const int playerUniqueId)
{
#ifdef LOBBIES_INSIDE_STUB
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[PromotePlayerFail]);
#endif
}

void ServerCommunicator::changeNickname(const QString newNickname)
{
    // !!! STUB !!!
}

void ServerCommunicator::oauthConfigure(uint8_t authType)
{
    m_replyHandler->setCallbackPath(m_redirectUri[authType].toString());
    QString callbackText = FileManager::getPageAsCallbackText();
    m_replyHandler->setCallbackText(callbackText);
    m_oauth->setReplyHandler(m_replyHandler);
    m_oauth->setAccessTokenUrl(m_tokenUrl[authType]);
    m_oauth->setAuthorizationUrl(m_authUrl[authType]);
    m_oauth->setClientIdentifier(m_clientId[authType]);
    m_oauth->setScope(QString::number(m_scopeMask[authType]));
    QObject::connect(m_oauth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(m_oauth, &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived, [=](const QVariantMap data)
    {
        if(data.value("code").toString().isEmpty())
            return;

        QJsonObject codeObj;
        codeObj.insert("code", data.value("code").toString());
        QJsonDocument codeDoc(codeObj);
        codeDoc.toJson(QJsonDocument::Compact);

        auto status = connect(m_networkManager, &QNetworkAccessManager::finished,
                              this, &ServerCommunicator::parseAuthReply);

        QNetworkRequest request(makeAddress(m_host, m_port,
                                            m_httpMethods[authType]));
        request.setHeader(QNetworkRequest::ContentTypeHeader, m_jsonContentType.toUtf8());

        m_networkManager->post(request, codeDoc.toJson());
    });
}

void ServerCommunicator::parseAuthReply(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    qDebug().noquote() << "Auth code: " << statusCode.toString();

    if(!statusCode.isValid())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "Status code is not valid";
        return;
    }
    else if(statusCode.toInt() != CODE_SUCCESS)
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << QString("Status code is not success (%1)").arg(statusCode.toString());

    FileManager::commitTokens(reply->readAll());

    QJsonObject jsonObj = QJsonDocument::fromJson(reply->readAll()).object();

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

void ServerCommunicator::parseGetInfo(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    qDebug().noquote() << "Get info code: " << statusCode.toString();

    if(!statusCode.isValid())
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не валиден");
    else if(statusCode.toInt() == CODE_NOT_AUTHORIZED)
    {
        if(!doRefreshAccessToken())
        {
            qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                               << "Refresh Access Token: Can not provide";
            return;
        }

        retryMethod = true;
        emit getInfoProcessOver();
        return;
    }
    else if((statusCode.toInt() != CODE_SUCCESS) && (statusCode.toInt() != CODE_NOT_AUTHORIZED))
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не подходит к выборке (" + statusCode.toString().toStdString() + ")");

    QJsonObject jsonObj = QJsonDocument::fromJson(reply->readAll()).object();

    m_temporaryHostData.uniqueId = jsonObj[ssJsonUserMeta[HostId]].toInt();
    m_temporaryHostData.nickname = jsonObj[ssJsonUserMeta[HostNickname]].toString();
    m_temporaryHostData.rpCount  = jsonObj[ssJsonUserMeta[HostRpCount]].toInt();
    m_temporaryHostData.isGuest  = jsonObj[ssJsonUserMeta[HostIsGuest]].toBool();

    FileManager::commitHostData(m_temporaryHostData.uniqueId,
                                m_temporaryHostData.nickname,
                                m_temporaryHostData.rpCount,
                                m_temporaryHostData.isGuest);

    emit getInfoProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::parseRefreshAccessToken(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    qDebug().noquote() << "Refresh Access Token: HTTP code " << statusCode.toString();

    if(!statusCode.isValid())
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "Refresh Access Token: Status code not valid.";
        return;
    }
    else if(statusCode.toInt() == CODE_NOT_AUTHORIZED)
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "Refresh Access Token: Need to authorize";
        return;
    }
    else if(statusCode.toInt() != CODE_SUCCESS)
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "Refresh Access Token: Code is not 200";
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObj = jsonDoc.object();

    QString accessToken = jsonObj[ssJsonUserMeta[AccessToken]].toString();
    FileManager::commitToken(TokenType::Access, accessToken);

    emit refreshTokenProcessOver();

    reply->deleteLater();
}

QUrl ServerCommunicator::makeAddress(QString host, int port, QString additionalParameters)
{
    return QUrl(QString("https://1%:%2").arg(host, port)
                + QString(!additionalParameters.isEmpty() ? "/%1" : "").arg(additionalParameters));
}

bool ServerCommunicator::doRefreshAccessToken()
{
    QTimer localTimer;
    QEventLoop localEventLoop;

    connect(this, &ServerCommunicator::refreshTokenProcessOver, &localEventLoop, &QEventLoop::quit);
    connect(&localTimer, &QTimer::timeout, &localEventLoop, &QEventLoop::quit);

    localTimer.start(MS_TIMEOUT);

    auto status = connect(m_networkManager, &QNetworkAccessManager::finished,
                          this, &ServerCommunicator::parseRefreshAccessToken);

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









