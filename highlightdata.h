#ifndef HIGHLIGHTDATA_H
#define HIGHLIGHTDATA_H

#include <QColor>
#include <QObject>
#include <QString>

enum HighlightPos { TEXT, FOREGROUND, BACKGROUND };

class HighlightData {
public:
  HighlightData();
  ~HighlightData();

  QString text() const;
  void setText(const QString &text);

  QColor foreColor() const;
  void setForeColor(const QColor &foreColor);

  QColor backColor() const;
  void setBackColor(const QColor &backColor);

  QString toString();

private:
  QString m_text;
  QColor m_foreColor;
  QColor m_backColor;
};

#endif // HIGHLIGHTDATA_H
