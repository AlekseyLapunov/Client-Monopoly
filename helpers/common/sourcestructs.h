#ifndef SOURCESTRUCTS_H
#define SOURCESTRUCTS_H

#include <stdint.h>
#include <vector>

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

#include "sourcestrings.h"

// MenuWindow: parameters of table with lobbies
#define LOBBIES_TABLE_COLS      4
#define LOBBY_NAME_COL          0
#define IS_PASSWORDED_COL       1
#define PLAYERS_COUNT_COL       2
#define UNIQUE_ID_COL           3

// String lengths
#define NICKNAME_MAX_LEN        16
#define LOBBY_NAME_MAX_LEN      16
#define LOBBY_PASSWORD_MAX_LEN  16

// LobbyWindow: parameters of table with users
#define USERS_TABLE_COLS        4
#define NICKNAME_COL            0
#define USER_RP_COL             1
#define READY_COL               2
#define PLAYER_UNIQUE_ID_COL    3

// Lobby's types
#define LOBBY_TYPE_PUBLIC  "PUBLIC"  // without password
#define LOBBY_TYPE_RPIVATE "PRIVATE" // with password
#define LOBBY_TYPE_RANKED  "RANKED"  // ranked

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
#define RS_UNIQUE_ID         -1
#define RS_LOBBY_NAME        ""
#define RS_LOBBY_PASSWORD    ""
#define RS_MAX_PLAYERS_COUNT -1
#define RS_OWNER_UNIQUE_ID   -1
#define RS_IS_TIMER_ACTIVE   false
#define RS_SESSION_ADDRESS   ""
#define RS_SESSION_PORT      0
#define RS_TURN_TIME         8
#define RS_MAX_MONEY         2000
#define RS_IS_MAX_MONEY_INF  true
#define RS_MAX_TURNS         72
#define RS_ARE_MAX_TURNS_INF false
#define RS_TYPE              LOBBY_TYPE_PUBLIC

// Default lobby settings source values
#define DS_UNIQUE_ID         -1
#define DS_LOBBY_NAME        ""
#define DS_LOBBY_PASSWORD    ""
#define DS_MAX_PLAYERS_COUNT 4
#define DS_OWNER_UNIQUE_ID   -1
#define DS_IS_TIMER_ACTIVE   false
#define DS_SESSION_ADDRESS   ""
#define DS_SESSION_PORT      0
#define DS_TURN_TIME         10
#define DS_MAX_MONEY         2000
#define DS_IS_MAX_MONEY_INF  false
#define DS_MAX_TURNS         72
#define DS_ARE_MAX_TURNS_INF false
#define DS_TYPE              LOBBY_TYPE_PUBLIC

// Set by game rules
#define MIN_PLAYERS_COUNT 2

using std::vector;
using std::count_if;

// Lobbies short information inside MenuWindow's center table
struct LobbyShortInfo
{
    int uniqueId;
    QString name;
    bool isPassworded;
    int8_t curPlayersCount;
    int8_t maxPlayersCount;
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
    int uniqueId;
    QString lobbyName;
    QString lobbyPassword;
    int8_t maxPlayersCount;
    int ownerUniqueId;
    bool isTimerActive;

    QString sessionAddress;
    int sessionPort;

    int8_t turnTime;
    int maxMoney; // represents millions e.g. maxMoney = 1000 millions
    bool isMaxMoneyInfinite;
    unsigned short maxTurns;
    bool areMaxTurnsInfinite;
    QString type;

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

    QString toJsonQString()
    {
        QJsonObject settingsObject;
        settingsObject.insert(ssJsonLobbySettings[UniqueId],            this->uniqueId);
        settingsObject.insert(ssJsonLobbySettings[LobbyName],           this->lobbyName);
        settingsObject.insert(ssJsonLobbySettings[LobbyPassword],       this->lobbyPassword);
        settingsObject.insert(ssJsonLobbySettings[MaxPlayersCount],     this->maxPlayersCount);
        settingsObject.insert(ssJsonLobbySettings[OwnerUniqueId],       this->ownerUniqueId);
        settingsObject.insert(ssJsonLobbySettings[IsTimerActive],       this->isTimerActive);
        settingsObject.insert(ssJsonLobbySettings[SessionAddress],      this->sessionAddress);
        settingsObject.insert(ssJsonLobbySettings[SessionPort],         this->sessionPort);
        settingsObject.insert(ssJsonLobbySettings[TurnTime],            this->turnTime);
        settingsObject.insert(ssJsonLobbySettings[MaxMoney],            this->maxMoney);
        settingsObject.insert(ssJsonLobbySettings[IsMaxMoneyInfinite],  this->isMaxMoneyInfinite);
        settingsObject.insert(ssJsonLobbySettings[MaxTurns],            this->maxTurns);
        settingsObject.insert(ssJsonLobbySettings[AreMaxTurnsInfinite], this->areMaxTurnsInfinite);
        settingsObject.insert(ssJsonLobbySettings[LobbyType],           this->type);
        QJsonDocument doc(settingsObject);
        return doc.toJson(QJsonDocument::Indented);
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
    bool isGuest;
};

static const LobbySettings TrueRankedSettings
{
    RS_UNIQUE_ID,
    RS_LOBBY_NAME,
    RS_LOBBY_PASSWORD,
    RS_MAX_PLAYERS_COUNT,
    RS_OWNER_UNIQUE_ID,
    RS_IS_TIMER_ACTIVE,
    RS_SESSION_ADDRESS,
    RS_SESSION_PORT,
    RS_TURN_TIME,
    RS_MAX_MONEY,
    RS_IS_MAX_MONEY_INF,
    RS_MAX_TURNS,
    RS_ARE_MAX_TURNS_INF,
    RS_TYPE
};

static const LobbySettings DefaultLobbySettings
{
    DS_UNIQUE_ID,
    DS_LOBBY_NAME,
    DS_LOBBY_PASSWORD,
    DS_MAX_PLAYERS_COUNT,
    DS_OWNER_UNIQUE_ID,
    DS_IS_TIMER_ACTIVE,
    DS_SESSION_ADDRESS,
    DS_SESSION_PORT,
    DS_TURN_TIME,
    DS_MAX_MONEY,
    DS_IS_MAX_MONEY_INF,
    DS_MAX_TURNS,
    DS_ARE_MAX_TURNS_INF,
    RS_TYPE
};

#endif // SOURCESTRUCTS_H
