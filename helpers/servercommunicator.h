#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <vector>
#include <stdexcept>

#include <QString>

#include "helperstructs.h"

using std::vector;

class ServerCommunicator
{
public:
    ServerCommunicator();
    ~ServerCommunicator();

    // Logins
    HostUserData doVkLogin();
    HostUserData doGoogleLogin();

    // Lobbies
    vector<LobbyShortInfo>& getLobbiesShortInfo();
    LobbyFullInfo tryJoinById(int uniqueId);
    LobbyFullInfo tryJoinById(int uniqueId, QString &enteredPassword);

private:
    QString m_httpHost = "";
    int m_port = 0;

    vector<LobbyShortInfo> lobbiesShortInfoVec;

};

#endif // SERVERCOMMUNICATOR_H
