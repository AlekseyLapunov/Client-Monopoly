#include "filemanager.h"

void FileManager::fillUserMetaJson(bool uses3dDice)
{
    initLocalDirectory();
    QFile userMeta(ssLocalDirPath + ssUserMetaFileName);

    QJsonObject mainObject;
    mainObject.insert(ssJsonUserMeta[Uses3dDice], uses3dDice);

    QJsonDocument doc(mainObject);
    writeFile(userMeta, doc.toJson(QJsonDocument::Indented));
}

bool FileManager::get3dDicePrefFromLocal()
{
    initLocalDirectory();

    LobbySettings settingsOutput;

    QString jsonString = readJsonToQString(ssLocalDirPath + ssUserMetaFileName);

    if(isJsonIntegral(jsonString, ssUserMetaFileName))
        return true;

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(!doc.isObject())
        return true;

    QJsonObject jsonObj = doc.object();

    return jsonObj[ssJsonUserMeta[Uses3dDice]].toBool();
}

void FileManager::createUserMetaJson(const QString &dir)
{
    QFile userMeta(dir + ssUserMetaFileName);
    if(!userMeta.exists())
    {
        userMeta.open(QIODevice::WriteOnly);
        userMeta.close();
    }
}

void FileManager::createRankedSettingsJson(const QString &dir)
{
    QFile rankedSettingsJsonFile(dir + ssRankedSettingsFileName);

    LobbySettings rankedSettingsCopy = TrueRankedSettings;
    const QString trueRankedSettingsJson = rankedSettingsCopy.toJsonQString();

    if(!rankedSettingsJsonFile.exists())
    {
        writeFile(rankedSettingsJsonFile, trueRankedSettingsJson);
    }
    else
    {
        QString rankedSettingsFromLocalDir = readJsonToQString(dir + ssRankedSettingsFileName);
        if(rankedSettingsFromLocalDir != trueRankedSettingsJson)
            writeFile(rankedSettingsJsonFile, trueRankedSettingsJson);
    }
}

void FileManager::createLastSettingsJson(const QString &dir)
{
    QFile lastSettingsJsonFile(dir + ssLastSettingsFileName);

    if(!lastSettingsJsonFile.exists())
    {
        LobbySettings defaultSettingsCopy = DefaultLobbySettings;
        writeFile(lastSettingsJsonFile, defaultSettingsCopy.toJsonQString());
    }
    else
    {
        isJsonFileIntegral(dir + ssLastSettingsFileName);
    }
}

void FileManager::initLocalDirectory()
{
    QDir dir;
    try
    {
        if(!dir.exists(ssLocalDirPath))
            dir.mkpath(ssLocalDirPath);

        createUserMetaJson(ssLocalDirPath);
        createRankedSettingsJson(ssLocalDirPath);
        createLastSettingsJson(ssLocalDirPath);
    }
    catch (std::exception &e)
    {
        qDebug() << e.what() << Qt::endl;
        return;
    }
}

LobbySettings FileManager::loadSettingsFromFile(const QString &path)
{
    LobbySettings settingsOutput;

    QString jsonString = readJsonToQString(path);

    if(isJsonIntegral(jsonString, path))
        throw std::runtime_error(ssRuntimeErrors[JsonParseError]);

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());

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

QString FileManager::readJsonToQString(const QString &path)
{
    QString readJson;

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    readJson = file.readAll();
    file.close();

    return readJson;
}

void FileManager::writeFile(const QString &path, const QString &content)
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

void FileManager::writeFile(QFile &file, const QString &content)
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

bool FileManager::isJsonFileIntegral(const QString &path)
{
    QString jsonString = readJsonToQString(path);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    qDebug() << "JSON Integrity" << path << error.errorString() << error.offset << error.error;

    return (error.offset == 0 && error.error == 0 && doc.isObject());
}

bool FileManager::isJsonIntegral(const QString &jsonString, const QString &name)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    qDebug() << "JSON Integrity" << (name.isEmpty() ? "" : name)
             << error.errorString() << error.offset << error.error;

    return (error.offset == 0 && error.error == 0 && doc.isObject());
}

void FileManager::saveLastSettingsToLocal(LobbySettings settingsToSave)
{
    const QString content = settingsToSave.toJsonQString();
    writeFile(ssLocalDirPath + ssLastSettingsFileName, content);
}

LobbySettings FileManager::getRankedSettingsFromLocal()
{
    initLocalDirectory();
    LobbySettings settingsOutput = loadSettingsFromFile(ssLocalDirPath + ssRankedSettingsFileName);
    return settingsOutput;
}

bool FileManager::isLastSettingsFileExists()
{
    QFile lastSettingsFile(ssLocalDirPath + ssLastSettingsFileName);
    return lastSettingsFile.exists();
}

void FileManager::checkLastSettingsIntegrity()
{

}

LobbySettings FileManager::getLastSettingsFromLocal()
{
    initLocalDirectory();

    if(!isLastSettingsFileExists())
        return {};

    LobbySettings settingsOutput = loadSettingsFromFile(ssLocalDirPath + ssLastSettingsFileName);
    return settingsOutput;
}

LobbySettings FileManager::manageSettingsImport(bool &gotSettings, QWidget *parent)
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

void FileManager::manageSettingsExport(LobbySettings settingsToExport, QWidget *parent)
{
    QString filePath = QFileDialog::getSaveFileName(parent, ssCaptionExportSettings, QString(), ssJsonFilter);

    if(filePath.isEmpty())
        return;

    writeFile(filePath, settingsToExport.toJsonQString());
}
