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

typedef short int quantityFlag;
typedef short int mapPatternFlag;

static const QString lobbyRegExpString = "^[a-zA-Zа-яА-ЯёЁ0-9]+ ?[a-zA-Zа-яА-ЯёЁ0-9]*$";
static const QString uniqueIdRegExpString = "^[0-9]*$";

// Lobbies short information inside MenuWindow's center table
struct LobbyShortInfo
{
    int uniqueId;
    QString lobbyName;
    bool isPassworded;
    short int curPlayersCount;
    short int maxPlayersCount;
};

static const QStringList lobbyTableLabels {"Название",
                                           "Пароль",
                                           "Игроки"};

// Detailed lobby system info
struct LobbySystemInfo : public LobbyShortInfo
{
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
    enum fieldsQuantity { none = -1, random, few, mid, many };
    enum mapPattern { crossroads, angles };

    quantityFlag sabotageDistrib;
    quantityFlag vacationDistrib;

    mapPatternFlag mapPattern;

    long int moneyRightBorder;
    short int turnsRightBorder;
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
