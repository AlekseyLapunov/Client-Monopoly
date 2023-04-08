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
#define USING_3D_DICES          true
#define NOT_USING_3D_DICES      false

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
    QString lobbyPassword;
    short maxPlayersCount;
    int ownerUniqueId;
    bool isPrivate;

    bool operator==(LobbySystemInfo& other)
    {
        if(this->uniqueId != other.uniqueId ||
           this->lobbyName != other.lobbyName ||
           this->lobbyPassword != other.lobbyPassword ||
           this->maxPlayersCount != other.maxPlayersCount ||
           this->ownerUniqueId != other.ownerUniqueId ||
           this->isPrivate != other.isPrivate)
            return false;
        return true;
    }

    bool operator!=(LobbySystemInfo& other)
    {
        return !(*this == other);
    }
};

// Users short information inside LobbyWindow's table of users
struct UserShortInfo
{
    QString nickname;
    int rpCount;
    bool isReady;
    int uniqueId;
};

// Game settings information inside LobbyWindow's section
struct GameSettingsInfo
{
    short turnTime;
    float moneyRightBorder; // represents billions
    bool isMaxMoneyInfinite;
    short turnsRightBorder;
    bool areMaxTurnsInfinite;

    bool operator==(GameSettingsInfo& other)
    {
        if(this->turnTime != other.turnTime ||
           this->moneyRightBorder != other.moneyRightBorder ||
           this->isMaxMoneyInfinite != other.isMaxMoneyInfinite ||
           this->turnsRightBorder != other.turnsRightBorder ||
           this->areMaxTurnsInfinite != other.areMaxTurnsInfinite)
            return false;
        return true;
    }

    bool operator!=(GameSettingsInfo& other)
    {
        return !(*this == other);
    }
};

// Full lobby info inside LobbyWindow
struct LobbyFullInfo
{
    LobbySystemInfo lobbySystem;
    GameSettingsInfo gameSettings;
    vector<UserShortInfo> usersInTable;
};

// Settings in one place
struct LobbySettingsCombined
{
    LobbySystemInfo lobbySystem;
    GameSettingsInfo gameSettings;

    bool operator==(LobbySettingsCombined other)
    {
        if(this->lobbySystem != other.lobbySystem || this->gameSettings != other.gameSettings)
            return false;
        return true;
    }

    bool operator!=(LobbySettingsCombined& other)
    {
        return !(*this == other);
    }
};

// Host-user data
struct HostUserData
{
    int uniqueUserId;
    QString userName;
    int userRpCount;
};

#endif // SOURCESTRUCTS_H
