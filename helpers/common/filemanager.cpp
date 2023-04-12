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
    QJsonObject mainObject;

    QJsonObject lobbySystemObject;
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][UniqueId],        RS_LS_UNIQUE_ID);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][LobbyName],       RS_LS_LOBBY_NAME);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][LobbyPassword],   RS_LS_LOBBY_PASSWORD);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][MaxPlayersCount], RS_LS_MAX_PLAYERS_COUNT);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][OwnerUniqueId],   RS_LS_OWNER_UNIQUE_ID);

    QJsonObject gameSettingsObject;
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][TurnTime],            RS_GS_TURN_TIME);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][MaxMoney],            RS_GS_MAX_MONEY);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][IsMaxMoneyInfinite],  RS_GS_IS_MAX_MONEY_INF);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][MaxTurns],            RS_GS_MAX_TURNS);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][AreMaxTurnsInfinite], RS_GS_ARE_MAX_TURNS_INF);

    mainObject.insert(ssJsonObjectsId[LobbySystemIter], lobbySystemObject);
    mainObject.insert(ssJsonObjectsId[GameSettingsIter], gameSettingsObject);

    QJsonDocument doc(mainObject);
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

    QJsonObject jsonObj = doc.object();
    QJsonValue jsonLSI = jsonObj.value(ssJsonObjectsId[LobbySystemIter]);
    QJsonValue jsonGSI = jsonObj.value(ssJsonObjectsId[GameSettingsIter]);

    if(!jsonLSI.isObject() || !jsonGSI.isObject())
        throw std::runtime_error(ssRuntimeErrors[JsonParseError]);

    QJsonObject jsonObjLSI = jsonLSI.toObject();
    QJsonObject jsonObjGSI = jsonGSI.toObject();

    settingsOutput.uniqueId        = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][UniqueId]].toString();
    settingsOutput.lobbyName       = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][LobbyName]].toString();
    settingsOutput.lobbyPassword   = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][LobbyPassword]].toString();
    settingsOutput.maxPlayersCount = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][MaxPlayersCount]].toInt();
    settingsOutput.ownerUniqueId   = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][OwnerUniqueId]].toInt();

    settingsOutput.turnTime            = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][TurnTime]].toInt();
    settingsOutput.maxMoney            = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][MaxMoney]].toDouble();
    settingsOutput.isMaxMoneyInfinite  = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][IsMaxMoneyInfinite]].toBool();
    settingsOutput.maxTurns            = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][MaxTurns]].toInt();
    settingsOutput.areMaxTurnsInfinite = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][AreMaxTurnsInfinite]].toBool();

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
    const QString content = toJsonQString(settingsToSave);
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

    writeFile(filePath, toJsonQString(settingsToExport));
}

QString toJsonQString(LobbySettings settingsToConvert)
{
    QJsonObject mainObject;

    QJsonObject lobbySystemObject;
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][UniqueId],        settingsToConvert.uniqueId);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][LobbyName],       settingsToConvert.lobbyName);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][LobbyPassword],   settingsToConvert.lobbyPassword);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][MaxPlayersCount], settingsToConvert.maxPlayersCount);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][OwnerUniqueId],   settingsToConvert.ownerUniqueId);

    QJsonObject gameSettingsObject;
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][TurnTime],            settingsToConvert.turnTime);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][MaxMoney],            settingsToConvert.maxMoney);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][IsMaxMoneyInfinite],  settingsToConvert.isMaxMoneyInfinite);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][MaxTurns],            settingsToConvert.maxTurns);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][AreMaxTurnsInfinite], settingsToConvert.areMaxTurnsInfinite);

    mainObject.insert(ssJsonObjectsId[LobbySystemIter], lobbySystemObject);
    mainObject.insert(ssJsonObjectsId[GameSettingsIter], gameSettingsObject);

    QJsonDocument doc(mainObject);
    return doc.toJson(QJsonDocument::Indented);
}
