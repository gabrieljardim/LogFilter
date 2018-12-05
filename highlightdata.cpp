#include "highlightdata.h"

HighlightData::HighlightData() {}

HighlightData::~HighlightData() {}

QString HighlightData::text() const { return m_text; }

void HighlightData::setText(const QString &text) { m_text = text; }

QColor HighlightData::foreColor() const { return m_foreColor; }

void HighlightData::setForeColor(const QColor &foreColor) {
  m_foreColor = foreColor;
}

QColor HighlightData::backColor() const { return m_backColor; }

void HighlightData::setBackColor(const QColor &backColor) {
  m_backColor = backColor;
}

QString HighlightData::toString() {
  return "Text: " + m_text + " Fore: " + m_foreColor.name() +
         " Back:" + m_backColor.name();
}
