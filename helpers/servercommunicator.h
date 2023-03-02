#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <vector>
#include <stdexcept>

#include <QString>

#include "helperstructs.h"
#include "sourcestrings.h"

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
    LobbyFullInfo tryJoinById(const int uniqueId);
    LobbyFullInfo tryJoinById(const int uniqueId, const QString &enteredPassword);

private:
    QString m_httpHost = "";
    int m_port = 0;

    vector<LobbyShortInfo> lobbiesShortInfoVec;

};

#endif // SERVERCOMMUNICATOR_H
