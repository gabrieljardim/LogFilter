#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>

namespace FileHandler {
void saveOpenedFilePath(QString filePath);

QString getLastLogFile();

QStringList getFileContent(QString filePath);
}; // namespace FileHandler

#endif // FILEHANDLER_H
