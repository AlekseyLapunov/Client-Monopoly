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
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][IsPrivate],       RS_LS_IS_PRIVATE);

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

LobbySettingsCombined loadSettingsFromFile(const QString &path)
{
    LobbySettingsCombined settingsOutput;

    QString jsonString = readJsonToQString(path);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    qDebug() << "JSON Parse Error: " << error.errorString() << error.offset << error.error;

    if(!doc.isObject())
        throw std::runtime_error(ssRuntimeErrors[JsonParseError]);

    QJsonObject jsonObj = doc.object();
    QJsonValue jsonLSI = jsonObj.value(ssJsonObjectsId[LobbySystemIter]);
    QJsonValue jsonGSI = jsonObj.value(ssJsonObjectsId[GameSettingsIter]);

    if(!jsonLSI.isObject() || !jsonGSI.isObject())
        throw std::runtime_error(ssRuntimeErrors[JsonParseError]);

    QJsonObject jsonObjLSI = jsonLSI.toObject();
    QJsonObject jsonObjGSI = jsonGSI.toObject();

    settingsOutput.lobbySystem.uniqueId        = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][UniqueId]].toInt();
    settingsOutput.lobbySystem.lobbyName       = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][LobbyName]].toString();
    settingsOutput.lobbySystem.lobbyPassword   = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][LobbyPassword]].toString();
    settingsOutput.lobbySystem.maxPlayersCount = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][MaxPlayersCount]].toInt();
    settingsOutput.lobbySystem.ownerUniqueId   = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][OwnerUniqueId]].toInt();
    settingsOutput.lobbySystem.isPrivate       = jsonObjLSI[ssJsonKeysLobbySettings[LobbySystemIter][IsPrivate]].toBool();

    settingsOutput.gameSettings.turnTime            = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][TurnTime]].toInt();
    settingsOutput.gameSettings.maxMoney            = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][MaxMoney]].toDouble();
    settingsOutput.gameSettings.isMaxMoneyInfinite  = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][IsMaxMoneyInfinite]].toBool();
    settingsOutput.gameSettings.maxTurns            = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][MaxTurns]].toInt();
    settingsOutput.gameSettings.areMaxTurnsInfinite = jsonObjGSI[ssJsonKeysLobbySettings[GameSettingsIter][AreMaxTurnsInfinite]].toBool();

    return {settingsOutput.lobbySystem, settingsOutput.gameSettings};
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

void writeFile(QString &path, const QString &content)
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

LobbySettingsCombined getRankedSettingsFromLocal()
{
    initLocalDirectory();
    LobbySettingsCombined settingsOutput = loadSettingsFromFile(ssLocalDirPath + ssRankedSettingsFileName);
    return { settingsOutput.lobbySystem, settingsOutput.gameSettings };
}

LobbySettingsCombined manageSettingsImport(bool &gotSettings)
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, ssCaptionImportSettings, QString(), ssJsonFilter);

    if(filePath.isEmpty())
    {
        gotSettings = false;
        return {};
    }

    LobbySettingsCombined settingsOutput = loadSettingsFromFile(filePath);

    gotSettings = true;
    return {settingsOutput.lobbySystem, settingsOutput.gameSettings};
}

void manageSettingsExport(LobbySettingsCombined settingsToExport)
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, ssCaptionExportSettings, QString(), ssJsonFilter);

    if(filePath.isEmpty())
        return;

    writeFile(filePath, toJsonQString(settingsToExport));
}

QString toJsonQString(LobbySettingsCombined settingsToConvert)
{
    QJsonObject mainObject;

    QJsonObject lobbySystemObject;
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][UniqueId],        settingsToConvert.lobbySystem.uniqueId);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][LobbyName],       settingsToConvert.lobbySystem.lobbyName);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][LobbyPassword],   settingsToConvert.lobbySystem.lobbyPassword);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][MaxPlayersCount], settingsToConvert.lobbySystem.maxPlayersCount);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][OwnerUniqueId],   settingsToConvert.lobbySystem.ownerUniqueId);
    lobbySystemObject.insert(ssJsonKeysLobbySettings[LobbySystemIter][IsPrivate],       settingsToConvert.lobbySystem.isPrivate);

    QJsonObject gameSettingsObject;
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][TurnTime],            settingsToConvert.gameSettings.turnTime);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][MaxMoney],            settingsToConvert.gameSettings.maxMoney);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][IsMaxMoneyInfinite],  settingsToConvert.gameSettings.isMaxMoneyInfinite);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][MaxTurns],            settingsToConvert.gameSettings.maxTurns);
    gameSettingsObject.insert(ssJsonKeysLobbySettings[GameSettingsIter][AreMaxTurnsInfinite], settingsToConvert.gameSettings.areMaxTurnsInfinite);

    mainObject.insert(ssJsonObjectsId[LobbySystemIter], lobbySystemObject);
    mainObject.insert(ssJsonObjectsId[GameSettingsIter], gameSettingsObject);

    QJsonDocument doc(mainObject);
    return doc.toJson(QJsonDocument::Indented);
}
