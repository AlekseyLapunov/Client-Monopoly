#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator(QObject* parent) : QObject(parent)
{
    m_oauth = new QOAuth2AuthorizationCodeFlow(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_replyHandler = new QOAuthHttpServerReplyHandler(m_port, this);
    m_eventTimer = new QTimer(this);
    m_eventLoop = new QEventLoop(this);
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
    delete m_eventTimer;
    m_eventTimer = nullptr;
    delete m_eventLoop;
    m_eventLoop = nullptr;
}

HostUserData ServerCommunicator::tryVkLogin()
{
#ifdef AUTH_STUB
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[VkAuthFail]);

    m_temporaryHostData = {11, "VK STUB", 1110};
#else
    oauthConfigure(AuthType::VK);

    m_eventTimer->setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, m_eventLoop, &QEventLoop::quit );
    connect( m_eventTimer, &QTimer::timeout, m_eventLoop, &QEventLoop::quit );

    m_eventTimer->start(MS_TIMEOUT);

    m_oauth->grant();

    m_eventLoop->exec();

    if(m_eventTimer->isActive())
        return m_temporaryHostData;
    else
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[VkAuthFail]);
#endif

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::tryGoogleLogin()
{
#ifdef AUTH_STUB
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[GoogleAuthFail]);

    m_temporaryHostData = {13, "GOOGLE STUB", 1200};
#else
    oauthConfigure(AuthType::Google);

    m_eventTimer->setSingleShot(true);

    connect( this, &ServerCommunicator::authorizationProcessOver, m_eventLoop, &QEventLoop::quit );
    connect( m_eventTimer, &QTimer::timeout, m_eventLoop, &QEventLoop::quit );

    m_eventTimer->start(MS_TIMEOUT);

    m_oauth->grant();

    m_eventLoop->exec();

    if(m_eventTimer->isActive())
        return m_temporaryHostData;
    else
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[GoogleAuthFail]);
#endif

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::tryIfNoNeedToAuth()
{
#ifdef AUTH_STUB
    m_temporaryHostData = {17, "NO AUTH STUB", 2200};
#else
    QString gotAccessToken = FileManager::getToken(TokenType::Access);
    QString gotHostUniqueId = FileManager::getHostUniqueId();

    if(gotAccessToken.isEmpty() || gotHostUniqueId.isEmpty())
        throw std::runtime_error("Токен доступа отсутствует.");

    m_eventTimer->setSingleShot(true);

    connect( this, &ServerCommunicator::getInfoProcessOver, m_eventLoop, &QEventLoop::quit );
    connect( m_eventTimer, &QTimer::timeout, m_eventLoop, &QEventLoop::quit);

    m_eventTimer->start(MS_TIMEOUT);

    auto status = connect(m_networkManager, &QNetworkAccessManager::finished,
                          this, &ServerCommunicator::parseGetInfo);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart textPart;
    textPart.setRawHeader("Authorization", ("Bearer " + gotAccessToken).toUtf8());

    multiPart->append(textPart);

    m_networkManager->post(QNetworkRequest(makeAddress(m_host, m_port,
                                                       (m_getInfoMethodPrefix +
                                                        gotHostUniqueId +
                                                        m_getInfoMethodPostfix))),
                           multiPart);

    m_eventLoop->exec();

    if(retryIfNotNeedToAuth)
    {
        retryIfNotNeedToAuth = false;
        return m_temporaryHostData = tryIfNoNeedToAuth();
    }

    if(m_eventTimer->isActive())
        return m_temporaryHostData;
    else
        throw std::runtime_error(ssClassNames[ServerCommCN] + ssRuntimeErrors[GoogleAuthFail]);
#endif
    return m_temporaryHostData;
}

HostUserData ServerCommunicator::getCurrentHostInfo()
{
    // Need to request Host Data from server data base
    return m_temporaryHostData;
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
            throw std::runtime_error(ssClassNames[ServerCommCN] + (authType == AuthType::VK ? ssRuntimeErrors[VkAuthFail]
                                                                                        : ssRuntimeErrors[GoogleAuthFail]));

        QJsonObject codeObj;
        codeObj.insert("code", data.value("code").toString());
        QJsonDocument codeDoc(codeObj);
        codeDoc.toJson(QJsonDocument::Compact);

        auto status = connect(m_networkManager, &QNetworkAccessManager::finished,
                              this, &ServerCommunicator::parseAuthReply);

        QNetworkRequest codeTransferRequest(makeAddress(m_host, m_port, m_authMethod[authType]));
        codeTransferRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(jsonContentType));
        m_networkManager->post(codeTransferRequest, codeDoc.toJson());
    });
}

void ServerCommunicator::parseAuthReply(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    qDebug().noquote() << "Auth code: " << statusCode.toString();

    if(!statusCode.isValid())
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не валиден");
    else if(statusCode.toInt() != CODE_SUCCESS)
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не 200 (" + statusCode.toString().toStdString() + ")");

    FileManager::commitTokens(reply->readAll());

    QJsonObject jsonObj = QJsonDocument::fromJson(reply->readAll()).object();

    m_temporaryHostData.uniqueId = jsonObj.value(ssJsonHostDisplayData[HostId]).toInt();
    m_temporaryHostData.nickname = jsonObj.value(ssJsonHostDisplayData[HostNickname]).toString();
    m_temporaryHostData.rpCount  = jsonObj.value(ssJsonHostDisplayData[HostRpCount]).toInt();
    m_temporaryHostData.isGuest  = jsonObj.value(ssJsonHostDisplayData[HostIsGuest]).toBool();

    FileManager::commitHostUniqueId(m_temporaryHostData.uniqueId);

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
        try
        {
            doRefreshAccessToken();
        }
        catch (std::exception &e)
        {
            qDebug().noquote() << e.what();
            return;
        }

        retryIfNotNeedToAuth = true;
        emit accessTokenRefreshed();
        emit getInfoProcessOver();
        return;
    }
    else if((statusCode.toInt() != CODE_SUCCESS) && (statusCode.toInt() != CODE_NOT_AUTHORIZED))
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не подходит к выборке (" + statusCode.toString().toStdString() + ")");

    QJsonObject jsonObj = QJsonDocument::fromJson(reply->readAll()).object();

    m_temporaryHostData.uniqueId = jsonObj.value(ssJsonHostDisplayData[HostId]).toInt();
    m_temporaryHostData.nickname = jsonObj.value(ssJsonHostDisplayData[HostNickname]).toString();
    m_temporaryHostData.rpCount  = jsonObj.value(ssJsonHostDisplayData[HostRpCount]).toInt();
    m_temporaryHostData.isGuest  = jsonObj.value(ssJsonHostDisplayData[HostIsGuest]).toBool();

    FileManager::commitHostUniqueId(m_temporaryHostData.uniqueId);

    emit getInfoProcessOver();

    reply->deleteLater();
}

void ServerCommunicator::parseRefreshAccessToken(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    qDebug().noquote() << "Refresh Access Token code: " << statusCode.toString();

    if(!statusCode.isValid())
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не валиден");
    else if(statusCode.toInt() == CODE_NOT_AUTHORIZED)
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[ServerCommCN])
                           << "Токен доступа не действителен. Необходима авторизация.";
        return;
    }
    else if(statusCode.toInt() != CODE_SUCCESS)
        throw std::runtime_error(ssClassNames[ServerCommCN] + "Полученный код не равен 200 ("
                                 + statusCode.toString().toStdString() + ")");

    FileManager::commitTokens(reply->readAll());

    emit refreshTokenProcessOver();

    reply->deleteLater();
}

QUrl ServerCommunicator::makeAddress(QString host, int port, QString additionalParameters)
{
    return ("https://" + host + ":" + QString::number(port) + (!additionalParameters.isEmpty() ? ("/" + additionalParameters) : ""));
}

void ServerCommunicator::doRefreshAccessToken()
{
    QTimer localTimer;
    QEventLoop localEventLoop;

    connect( this, &ServerCommunicator::refreshTokenProcessOver, &localEventLoop, &QEventLoop::quit );
    connect( &localTimer, &QTimer::timeout, &localEventLoop, &QEventLoop::quit);

    localTimer.start(MS_TIMEOUT);

    auto status = connect(m_networkManager, &QNetworkAccessManager::finished,
                          this, &ServerCommunicator::parseRefreshAccessToken);

    QString gotRefreshToken = FileManager::getToken(TokenType::Refresh);

    if(gotRefreshToken.isEmpty())
        return;

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart textPart;
    textPart.setRawHeader("Authorization", ("Bearer " + gotRefreshToken.toUtf8()));

    multiPart->append(textPart);

    m_networkManager->post(QNetworkRequest(makeAddress(m_host, m_port, m_refreshAccessTokenMethod)),
                           multiPart);

    localEventLoop.exec();

    if(localTimer.isActive())
        return;
    else
        throw std::runtime_error("Не удалось обновить токен.");
}









