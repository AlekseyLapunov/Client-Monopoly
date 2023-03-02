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
                {0, "Моё лобби", true, 4, 6},
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
    throw std::runtime_error(serverCommClassName + lobbyNotFound);
}

LobbyFullInfo ServerCommunicator::tryJoinById(const int uniqueId, const QString &enteredPassword)
{
    // Make request
    throw std::runtime_error(serverCommClassName + lobbyNotFound);
}









