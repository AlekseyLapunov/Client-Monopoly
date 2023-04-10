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
    LobbyFullInfo tryJoinById(const int lobbyUniqueId);
    LobbyFullInfo tryJoinById(const int lobbyUniqueId, const QString &enteredPassword);
    LobbyFullInfo tryCreateLobby(const int hostUserId);
    LobbyFullInfo tryRankedQueue(const int hostUserId);
    void deleteLobbyRequest(const int lobbyUniqueId);
    void tryToggleReady(const int lobbyUniqueId);
    void tryToggleLobbyVision(const int lobbyUniqueId);
    void tryLobbySettingsApply(const int lobbyUniqueId, LobbySettingsCombined newSettings);
    void tryStartGame(const int lobbyUniqueId, LobbySettingsCombined settingsBase);
    void tryKickPlayer(const int playerUniqueId);
    void tryPromotePlayer(const int playerUniqueId);

    // Miscelanious
    void tryChangeNickname(const QString newNickname);

private:
    QString m_httpHost = "";
    int m_port = 0;

    HostUserData m_temporaryHostData;
    vector<LobbyShortInfo> lobbiesShortInfoVec;

};

#endif // SERVERCOMMUNICATOR_H
