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
void createRankedSettingsJson(const QString &path);
void initLocalDirectory();

QString makeTrueRankedJsonString();
QString readJsonToQString(const QString &path);
QString toJsonQString(LobbySettingsCombined settingsToConvert);

void writeFile(QString &path, const QString &content);
void writeFile(QFile &file, const QString &content);

LobbySettingsCombined getRankedSettingsFromLocal();
LobbySettingsCombined loadSettingsFromFile(const QString &path);

// Import / Export
LobbySettingsCombined manageSettingsImport(bool &gotSettings);
void manageSettingsExport(LobbySettingsCombined settingsToExport);

#endif // FILEMANAGER_H
