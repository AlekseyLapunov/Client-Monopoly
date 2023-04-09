#include "filemanager.h"

void createUserMetaJson(const QString &path)
{
    QFile userMeta(path + userMetaFileName);
    if(!userMeta.exists())
    {
        userMeta.open(QIODevice::WriteOnly);
        userMeta.close();
    }
}

void createRankedSettingsJson(const QString &path)
{
    QFile rankedSettingsJsonFile(path + rankedSettingsFileName);
    const QString trueRankedSettingsJson = makeTrueRankedJsonString();

    if(!rankedSettingsJsonFile.exists())
    {
        writeFile(rankedSettingsJsonFile, trueRankedSettingsJson);
    }
    else
    {
        QString rankedSettingsFromLocalDir = readJsonToQString(path + rankedSettingsFileName);
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
    lobbySystemObject.insert("uniqueId",        "-1");
    lobbySystemObject.insert("lobbyName",       "");
    lobbySystemObject.insert("lobbyPassword",   "");
    lobbySystemObject.insert("maxPlayersCount", "-1");
    lobbySystemObject.insert("ownerUniqueId",   "-1");
    lobbySystemObject.insert("isPrivate",       "false");

    QJsonObject gameSettingsObject;
    gameSettingsObject.insert("turnTime",               "8");
    gameSettingsObject.insert("moneyRightBorder",       "2.000");
    gameSettingsObject.insert("isMaxMoneyInfinite",     "true");
    gameSettingsObject.insert("turnsRightBorder",       "72");
    gameSettingsObject.insert("areMaxTurnsInfinite",    "false");

    mainObject.insert("LobbySystemInfo", lobbySystemObject);
    mainObject.insert("GameSettingsInfo", gameSettingsObject);

    QJsonDocument doc(mainObject);
    return doc.toJson(QJsonDocument::Indented);
}

void initLocalDirectory()
{
    QDir dir;
    try
    {
        if(!dir.exists(localDirPath))
            dir.mkpath(localDirPath);

        createUserMetaJson(localDirPath);
        createRankedSettingsJson(localDirPath);
    }
    catch (std::exception &e)
    {
        qDebug() << e.what() << Qt::endl;
        return;
    }
}

LobbySettingsCombined loadSettingsFromFile()
{
    LobbySettingsCombined settingsOutput;

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

QString writeFile(QFile &file, const QString &content)
{
    try
    {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream( &file );
        stream << content;
        file.close();
    }
    catch (std::exception &e)
    {
        qDebug() << e.what() << Qt::endl;
    }
    return "";
}
