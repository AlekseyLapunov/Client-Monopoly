#ifndef HELPERSTRUCTS_H
#define HELPERSTRUCTS_H

#include <vector>

#include <QString>
#include <QStringList>

#define LOBBIES_TABLE_COLS 4
#define LOBBY_NAME_COL 0
#define IS_PASSWORDED_COL 1
#define PLAYERS_COUNT_COL 2
#define UNIQUE_ID_COL 3
#define NICKNAME_MAX_LENGTH 20
#define LOBBY_PASSWORD_MAX_LENGTH 10
#define UNIQUE_ID_MAX_LENGTH 9

using std::vector;
using std::count_if;

// Lobbies short information inside MenuWindow's center table
struct LobbyShortInfo
{
    int uniqueId;
    QString lobbyName;
    bool isPassworded;
    short curPlayersCount;
    short maxPlayersCount;
};

// Detailed lobby system info
struct LobbySystemInfo
{
    int uniqueId;
    QString lobbyName;
    bool isPassworded;
    short curPlayersCount;
    short maxPlayersCount;
    int ownerUniqueId;
    QString ownerName;
    bool isPrivate;
};

// Users short information inside LobbyWindow's table of users
struct UserShortInfo
{
    QString nickname;
    int rpCount;
    bool isReady;
};

// Game settings information inside LobbyWindow's section
struct GameSettingsInfo
{
    short turnTime;
    float moneyRightBorder; // represents billions
    bool isMaxMoneyInfinite;
    short turnsRightBorder;
    bool isMaxTurnsInfinite;
};

// Full lobby info inside LobbyWindow
struct LobbyFullInfo
{
    LobbySystemInfo lobbySystem;
    GameSettingsInfo gameSettings;
    vector<UserShortInfo> usersInTable;
};

// Host-user data
struct HostUserData
{
    int uniqueUserId;
    QString userName;
    int userRpCount;
    bool uses3dDices;
};

#endif // HELPERSTRUCTS_H
