#ifndef SOURCESTRUCTS_H
#define SOURCESTRUCTS_H

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
#define NICKNAME_MAX_LEN        20
#define LOBBY_PASSWORD_MAX_LEN  10
#define UNIQUE_ID_MAX_LEN       6

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
#define RS_UNIQUE_ID         ""
#define RS_LOBBY_NAME        ""
#define RS_LOBBY_PASSWORD    ""
#define RS_MAX_PLAYERS_COUNT -1
#define RS_OWNER_UNIQUE_ID   -1
#define RS_IS_TIMER_ACTIVE   false
#define RS_SESSION_ADDRESS   ""
#define RS_SESSION_PORT      0
#define RS_TURN_TIME         8
#define RS_MAX_MONEY         2.000
#define RS_IS_MAX_MONEY_INF  true
#define RS_MAX_TURNS         72
#define RS_ARE_MAX_TURNS_INF false

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
    QString uniqueId;
    QString lobbyName;
    QString lobbyPassword;
    short maxPlayersCount;
    int ownerUniqueId;
    bool isTimerActive;
    QString sessionAddress;
    int sessionPort;

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

    QString toJsonQString()
    {
        QJsonObject settingsObject;
        settingsObject.insert(ssJsonKeysLobbySettingsIds[UniqueId],            this->uniqueId);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[LobbyName],           this->lobbyName);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[LobbyPassword],       this->lobbyPassword);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[MaxPlayersCount],     this->maxPlayersCount);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[OwnerUniqueId],       this->ownerUniqueId);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[IsTimerActive],       this->isTimerActive);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[SessionAddress],      this->sessionAddress);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[SessionPort],         this->sessionPort);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[TurnTime],            this->turnTime);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[MaxMoney],            this->maxMoney);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[IsMaxMoneyInfinite],  this->isMaxMoneyInfinite);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[MaxTurns],            this->maxTurns);
        settingsObject.insert(ssJsonKeysLobbySettingsIds[AreMaxTurnsInfinite], this->areMaxTurnsInfinite);
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
    RS_ARE_MAX_TURNS_INF
};

#endif // SOURCESTRUCTS_H
