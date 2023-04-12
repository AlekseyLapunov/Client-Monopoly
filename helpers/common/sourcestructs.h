#ifndef SOURCESTRUCTS_H
#define SOURCESTRUCTS_H

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
#define USERS_TABLE_COLS        4
#define NICKNAME_COL            0
#define USER_RP_COL             1
#define READY_COL               2
#define PLAYER_UNIQUE_ID_COL    3

// bools
#define IS_READY                true
#define NOT_READY               false
#define HAS_PASSWORD            true
#define NO_PASSWORD             false
#define IS_INFINITE             true
#define NOT_INFINITE            false
#define USING_3D_DICES          true
#define NOT_USING_3D_DICES      false

// True ranked settings source values
/// Ranked Settings Lobby System Info
#define RS_LS_UNIQUE_ID         ""
#define RS_LS_LOBBY_NAME        ""
#define RS_LS_LOBBY_PASSWORD    ""
#define RS_LS_MAX_PLAYERS_COUNT -1
#define RS_LS_OWNER_UNIQUE_ID   -1
#define RS_LS_IS_PRIVATE        false
/// Ranked Settings Game Settings Info
#define RS_GS_TURN_TIME         8
#define RS_GS_MAX_MONEY         2.000
#define RS_GS_IS_MAX_MONEY_INF  true
#define RS_GS_MAX_TURNS         72
#define RS_GS_ARE_MAX_TURNS_INF false

// Set by game rules
#define MIN_PLAYERS_COUNT 2

using std::vector;
using std::count_if;

// Lobbies short information inside MenuWindow's center table
struct LobbyShortInfo
{
    QString uniqueId;
    QString name;
    bool isPassworded;
    short curPlayersCount;
    short maxPlayersCount;
};

// Users short information inside LobbyWindow's table of users
struct UserShortInfo
{
    QString nickname;
    int rp;
    bool isReady;
    int uniqueId;
};

// All lobby settings
struct LobbySettings
{
    // System info
    QString uniqueId;
    QString lobbyName;
    QString lobbyPassword;
    short maxPlayersCount;
    int ownerUniqueId;
    bool isTimerActive;
    QString sessionAddress;
    int sessionPort;

    // Game settings
    short turnTime;
    float maxMoney; // represents billions
    bool isMaxMoneyInfinite;
    short maxTurns;
    bool areMaxTurnsInfinite;

    bool softEquals(LobbySettings& other)
    {
        if(this->lobbyName           != other.lobbyName          ||
           this->lobbyPassword       != other.lobbyPassword      ||
           this->maxPlayersCount     != other.maxPlayersCount    ||
           this->turnTime            != other.turnTime           ||
           this->maxMoney            != other.maxMoney           ||
           this->isMaxMoneyInfinite  != other.isMaxMoneyInfinite ||
           this->maxTurns            != other.maxTurns           ||
           this->areMaxTurnsInfinite != other.areMaxTurnsInfinite)
            return false;
        return true;
    }

    bool softNotEquals(LobbySettings& other)
    {
        return !softEquals(other);
    }

    bool operator==(LobbySettings& other)
    {
        if(this->uniqueId            != other.uniqueId           ||
           this->lobbyName           != other.lobbyName          ||
           this->lobbyPassword       != other.lobbyPassword      ||
           this->maxPlayersCount     != other.maxPlayersCount    ||
           this->ownerUniqueId       != other.ownerUniqueId      ||
           this->turnTime            != other.turnTime           ||
           this->maxMoney            != other.maxMoney           ||
           this->isMaxMoneyInfinite  != other.isMaxMoneyInfinite ||
           this->maxTurns            != other.maxTurns           ||
           this->areMaxTurnsInfinite != other.areMaxTurnsInfinite)
            return false;
        return true;
    }

    bool operator!=(LobbySettings& other)
    {
        return !(*this == other);
    }

    void softOverride(LobbySettings &other)
    {
        this->lobbyName           = other.lobbyName;
        this->lobbyPassword       = other.lobbyPassword;
        this->maxPlayersCount     = other.maxPlayersCount;
        this->turnTime            = other.turnTime;
        this->maxMoney            = other.maxMoney;
        this->isMaxMoneyInfinite  = other.isMaxMoneyInfinite;
        this->maxTurns            = other.maxTurns;
        this->areMaxTurnsInfinite = other.areMaxTurnsInfinite;
    }
};

// Full lobby info inside LobbyWindow
struct LobbyFullInfo
{
    LobbySettings settings;
    vector<UserShortInfo> usersInLobby;
};

// Host-user data
struct HostUserData
{
    int uniqueId;
    QString nickname;
    int rpCount;
};

#endif // SOURCESTRUCTS_H
