#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "highlightdata.h"

#include <QList>
#include <QString>

namespace FileHandler {
void saveOpenedFilePath(QString filePath);

QString getLastLogFile();

QStringList getFileContent(QString filePath);

void saveHighlightList(QList<HighlightData> highList);

QList<HighlightData> getHighlightList();

}; // namespace FileHandler

#endif // FILEHANDLER_H
