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

void createUserMetaJson(const QString &path);
void fillUserMetaJson(bool uses3dDice);
void createRankedSettingsJson(const QString &path);
void initLocalDirectory();

bool get3dDicePrefFromLocal();

QString makeTrueRankedJsonString();
QString readJsonToQString(const QString &path);
QString toJsonQString(LobbySettingsCombined settingsToConvert);

void writeFile(const QString &path, const QString &content);
void writeFile(QFile &file, const QString &content);

void saveLastSettingsToLocal(LobbySettingsCombined settingsToSave);

LobbySettingsCombined getRankedSettingsFromLocal();
bool isLastSettingsFileExists();
LobbySettingsCombined getLastSettingsFromLocal();
LobbySettingsCombined loadSettingsFromFile(const QString &path);

// Import / Export
LobbySettingsCombined manageSettingsImport(bool &gotSettings, QWidget *parent = nullptr);
void manageSettingsExport(LobbySettingsCombined settingsToExport, QWidget *parent = nullptr);

#endif // FILEMANAGER_H
