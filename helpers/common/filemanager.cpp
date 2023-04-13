#include "filemanager.h"

void createUserMetaJson(const QString &path)
{
    QFile userMeta(path + ssUserMetaFileName);
    if(!userMeta.exists())
    {
        userMeta.open(QIODevice::WriteOnly);
        userMeta.close();
    }
}

void fillUserMetaJson(bool uses3dDice)
{
    initLocalDirectory();
    QFile userMeta(ssLocalDirPath + ssUserMetaFileName);

    QJsonObject mainObject;
    mainObject.insert(ssJsonUserMeta[Uses3dDice], uses3dDice);

    QJsonDocument doc(mainObject);
    writeFile(userMeta, doc.toJson(QJsonDocument::Indented));
}

bool get3dDicePrefFromLocal()
{
    initLocalDirectory();

    LobbySettings settingsOutput;

    QString jsonString = readJsonToQString(ssLocalDirPath + ssUserMetaFileName);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    qDebug() << "JSON Parse:" << error.errorString() << error.offset << error.error;

    if(!doc.isObject())
        return true;

    QJsonObject jsonObj = doc.object();

    return jsonObj[ssJsonUserMeta[Uses3dDice]].toBool();
}

void createRankedSettingsJson(const QString &path)
{
    QFile rankedSettingsJsonFile(path + ssRankedSettingsFileName);
    const QString trueRankedSettingsJson = makeTrueRankedJsonString();

    if(!rankedSettingsJsonFile.exists())
    {
        writeFile(rankedSettingsJsonFile, trueRankedSettingsJson);
    }
    else
    {
        QString rankedSettingsFromLocalDir = readJsonToQString(path + ssRankedSettingsFileName);
        if(rankedSettingsFromLocalDir != trueRankedSettingsJson)
        {
            writeFile(rankedSettingsJsonFile, trueRankedSettingsJson);
        }
    }
}

QString makeTrueRankedJsonString()
{
    QJsonObject settingsObject;
    settingsObject.insert(ssJsonKeysLobbySettingsIds[UniqueId],            RS_UNIQUE_ID);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[LobbyName],           RS_LOBBY_NAME);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[LobbyPassword],       RS_LOBBY_PASSWORD);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[MaxPlayersCount],     RS_MAX_PLAYERS_COUNT);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[OwnerUniqueId],       RS_OWNER_UNIQUE_ID);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[IsTimerActive],       RS_IS_TIMER_ACTIVE);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[SessionAddress],      RS_SESSION_ADDRESS);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[SessionPort],         RS_SESSION_PORT);

    settingsObject.insert(ssJsonKeysLobbySettingsIds[TurnTime],            RS_TURN_TIME);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[MaxMoney],            RS_MAX_MONEY);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[IsMaxMoneyInfinite],  RS_IS_MAX_MONEY_INF);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[MaxTurns],            RS_MAX_TURNS);
    settingsObject.insert(ssJsonKeysLobbySettingsIds[AreMaxTurnsInfinite], RS_ARE_MAX_TURNS_INF);

    QJsonDocument doc(settingsObject);
    return doc.toJson(QJsonDocument::Indented);
}

void initLocalDirectory()
{
    QDir dir;
    try
    {
        if(!dir.exists(ssLocalDirPath))
            dir.mkpath(ssLocalDirPath);

        createUserMetaJson(ssLocalDirPath);
        createRankedSettingsJson(ssLocalDirPath);
    }
    catch (std::exception &e)
    {
        qDebug() << e.what() << Qt::endl;
        return;
    }
}

LobbySettings loadSettingsFromFile(const QString &path)
{
    LobbySettings settingsOutput;

    QString jsonString = readJsonToQString(path);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    qDebug() << "JSON Parse:" << error.errorString() << error.offset << error.error;

    if(!doc.isObject())
        throw std::runtime_error(ssRuntimeErrors[JsonParseError]);

    QJsonObject settingsObj = doc.object();

    settingsOutput.uniqueId            = settingsObj[ssJsonKeysLobbySettingsIds[UniqueId]].toString();
    settingsOutput.lobbyName           = settingsObj[ssJsonKeysLobbySettingsIds[LobbyName]].toString();
    settingsOutput.lobbyPassword       = settingsObj[ssJsonKeysLobbySettingsIds[LobbyPassword]].toString();
    settingsOutput.maxPlayersCount     = settingsObj[ssJsonKeysLobbySettingsIds[MaxPlayersCount]].toInt();
    settingsOutput.ownerUniqueId       = settingsObj[ssJsonKeysLobbySettingsIds[OwnerUniqueId]].toInt();
    settingsOutput.isTimerActive       = settingsObj[ssJsonKeysLobbySettingsIds[IsTimerActive]].toBool();
    settingsOutput.sessionAddress      = settingsObj[ssJsonKeysLobbySettingsIds[SessionAddress]].toString();
    settingsOutput.sessionPort         = settingsObj[ssJsonKeysLobbySettingsIds[SessionPort]].toInt();

    settingsOutput.turnTime            = settingsObj[ssJsonKeysLobbySettingsIds[TurnTime]].toInt();
    settingsOutput.maxMoney            = settingsObj[ssJsonKeysLobbySettingsIds[MaxMoney]].toDouble();
    settingsOutput.isMaxMoneyInfinite  = settingsObj[ssJsonKeysLobbySettingsIds[IsMaxMoneyInfinite]].toBool();
    settingsOutput.maxTurns            = settingsObj[ssJsonKeysLobbySettingsIds[MaxTurns]].toInt();
    settingsOutput.areMaxTurnsInfinite = settingsObj[ssJsonKeysLobbySettingsIds[AreMaxTurnsInfinite]].toBool();

    return settingsOutput;
}

QString readJsonToQString(const QString &path)
{
    QString readJson;

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    readJson = file.readAll();
    file.close();

    return readJson;
}

void writeFile(const QString &path, const QString &content)
{
    try
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&file);
        stream << content;
        file.close();
    }
    catch (std::exception &e)
    {
        qDebug() << e.what() << Qt::endl;
    }
}

void writeFile(QFile &file, const QString &content)
{
    try
    {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&file);
        stream << content;
        file.close();
    }
    catch (std::exception &e)
    {
        qDebug() << e.what() << Qt::endl;
    }
}

void saveLastSettingsToLocal(LobbySettings settingsToSave)
{
    const QString content = settingsToSave.toJsonQString();
    writeFile(ssLocalDirPath + ssLastSettingsFileName, content);
}

LobbySettings getRankedSettingsFromLocal()
{
    initLocalDirectory();
    LobbySettings settingsOutput = loadSettingsFromFile(ssLocalDirPath + ssRankedSettingsFileName);
    return settingsOutput;
}

bool isLastSettingsFileExists()
{
    QFile lastSettingsFile(ssLocalDirPath + ssLastSettingsFileName);
    return lastSettingsFile.exists();
}

LobbySettings getLastSettingsFromLocal()
{
    initLocalDirectory();

    if(!isLastSettingsFileExists())
        return {};

    LobbySettings settingsOutput = loadSettingsFromFile(ssLocalDirPath + ssLastSettingsFileName);
    return settingsOutput;
}

LobbySettings manageSettingsImport(bool &gotSettings, QWidget *parent)
{
    QString filePath = QFileDialog::getOpenFileName(parent, ssCaptionImportSettings, QString(), ssJsonFilter);

    if(filePath.isEmpty())
    {
        gotSettings = false;
        return {};
    }

    LobbySettings settingsOutput = loadSettingsFromFile(filePath);

    gotSettings = true;
    return settingsOutput;
}

void manageSettingsExport(LobbySettings settingsToExport, QWidget *parent)
{
    QString filePath = QFileDialog::getSaveFileName(parent, ssCaptionExportSettings, QString(), ssJsonFilter);

    if(filePath.isEmpty())
        return;

    writeFile(filePath, settingsToExport.toJsonQString());
}
