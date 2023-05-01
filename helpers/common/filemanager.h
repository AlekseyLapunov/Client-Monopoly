#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdexcept>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QString>
#include <QTextStream>

#include "sourcestructs.h"
#include "sourcestrings.h"

class FileManager
{
public:
    static void initLocalDirectory();
    static void saveLastSettingsToLocal(LobbySettings settingsToSave);
    static void apply3dDiceStateToLocal(bool uses3dDice);
    static void manageSettingsExport(LobbySettings settingsToExport, QWidget *parent = nullptr);
    static LobbySettings manageSettingsImport(bool &gotSettings, QWidget *parent = nullptr);
    static LobbySettings getRankedSettingsFromLocal();
    static LobbySettings getLastSettingsFromLocal();
    static QString getUserMetaFromLocal(short dataUnitFlag);
    static void checkLastSettingsIntegrity();
    static QString getPageAsCallbackText();
    static void commitTokens(QByteArray data);
    static void commitToken(uint8_t tokenType, QString token);
    static QString getToken(uint8_t tokenType);
    static void commitHostData(int uniqueId,
                               QString nickname,
                               int rpCount,
                               bool isGuest);
    static void commitHostData(int hostUserId);
    static QString getHostUniqueId();

private:
    static void createUserMetaJson(const QString &dir);
    static void createRankedSettingsJson(const QString &dir);
    static void createLastSettingsJson(const QString &dir);
    static void writeFile(const QString &path, const QString &content);
    static void writeFile(QFile &file, const QString &content);
    static bool isJsonFileIntegral(const QString &path);
    static bool isJsonIntegral(const QString &jsonString, const QString &name = "");
    static QString readJsonToQString(const QString &path);
    static LobbySettings loadSettingsFromFile(const QString &path);
    static QString defaultUserJson();
};

#endif // FILEMANAGER_H
