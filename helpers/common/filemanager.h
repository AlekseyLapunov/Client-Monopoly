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
    static void fillUserMetaJson(bool uses3dDice);
    static void manageSettingsExport(LobbySettings settingsToExport, QWidget *parent = nullptr);
    static LobbySettings manageSettingsImport(bool &gotSettings, QWidget *parent = nullptr);
    static LobbySettings getRankedSettingsFromLocal();
    static LobbySettings getLastSettingsFromLocal();
    static bool get3dDicePrefFromLocal();
    static bool isLastSettingsFileExists();

private:
    static void createUserMetaJson(const QString &path);
    static void createRankedSettingsJson(const QString &path);
    static void writeFile(const QString &path, const QString &content);
    static void writeFile(QFile &file, const QString &content);
    static QString readJsonToQString(const QString &path);
    static LobbySettings loadSettingsFromFile(const QString &path);
};

#endif // FILEMANAGER_H
