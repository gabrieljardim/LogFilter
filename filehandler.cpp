#include "filehandler.h"

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>

const QString fileName = "data.dat";

void FileHandler::saveOpenedFilePath(QString filePath) {

  qDebug() << "Saving path " << filePath;

  QFile file(fileName);
  file.open(QIODevice::ReadWrite | QIODevice::Text);

  if (file.size() > 0) {
    file.resize(0);
  }

  QTextStream stream(&file);

  if (stream.readAll().contains(filePath) == false) {
    stream << filePath;
  }

  file.close();
}

QString FileHandler::getLastLogFile() {

  QFile file(fileName);
  file.open(QIODevice::ReadWrite | QIODevice::Text);

  if (file.size() <= 0) {
    qDebug() << "There is no last file log path saved.";
    return QString();
  }

  QTextStream stream(&file);
  QString lastLogPath = stream.readLine();

  file.close();

  return lastLogPath;
}

QStringList FileHandler::getFileContent(QString filePath) {

  QFile file(filePath);

  if (file.open(QIODevice::ReadOnly) == false) {
    return QStringList();
  }

  QTextStream stream(&file);
  QStringList list;

  while (stream.atEnd() == false) {
    list << stream.readLine();
  }

  file.close();

  return list;
}
