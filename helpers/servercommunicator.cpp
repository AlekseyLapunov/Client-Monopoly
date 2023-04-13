#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator()
{

}

ServerCommunicator::~ServerCommunicator()
{
    lobbiesShortInfoVec.clear();
}

HostUserData ServerCommunicator::tryVkLogin()
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[VkAuthFail]);

    m_temporaryHostData = {11, "VK STUB", 1110};

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::tryGoogleLogin()
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[GoogleAuthFail]);

    m_temporaryHostData = {15, "GOOGLE STUB", 1200};

    return m_temporaryHostData;
}

HostUserData ServerCommunicator::getCurrentHostInfo()
{
    // !!! STUB !!!
    // Need to request Host Data from server data base
    return m_temporaryHostData;
}

vector<LobbyShortInfo> &ServerCommunicator::getLobbiesShortInfo()
{
    lobbiesShortInfoVec.clear();

    // !!! STUB !!!
    LobbyShortInfo stubLobbiesShortInfo[]
            = {
                {"0", "A STUB LOBBY", NO_PASSWORD, 3, 6},
                {"1", "Poison", NO_PASSWORD, 1, 6},
                {"2", "horseeeee", HAS_PASSWORD, 3, 5},
                {"3", "Лабби1234", HAS_PASSWORD, 2, 4},
                {"4", "JoinMePls", NO_PASSWORD, 1, 2},
                {"5", "SartaSMV", HAS_PASSWORD, 1, 3},
                {"6", "Uncut", NO_PASSWORD, 6, 6},
                {"7", "Кукуруза", HAS_PASSWORD, 2, 2},
                {"8", "123456789", NO_PASSWORD, 2, 3},
                {"9", "professionals", HAS_PASSWORD, 3, 5},
              };

    for(int i = 0; i < 10; i++)
        lobbiesShortInfoVec.push_back(stubLobbiesShortInfo[i]);

    return lobbiesShortInfoVec;
}

LobbyFullInfo ServerCommunicator::tryJoinById(const QString lobbyUniqueId)
{
    // Make request

    // !!! STUB !!!
    if(lobbyUniqueId != "0")
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[LobbyNotFound]);

    return
    {
        {
            "0", "A STUB LOBBY", "", 6, 23, false, "", 0,
            8, 1.000, IS_INFINITE, 72, NOT_INFINITE
        },
        // vector<UserShortInfo>
        {
          {"monk", 800, IS_READY, 23},
          {"Kisl Jsji", 2400, IS_READY, 67},
          {"Роман Заглушевич", 11111, NOT_READY, 63},
        }
    };
}

LobbyFullInfo ServerCommunicator::tryJoinById(const QString lobbyUniqueId, const QString &enteredPassword)
{
    // Make request
    throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[LobbyNotFound]);
}

LobbyFullInfo ServerCommunicator::tryCreateLobby(const int hostUserId, LobbySettings priorSettings)
{
    // !!! STUB !!!
    // Need to check if user already has created lobby. If does - throw exception
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[AlreadyHasLobby]);

    return
    {
        {
           "1", "My Lobby", "", 6, hostUserId, false, "", 0,
            8, 0.100, NOT_INFINITE, 72, NOT_INFINITE
        },
        // vector<UserShortInfo>
        {}
    };
}

LobbyFullInfo ServerCommunicator::tryRankedQueue(const int hostUserId)
{
    // !!! STUB !!!
    // Need to check if user already in the queue
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[AlreadyInQueue]);

    return
    {
        {
            "-1", "RANKED STUB", "", 4, -1, false, "", 0,
            8, 2.000, IS_INFINITE, 72, NOT_INFINITE
        },
        // vector<UserShortInfo>
        {
          {"Максим Баландин", 1435, IS_READY, 92},
          {"Nevada", 1400, IS_READY, 34},
          {"PPCD", 1215, IS_READY, 77}
        }
    };

}

void ServerCommunicator::deleteLobby(const QString lobbyUniqueId)
{
    // !!! STUB !!!
}

void ServerCommunicator::tryToggleReady(const QString lobbyUniqueId)
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[ToggleReadyFail]);
}

void ServerCommunicator::tryLobbySettingsApply(const QString lobbyUniqueId, LobbySettings newSettings)
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[ApplySettingsFail]);
}

void ServerCommunicator::tryStartGame(const QString lobbyUniqueId)
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[StartGameFail]);
}

void ServerCommunicator::tryKickPlayer(const QString lobbyUniqueId, const int playerUniqueId)
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[KickPlayerFail]);
}

void ServerCommunicator::tryPromotePlayer(const QString lobbyUniqueId, const int playerUniqueId)
{
    // !!! STUB !!!
    if(false)
        throw std::runtime_error(ssServerCommClassName + ssRuntimeErrors[PromotePlayerFail]);
}

void ServerCommunicator::changeNickname(const QString newNickname)
{
    // !!! STUB !!!
}









