#include <QtTest>

#include "../helpers/common/filemanager.h"

class FileManagerTest : public QObject
{
    Q_OBJECT

public:
    FileManagerTest();
    ~FileManagerTest();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_initLocalDirectory();
    void test_saveGetLastSettingsToLocal();
    void test_applyGet3dDiceStateToLocal();
    void test_commitGetTokens();
    void test_commitGetToken();
    void test_commitGetHostUniqueId();
    void test_clearUserMeta();
};

FileManagerTest::FileManagerTest()
{

}

FileManagerTest::~FileManagerTest()
{

}

void FileManagerTest::initTestCase()
{

}

void FileManagerTest::cleanupTestCase()
{

}

void FileManagerTest::test_initLocalDirectory()
{
    FileManager::initLocalDirectory();

    if(!QFile::exists(ssLocalDirPath + ssUserMetaFileName)
            || !QFile::exists(ssLocalDirPath + ssLastSettingsFileName)
            || !QFile::exists(ssLocalDirPath + ssRankedSettingsFileName))
        QFAIL("One or more files do not exist");
}

void FileManagerTest::test_saveGetLastSettingsToLocal()
{
    LobbySettings testLS = {10, "testLobby", "testPassword", 5, 20,
                            false, "", 0, 15, 2000, true, 72, false, PRIVATE};

    FileManager::saveLastSettingsToLocal(testLS);

    LobbySettings getLS = FileManager::getLastSettingsFromLocal();

    if(testLS != getLS)
        QFAIL("Saved settings not equal");
}

void FileManagerTest::test_applyGet3dDiceStateToLocal()
{
    FileManager::apply3dDiceStateToLocal(true);

    uint8_t gotPref = FileManager::getUser3dDicePreference();

    if(gotPref != FileManager::DicePrefTrue)
        QFAIL("Wrong got dice preference");

    FileManager::apply3dDiceStateToLocal(false);

    gotPref = FileManager::getUser3dDicePreference();

    if(gotPref != FileManager::DicePrefFalse)
        QFAIL("Wrong got dice preference");
}

void FileManagerTest::test_commitGetTokens()
{
    FileManager::commitTokens("{\"tokens\":{\"accessToken\":\"testToken12345\","
                              "\"refreshToken\":\"testToken54321\"}}");

    QString accessToken = FileManager::getToken(AccessToken);

    if(accessToken != "testToken12345")
        QFAIL("Wrong access token parse");

    QString refreshToken = FileManager::getToken(RefreshToken);

    if(refreshToken != "testToken54321")
        QFAIL("Wrong refresh token parse");
}

void FileManagerTest::test_commitGetToken()
{
    FileManager::commitToken(AccessToken, "testAccessToken12345");

    QString accessToken = FileManager::getToken(AccessToken);

    if(accessToken != "testAccessToken12345")
        QFAIL("Wrong access token parse");

    FileManager::commitToken(RefreshToken, "testRefreshToken54321");

    QString refreshToken = FileManager::getToken(RefreshToken);

    if(refreshToken != "testRefreshToken54321")
        QFAIL("Wrong refresh token parse");
}

void FileManagerTest::test_commitGetHostUniqueId()
{
    FileManager::commitHostData(123);

    int hostUniqueId = FileManager::getHostUniqueId().toInt();

    if(hostUniqueId != 123)
        QFAIL("Wrong host unique id");

    FileManager::commitHostData(548128, "testNickname", 591923919, false);

    QString hostUniqueIdString = FileManager::getHostUniqueId();

    if(hostUniqueIdString != "548128")
        QFAIL("Wrong host unique id");
}

void FileManagerTest::test_clearUserMeta()
{
    FileManager::clearUserMetaForNewLogin();

    if(!FileManager::getToken(AccessToken).isEmpty())
        QFAIL("Access token is not empty");

    if(!FileManager::getToken(RefreshToken).isEmpty())
        QFAIL("Refresh token is not empty");

    if(FileManager::getHostUniqueId().toInt() != -1)
        QFAIL("Host unique id is not equal -1");
}

QTEST_APPLESS_MAIN(FileManagerTest)

#include "tst_filemanagertest.moc"
