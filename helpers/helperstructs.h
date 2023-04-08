#ifndef HELPERSTRUCTS_H
#define HELPERSTRUCTS_H

#include <vector>

#include <QString>
#include <QStringList>

// MenuWindow: parameters of table with lobbies
#define LOBBIES_TABLE_COLS      4
#define LOBBY_NAME_COL          0
#define IS_PASSWORDED_COL       1
#define PLAYERS_COUNT_COL       2
#define UNIQUE_ID_COL           3

// String lengths
#define NICKNAME_MAX_LEN        20
#define LOBBY_PASSWORD_MAX_LEN  10
#define UNIQUE_ID_MAX_LEN       9

// LobbyWindow: parameters of table with users
#define USERS_TABLE_COLS        3
#define NICKNAME_COL            0
#define USER_RP_COL             1
#define READY_COL               2

// bools
#define IS_READY                true
#define NOT_READY               false
#define HAS_PASSWORD            true
#define NO_PASSWORD             false
#define IS_PRIVATE              true
#define NOT_PRIVATE             false
#define IS_INFINITE             true
#define NOT_INFINITE            false

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
