#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdexcept>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QTextStream>

#include "sourcestructs.h"
#include "sourcestrings.h"

void createUserMetaJson(const QString &path);
void createRankedSettingsJson(const QString &path);
void initLocalDirectory();
QString makeTrueRankedJsonString();
QString readJsonToQString(const QString &path);
QString writeFile(QFile &file, const QString &content);
LobbySettingsCombined loadSettingsFromFile();

#endif // FILEMANAGER_H
