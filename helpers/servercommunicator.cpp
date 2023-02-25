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
        throw std::runtime_error("VK auth fail");
    }
    return {11, "Алексей Коритса", 1111};
}

HostUserData ServerCommunicator::doGoogleLogin()
{
    // !!! STUB !!!
    bool stubFailure = false;
    if(stubFailure)
    {
        throw std::runtime_error("Google auth fail");
    }
    return {15, "Aleksey Koritsa", 1212};
}

vector<LobbyShortInfo> &ServerCommunicator::getLobbiesShortInfo()
{
    lobbiesShortInfoVec.clear();

    // !!! STUB !!!
    LobbyShortInfo stubLobbiesShortInfo[]
            = {
                {"Моё лобби", true, 4, 6},
                {"Poison", false, 1, 6},
                {"horseeeee", true, 3, 5},
                {"Лабби1234", true, 2, 4},
                {"JoinMePls", false, 1, 2},
                {"SartaSMV", true, 1, 3},
                {"Uncut", false, 6, 6},
                {"Кукуруза", true, 2, 2},
                {"123456789", false, 2, 3},
                {"pofessionals", true, 3, 5},
               };

    for(int i = 0; i < 10; i++)
        lobbiesShortInfoVec.push_back(stubLobbiesShortInfo[i]);

    return lobbiesShortInfoVec;
}









