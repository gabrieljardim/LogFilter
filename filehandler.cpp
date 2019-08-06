#include "filehandler.h"

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>

#include "highlightdata.h"

const QString lastFileName = "data.dat";
const QString highlightFileName = "highlight.dat";

void FileHandler::saveOpenedFilePath(QString filePath) {

  qDebug() << "Saving path " << filePath;

  QFile file(lastFileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  file.resize(0);

  QTextStream stream(&file);
  stream << filePath;

  file.close();
}

QString FileHandler::getLastLogFile() {

  QFile file(lastFileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  if (file.size() <= 0) {
    qDebug() << "There is no last file log path saved.";
    return QString();
  }

  QTextStream stream(&file);
  QString lastLogPath = stream.readLine();

  file.close();

  qDebug() << "Opening last file used: " << lastLogPath;
  return lastLogPath;
}

QStringList FileHandler::getFileContent(QString filePath) {

  QFile file(filePath);

  if (!file.open(QIODevice::ReadOnly)) {
    return QStringList();
  }

  QTextStream stream(&file);
  QStringList list;

  while (!stream.atEnd()) {
    list << stream.readLine();
  }

  file.close();

  return list;
}

void FileHandler::saveHighlightList(QList<HighlightData>& highlightList) {
  QFile file(highlightFileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  file.resize(0);

  QTextStream stream(&file);

  for (auto highlight : highlightList) {
      QString line = highlight.text() + "," + highlight.foreColor().name() + "," +
                     highlight.backColor().name() + "\n";

      qDebug() << line;

      stream << line;
  }

  file.close();
}

QList<HighlightData> FileHandler::getHighlightList() {

  QFile file(highlightFileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  if (file.size() <= 0) {
    qDebug() << "There is nothing in  file log path saved.";
    return QList<HighlightData>();
  }

  QTextStream stream(&file);
  QList<HighlightData> highlightList;

  while (!stream.atEnd()) {
    QStringList stringList = stream.readLine().split(",");

    HighlightData highlight;
    highlight.setText(stringList.at(HighlightPos::TEXT));
    highlight.setForeColor(QColor(stringList.at(HighlightPos::FOREGROUND)));
    highlight.setBackColor(QColor(stringList.at(HighlightPos::BACKGROUND)));

    highlightList.append(highlight);
  }

  file.close();

  return highlightList;
}
