#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <vector>
#include <stdexcept>

#include <QString>

#include "common/sourcestructs.h"
#include "common/sourcestrings.h"

class ServerCommunicator
{
public:
    ServerCommunicator();
    ~ServerCommunicator();

    // Logins
    HostUserData doVkLogin();
    HostUserData doGoogleLogin();

    // Host info
    HostUserData getCurrentHostInfo();

    // Lobbies
    vector<LobbyShortInfo>& getLobbiesShortInfo();
    LobbyFullInfo tryJoinById(const QString lobbyUniqueId);
    LobbyFullInfo tryJoinById(const QString lobbyUniqueId, const QString &enteredPassword);
    LobbyFullInfo tryCreateLobby(const int hostUserId);
    LobbyFullInfo tryRankedQueue(const int hostUserId);
    void deleteLobbyRequest(const QString lobbyUniqueId);
    void tryToggleReady(const QString lobbyUniqueId);
    void tryLobbySettingsApply(const QString lobbyUniqueId, LobbySettings newSettings);
    void tryStartGame(const QString lobbyUniqueId, LobbySettings settingsBase);
    void tryKickPlayer(const QString lobbyUniqueId, const int playerUniqueId);
    void tryPromotePlayer(const QString lobbyUniqueId, const int playerUniqueId);

    // Miscelanious
    void tryChangeNickname(const QString newNickname);

private:
    QString m_httpHost = "";
    int m_port = 0;

    HostUserData m_temporaryHostData;
    vector<LobbyShortInfo> lobbiesShortInfoVec;

};

#endif // SERVERCOMMUNICATOR_H
