#ifndef TST_SOURCESTRUCTS_H
#define TST_SOURCESTRUCTS_H

#include <QtTest>

#include "../helpers/common/sourcestructs.h"

class SourceStructsTest : public QObject
{
    Q_OBJECT

public:
    SourceStructsTest()
    {

    }

    ~SourceStructsTest()
    {

    }

private slots:
    void test_defineLobbyTypeForServer()
    {
        LobbySettings temp = {0, "test", "", 6, 1, false, "", 0, 10, 2000, false, 72, true, 0};

        if(temp.defineLobbyTypeForServer() != PUBLIC)
            QFAIL("Lobby with empty password should be public");

        temp.lobbyPassword = "1";

        if(temp.defineLobbyTypeForServer() != PRIVATE)
            QFAIL("Lobby with not empty password should be private");

        temp.lobbyPassword = "         ";

        if(temp.defineLobbyTypeForServer() != PRIVATE)
            QFAIL("Lobby with not empty password should be private");

        temp.type = RANKED;

        if(temp.defineLobbyTypeForServer() != RANKED)
            QFAIL("Lobby with type set ranked should remain ranked");

        temp.lobbyPassword = "";

        if(temp.defineLobbyTypeForServer() != RANKED)
            QFAIL("Lobby with type set ranked should remain ranked");

        temp.type = 1;

        if(temp.defineLobbyTypeForServer() != PUBLIC)
            QFAIL("Lobby with empty password should be public");
    }

    void test_defineVictoryTypeForServer()
    {
        LobbySettings temp = {0, "test", "", 6, 1, false, "", 0, 10, 2000, false, 72, true, 0};

        if(temp.defineVictoryTypeForServer() != SCORE)
            QFAIL("Lobby with money is not infinite should have SCORE victory type");

        temp.isMaxMoneyInfinite = true;

        if(temp.defineVictoryTypeForServer() != SCORE)
            QFAIL("Lobby with both money and turn are infinite should have SCORE victory type");

        temp.areMaxTurnsInfinite = false;

        if(temp.defineVictoryTypeForServer() != TURN)
            QFAIL("Lobby with turns are not infinite should have TURN victory type");

        temp.isMaxMoneyInfinite  = false;
        temp.areMaxTurnsInfinite = false;

        if(temp.defineVictoryTypeForServer() != BOTH)
            QFAIL("Lobby with both money and turn are not infinite should have BOTH victory type");
    }

    void test_setupByVictoryType()
    {
        LobbySettings temp = {0, "test", "", 6, 1, false, "", 0, 10, 2000, false, 72, true, 0};

        temp.setupByVictoryType(BOTH);

        if(temp.areMaxTurnsInfinite || temp.isMaxMoneyInfinite)
            QFAIL("Turns and money should not be infinite while victory type is set as BOTH");

        temp.setupByVictoryType(TURN);

        if(temp.areMaxTurnsInfinite || !temp.isMaxMoneyInfinite)
            QFAIL("Turns should not be infinite and money should be infinite while victory type is set as TURN");

        temp.setupByVictoryType(SCORE);

        if(!temp.areMaxTurnsInfinite || temp.isMaxMoneyInfinite)
            QFAIL("Turns should be infinite and money should not be infinite while victory type is set as SCORE");
    }

    void test_lsEqualityOperators()
    {
        LobbySettings temp = {0, "test", "", 6, 1, false, "", 0, 10, 2000, false, 72, true, 0};

        LobbySettings temp2 = {0, "test", "", 6, 1, false, "", 0, 10, 2000, false, 72, true, 0};

        QVERIFY(temp == temp2);

        if(temp != temp2)
            QFAIL("temp (LS) should be equal to temp2 (LS)");

        temp.lobbyName = "temp";

        QVERIFY(temp != temp2);

        QVERIFY(temp.softNotEquals(temp2));

        temp.lobbyName = temp2.lobbyName;
        temp.lobbyPassword = "123123";
        temp.type = temp.defineLobbyTypeForServer();

        temp2.softOverride(temp);
        temp2.uniqueId = -123;

        QVERIFY(temp.softEquals(temp2));
        QVERIFY(temp2.softEquals(temp));
    }
};

#endif
