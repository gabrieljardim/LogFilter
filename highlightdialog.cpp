#include "highlightdialog.h"
#include "ui_highlightdialog.h"

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QString>

#include "filehandler.h"

HighlightDialog::HighlightDialog(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::HighlightDialog),
      m_model(new QStandardItemModel()), m_foregroundColor(Qt::black),
      m_backgroundColor(Qt ::white) {
  m_ui->setupUi(this);

  // disable row editing
  m_ui->highlightListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  m_ui->highlightListView->setViewMode(QListView::ListMode);
  m_ui->highlightListView->setModel(m_model);

  loadHighlightConfig(FileHandler::getHighlightList());

  // TODO: Implement this buttons
  m_ui->moveDownButton->setDisabled(true);
  m_ui->moveUpButton->setDisabled(true);
}

HighlightDialog::~HighlightDialog() { delete m_ui; }

void HighlightDialog::on_addButton_clicked() {
  QString text = m_ui->stringLineEdit->text();

  if (text.isEmpty()) {
    qDebug() << "String empty.";
    return;
  }

  addItemToModel(text, m_foregroundColor, m_backgroundColor);
  clearInputs();
}

void HighlightDialog::on_deleteButton_clicked() {

  m_model->removeRows(m_ui->highlightListView->currentIndex().row(), 1);
  clearInputs();
}

void HighlightDialog::on_moveUpButton_clicked() {
  // TODO: implement this
}

void HighlightDialog::on_moveDownButton_clicked() {
  // TODO: implement this
}

void HighlightDialog::on_okButton_clicked() {

  HighlightData highData;

  m_highlightList.clear();

  for (int i = 0; i < m_model->rowCount(); i++) {
    QModelIndex modelIndex = m_model->item(i)->index();

    highData.setText(modelIndex.data().toString());
    highData.setBackColor(
        modelIndex.data(Qt::BackgroundColorRole).value<QColor>());
    highData.setForeColor(modelIndex.data(Qt::ForegroundRole).value<QColor>());

    m_highlightList.append(highData);
  }

  // saving highlight data to file
  FileHandler::saveHighlightList(m_highlightList);

  emit highlightsChanged(m_highlightList);
  this->close();
}

void HighlightDialog::on_cancelButton_clicked() { this->close(); }

void HighlightDialog::on_foregroundColorButton_clicked() {
  m_foregroundColor =
      QColorDialog::getColor(Qt::white, this, "Pick a color...");

  if (m_foregroundColor.isValid()) {
    QPalette palette;
    palette.setColor(QPalette::Text, m_foregroundColor);
    palette.setColor(QPalette::Base,
                     m_ui->stringLineEdit->palette().base().color());

    m_ui->stringLineEdit->setPalette(palette);

    m_ui->foregroundColorButton->setStyleSheet(
        QString("QToolButton{ background: %1; }")
            .arg(m_foregroundColor.name()));
  }
}

void HighlightDialog::on_backgroundColorButton_clicked() {
  m_backgroundColor =
      QColorDialog::getColor(Qt::white, this, "Pick a color...");

  if (m_backgroundColor.isValid()) {
    QPalette palette;
    palette.setColor(QPalette::Base, m_backgroundColor);
    palette.setColor(QPalette::Text,
                     m_ui->stringLineEdit->palette().text().color());

    m_ui->stringLineEdit->setPalette(palette);

    m_ui->backgroundColorButton->setStyleSheet(
        QString("QToolButton{ background: %1; }")
            .arg(m_backgroundColor.name()));
  }
}

void HighlightDialog::on_stringLineEdit_returnPressed() {
  on_addButton_clicked();
}

void HighlightDialog::on_highlightListView_clicked(const QModelIndex &index) {

  m_ui->stringLineEdit->setText(index.data().toString());
  m_foregroundColor = index.data(Qt::ForegroundRole).value<QColor>();
  m_backgroundColor = index.data(Qt::BackgroundColorRole).value<QColor>();

  QPalette palette;
  palette.setColor(QPalette::Base, m_backgroundColor);
  palette.setColor(QPalette::Text, m_foregroundColor);

  m_ui->foregroundColorButton->setStyleSheet(
      QString("QToolButton{ background: %1; }").arg(m_foregroundColor.name()));

  m_ui->backgroundColorButton->setStyleSheet(
      QString("QToolButton{ background: %1; }").arg(m_backgroundColor.name()));

  m_ui->stringLineEdit->setPalette(palette);
}

QList<HighlightData> HighlightDialog::highlightList() {

  if (m_highlightList.isEmpty()) {
    m_highlightList = FileHandler::getHighlightList();
  }

  return m_highlightList;
}

void HighlightDialog::loadHighlightConfig(QList<HighlightData> highlightList) {

  for (int i = 0; i < highlightList.size(); i++) {
    HighlightData highlight = highlightList.at(i);
    addItemToModel(highlight.text(), highlight.foreColor(),
                   highlight.backColor());
  }
}

void HighlightDialog::addItemToModel(QString text, QColor foreground,
                                     QColor background) {
  QStandardItem *item = new QStandardItem(text);
  m_model->appendRow(item);

  QModelIndex vIndex = m_model->index((m_model->rowCount() - 1), 0);
  QMap<int, QVariant> vMap = m_model->itemData(vIndex);
  vMap.insert(Qt::BackgroundRole, QVariant(background));
  vMap.insert(Qt::ForegroundRole, QVariant(foreground));
  m_model->setItemData(vIndex, vMap);
}

void HighlightDialog::clearInputs() {
    m_ui->stringLineEdit->setPalette(QPalette());
    m_ui->stringLineEdit->clear();

    m_ui->foregroundColorButton->setStyleSheet(
        QString("QToolButton{ background: %1; }")
            .arg(Qt::white));

    m_ui->backgroundColorButton->setStyleSheet(
        QString("QToolButton{ background: %1; }")
            .arg(Qt::white));
}
