#include "servercommunicator.h"

ServerCommunicator::ServerCommunicator()
{

}

ServerCommunicator::~ServerCommunicator()
{
    lobbiesShortInfoVec.clear();
}

HostUserData ServerCommunicator::doVkLogin()
{
    // !!! STUB !!!
    bool stubFailure = false;
    if(stubFailure)
    {
        throw std::runtime_error(serverCommClassName + vkAuthFail);
    }
    return {11, "VK STUB", 1111, true};
}

HostUserData ServerCommunicator::doGoogleLogin()
{
    // !!! STUB !!!
    bool stubFailure = false;
    if(stubFailure)
    {
        throw std::runtime_error(serverCommClassName + googleAuthFail);
    }
    return {15, "GOOGLE STUB", 1212, true};
}

vector<LobbyShortInfo> &ServerCommunicator::getLobbiesShortInfo()
{
    lobbiesShortInfoVec.clear();

    // !!! STUB !!!
    LobbyShortInfo stubLobbiesShortInfo[]
            = {
                {0, "A STUB LOBBY", false, 4, 6},
                {1, "Poison", false, 1, 6},
                {2, "horseeeee", true, 3, 5},
                {3, "Лабби1234", true, 2, 4},
                {4, "JoinMePls", false, 1, 2},
                {5, "SartaSMV", true, 1, 3},
                {6, "Uncut", false, 6, 6},
                {7, "Кукуруза", true, 2, 2},
                {8, "123456789", false, 2, 3},
                {9, "professionals", true, 3, 5},
              };

    for(int i = 0; i < 10; i++)
        lobbiesShortInfoVec.push_back(stubLobbiesShortInfo[i]);

    return lobbiesShortInfoVec;
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int uniqueId)
{
    // Make request

    // !!! STUB !!!
    if(uniqueId != 0)
        throw std::runtime_error(serverCommClassName + lobbyNotFound);

    return
    {
        // LobbySystemInfo
        { 0, "A STUB LOBBY", false, 3, 6, 234, "monk", false },
        // GameSettingsInfo
        { 8, 1.000, false, 72, false },
        // vector<UserShortInfo>
        {
          {"monk", 800, true},
          {"Kisl Jsji", 2400, true},
          {"Роман Заглушевич", 11111, false},
        }
    };
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int uniqueId, const QString &enteredPassword)
{
    // Make request
    throw std::runtime_error(serverCommClassName + lobbyNotFound);
}

LobbyFullInfo ServerCommunicator::tryCreateLobby(const int hostUserId)
{
    // !!! STUB !!!
    // Need to check if user already has created lobby. If does - throw exception
    if(false)
        throw std::runtime_error(serverCommClassName + alreadyHasLobby);

    return
    {
        // LobbySystemInfo
        { 1, "MY STUB LOBBY", false, 1, 6, hostUserId, "HOST STUB", true },
        // GameSettingsInfo
        { 8, 0.100, false, 72, false },
        // vector<UserShortInfo>
        {}
    };
}

LobbyFullInfo ServerCommunicator::tryRankedQueue(const int hostUserId)
{
    // !!! STUB !!!
    // Need to check if user already in the queue
    if(false)
        throw std::runtime_error(serverCommClassName + rankedException);

    return
    {
        // LobbySystemInfo
        { -1, "RANKED STUB", false, 3, 4, -1, "HOST STUB", true },
        // GameSettingsInfo
        { 8, 2.000, true, 72, false },
        // vector<UserShortInfo>
        {
          {"monk", 800, true},
          {"Kisl Jsji", 2400, true},
          {"Роман Заглушевич", 11111, false}
        }
    };

}

void ServerCommunicator::deleteLobbyRequest(const int lobbyUniqueId)
{
    // !!! STUB !!!
}









