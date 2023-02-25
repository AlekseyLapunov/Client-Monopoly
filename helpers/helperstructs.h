#ifndef HELPERSTRUCTS_H
#define HELPERSTRUCTS_H

#include <vector>

#include <QString>

using std::vector;

typedef short int quantityFlag;
typedef short int mapPatternFlag;

// Lobbies short information inside MenuWindow's center table
struct LobbyShortInfo
{
    QString lobbyName;
    bool isPassworded;
    short int curPlayersCount;
    short int maxPlayersCount;
};

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
