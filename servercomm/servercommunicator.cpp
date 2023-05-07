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
}

HostUserData ServerCommunicator::doVkLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {11, "VK STUB", 1110};
    return m_temporaryHostData;
#else
    oauthConfigure(AuthType::VK);

    uint8_t thisSubModuleId = AuthSubModule;
    QString usingHttpMethod = "";

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::SpecAuth, "",
                          "", "") == RequestManagerAnswer::RequestAllGood)
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
}

HostUserData ServerCommunicator::doGoogleLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {13, "GOOGLE STUB", 1200};
    return m_temporaryHostData;
#else
    oauthConfigure(AuthType::Google);

    uint8_t thisSubModuleId = AuthSubModule;
    QString usingHttpMethod = "";

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::SpecAuth, "",
                          "", "") == RequestManagerAnswer::RequestAllGood)
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
}

HostUserData ServerCommunicator::doGuestLogin(bool &ok)
{
#ifdef AUTH_STUB
    ok = true;
    m_temporaryHostData = {21, "GUEST STUB", 0};
    return m_temporaryHostData;
#else
    uint8_t thisSubModuleId = AuthSubModule;
    QString usingHttpMethod = httpMethods[PostAuthAsGuest];

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, "",
                          "", "") == RequestManagerAnswer::RequestAllGood)
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
    uint8_t thisSubModuleId = GetUserInfoSubModule;

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter").arg(serverCommSubModule[GetUserInfoSubModule]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;

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
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return {};
    }

    QString usingHttpMethod = httpMethods[GetUsersGetInfoByUserId].arg(gotHostUniqueId);

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpGet, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken), "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId] && retryCheckIfNoNeedToAuth)
        {
            localCounter++;
            return m_temporaryHostData = checkIfNoNeedToAuth(ok, localCounter);
        }
        if(serverCommSubModuleRepeat[thisSubModuleId] && !retryCheckIfNoNeedToAuth)
        {
            localCounter++;
            return getCurrentHostInfo(ok, retryCheckIfNoNeedToAuth, localCounter);
        }
        ok = true;
        return m_temporaryHostData;
    }
    else
    {
        ok = false;
        return {};
    }
#endif
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

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
            return;

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          QString("{\"newNickname\": \"%1\"}")
                          .arg(newNickname)) == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
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
    uint8_t thisSubModuleId = GetLobbiesListSubModule;
    QString usingHttpMethod = httpMethods[GetLobbiesGetList];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return m_lobbiesShortInfoVec;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return m_lobbiesShortInfoVec;
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpGet, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return m_lobbiesShortInfoVec = getLobbiesShortInfo(ok, localCounter);
        }
        ok = true;
        return m_lobbiesShortInfoVec;
    }
    else
    {
        ok = false;
        return m_lobbiesShortInfoVec;
    }
#endif
}

vector<LobbyShortInfo> &ServerCommunicator::getStableLobbiesList()
{
    return m_lobbiesShortInfoVec;
}

LobbyFullInfo ServerCommunicator::createLobby(LobbySettings priorSettings,
                                              bool &ok, uint8_t localCounter)
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
    uint8_t thisSubModuleId = CreateLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCreate];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return {};
        }

    QString requestBody = makeServerFullLobbyJson(priorSettings);

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          requestBody) == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return createLobby(priorSettings, ok, localCounter);
        }
        ok = true;
        return m_temporaryLobbyFullInfo;
    }
    else
    {
        ok = false;
        return {};
    }
#endif
}

LobbyFullInfo ServerCommunicator::connectToLobby(const int lobbyUniqueId, bool &ok,
                                                 bool &isPassworded, uint8_t localCounter)
{
#ifdef LOBBIES_STUB
    if(lobbyUniqueId != 0)
        return;

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
    uint8_t thisSubModuleId = ConnectLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesConnectById].arg(QString::number(lobbyUniqueId));

    globalIsPassworded = false;

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return {};
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "{}") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return connectToLobby(lobbyUniqueId, ok, isPassworded, localCounter);
        }
        if(globalIsPassworded)
        {
            isPassworded = true;
            ok = false;
            return {};
        }
        ok = true;
        return m_temporaryLobbyFullInfo;
    }
    else
    {
        ok = false;
        return {};
    }
#endif
}

LobbyFullInfo ServerCommunicator::connectToLobby(const int lobbyUniqueId, const QString &enteredPassword,
                                                 bool &ok, uint8_t localCounter)
{
    // Make request
#ifdef LOBBIES_STUB
    return {};
#else
    uint8_t thisSubModuleId = ConnectLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesConnectById].arg(QString::number(lobbyUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return {};
        }

    QString requestBody = QString("{\"password\": \"%1\"}").arg(enteredPassword);

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          requestBody) == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return connectToLobby(lobbyUniqueId, enteredPassword, ok, localCounter);
        }
        ok = true;
        return m_temporaryLobbyFullInfo;
    }
    else
    {
        ok = false;
        return {};
    }
#endif
}

LobbyFullInfo ServerCommunicator::connectToRankedLobby(bool &ok, uint8_t localCounter)
{
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
#else
    uint8_t thisSubModuleId = ConnectRankedLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesRankedConnect];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return {};
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return connectToRankedLobby(ok, localCounter);
        }
        ok = true;
        return m_temporaryLobbyFullInfo;
    }
    else
    {
        ok = false;
        return {};
    }
#endif
}

LobbyFullInfo ServerCommunicator::getInfoLobby(bool &ok, uint8_t localCounter)
{
    uint8_t thisSubModuleId = GetInfoLobbySubModule;
    QString usingHttpMethod = httpMethods[GetLobbiesCurrentGetInfo];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return {};
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return {};
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpGet, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return getInfoLobby(ok, localCounter);
        }
        ok = true;
        return m_temporaryLobbyFullInfo;
    }
    else
    {
        ok = false;
        return {};
    }

}

void ServerCommunicator::deleteLobby(bool &ok, uint8_t localCounter)
{
#ifdef LOBBIES_INSIDE_STUB
    return;
#else
    uint8_t thisSubModuleId = DeleteLobbySubModule;
    QString usingHttpMethod = httpMethods[DeleteLobbiesCurrent];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return;
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpDelete, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return deleteLobby(ok, localCounter);
        }
        ok = true;
        return;
    }
    else
    {
        ok = false;
        return;
    }
#endif
}

void ServerCommunicator::switchReadiness(bool &ok, uint8_t localCounter)
{
#ifdef LOBBIES_INSIDE_STUB
    return;
#else
    uint8_t thisSubModuleId = SwitchReadinessSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentSwitchReadiness];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return;
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return switchReadiness(ok, localCounter);
        }
        ok = true;
        return;
    }
    else
    {
        ok = false;
        return;
    }
#endif
}

void ServerCommunicator::updateLobbySettings(LobbySettings newSettings,
                                             bool &ok, uint8_t localCounter)
{
#ifdef LOBBIES_INSIDE_STUB
    return;
#else
    uint8_t thisSubModuleId = UpdateLobbySettingsSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentUpdateSettings];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return;
        }

    QString requestBody = makeServerFullLobbyJson(newSettings);

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          requestBody) == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return updateLobbySettings(newSettings, ok, localCounter);
        }
        ok = true;
        return;
    }
    else
    {
        ok = false;
        return;
    }
#endif
}

void ServerCommunicator::runGame(bool &ok, uint8_t localCounter)
{
#ifdef LOBBIES_INSIDE_STUB
    return;
#else

#endif
}

void ServerCommunicator::kickPlayer(const int playerUniqueId,
                                    bool &ok, uint8_t localCounter)
{
#ifdef LOBBIES_INSIDE_STUB
    return;
#else
    uint8_t thisSubModuleId = KickPlayerSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentPlayersKickById]
                              .arg(QString::number(playerUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return;
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return kickPlayer(playerUniqueId, ok, localCounter);
        }
        ok = true;
        return;
    }
    else
    {
        ok = false;
        return;
    }
#endif
}

void ServerCommunicator::raisePlayer(const int playerUniqueId,
                                     bool &ok, uint8_t localCounter)
{
#ifdef LOBBIES_INSIDE_STUB
    return;
#else
    uint8_t thisSubModuleId = RaisePlayerSubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentPlayersRaiseById]
                              .arg(QString::number(playerUniqueId));

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return;
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return raisePlayer(playerUniqueId, ok, localCounter);
        }
        ok = true;
        return;
    }
    else
    {
        ok = false;
        return;
    }
#endif
}

void ServerCommunicator::disconnectFromLobby(bool &ok, uint8_t localCounter)
{
    uint8_t thisSubModuleId = DisconnectLobbySubModule;
    QString usingHttpMethod = httpMethods[PostLobbiesCurrentDisconnect];

    if(localCounter >= LOCAL_COUNTER_MAX)
    {
        qDebug().noquote() << QString("%1: Reached max local counter")
                              .arg(serverCommSubModule[thisSubModuleId]);
        ok = false;
        return;
    }

    serverCommSubModuleRepeat[thisSubModuleId] = false;
    QString gotAccessToken = FileManager::getToken(TokenType::Access);

    if(gotAccessToken.isEmpty())
        if(!doRefreshAccessToken())
        {
            ok = false;
            return;
        }

    if(basicRequestManage(thisSubModuleId, usingHttpMethod,
                          HttpMethodType::HttpPost, authorizationRawHeader,
                          authorizationHeaderContent.arg(gotAccessToken),
                          "") == RequestManagerAnswer::RequestAllGood)
    {
        if(serverCommSubModuleRepeat[thisSubModuleId])
        {
            localCounter++;
            return disconnectFromLobby(ok, localCounter);
        }
        ok = true;
        return;
    }
    else
    {
        ok = false;
        return;
    }
}

void ServerCommunicator::catchReplyAuth(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString replyBodyString = QString::fromUtf8(bytes.data(), bytes.size());

    if(basicReplyManage(reply, AuthSubModule, replyBodyString, false) != ReplyManagerAnswer::ReplyAllGood)
        return;

    FileManager::commitTokens(bytes);

    QJsonObject jsonObj = QJsonDocument::fromJson(bytes).object();

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
            qDebug().noquote() << "jsonLobbyValue is fractured";
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

uint8_t ServerCommunicator::basicReplyManage(QNetworkReply *pReply, uint8_t serverCommSubModuleId, QString replyBody,
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
        qDebug().noquote() << QString("%1%2 reply body (shorted): %3")
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
            return ReplyManagerAnswer::ReplyAllGood;
        }
        case CODE_FORBIDDEN:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            if(serverCommSubModuleId == ConnectLobbySubModule)
                globalIsPassworded = true;
            emitSignalBySubModuleId(serverCommSubModuleId);
            return ReplyManagerAnswer::ReplyNeedToAbort;
        }
        default:
        {
            serverCommSubModuleRepeat[serverCommSubModuleId] = false;
            return ReplyManagerAnswer::ReplyNeedToAbort;
        }
    }
}

uint8_t ServerCommunicator::basicRequestManage(uint8_t subModuleId, QString method, uint8_t methodType,
                                               QString headerName, QString headerValue, QString requestBody,
                                               bool isWaiting)
{
    QNetworkAccessManager localNetworkAccessManager(this);

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

    if(isWaiting)
        localEventLoop.exec();

    localTimer.disconnect();
    localEventLoop.disconnect();

    if(localTimer.isActive())
        return RequestManagerAnswer::RequestAllGood;
    else
    {
        qDebug().noquote() << QString("%1%2 request timed out")
                              .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
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
    m_oauthNetworkManager->disconnect();
    m_oauthCodeFlow->disconnect();

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
        qDebug().noquote() << QString("Got auth-code from service: %1..." ).arg((data.value("code").toString()).first(5));

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

        QNetworkRequest request(makeAddress(host, port,
                                            httpMethods[authType]));
        request.setHeader(QNetworkRequest::ContentTypeHeader, jsonContentType.toUtf8());

        m_oauthNetworkManager->post(request, codeDoc.toJson());
    });
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









