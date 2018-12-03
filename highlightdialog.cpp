#include "highlightdialog.h"
#include "ui_highlightdialog.h"

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QString>

HighlightDialog::HighlightDialog(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::HighlightDialog),
      m_model(new QStandardItemModel()), m_foregroundColor(Qt::black),
      m_backgroundColor(Qt ::white) {
  m_ui->setupUi(this);

  m_ui->highlightListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_ui->highlightListView->setViewMode(QListView::ListMode);
  m_ui->highlightListView->setModel(m_model);
}

HighlightDialog::~HighlightDialog() { delete m_ui; }

void HighlightDialog::on_addButton_clicked() {
  QString text = m_ui->stringLineEdit->text();

  if (text.isEmpty()) {
    qDebug() << "String empty.";
    return;
  }

  QStandardItem *item = new QStandardItem(text);
  m_model->appendRow(item);

  QModelIndex vIndex = m_model->index((m_model->rowCount() - 1), 0);
  QMap<int, QVariant> vMap = m_model->itemData(vIndex);
  vMap.insert(Qt::BackgroundRole, QVariant(m_backgroundColor));
  vMap.insert(Qt::ForegroundRole, QVariant(m_foregroundColor));
  m_model->setItemData(vIndex, vMap);

  m_ui->stringLineEdit->setPalette(QPalette());
  m_ui->stringLineEdit->clear();
}

void HighlightDialog::on_deleteButton_clicked() {

  m_model->removeRows(m_ui->highlightListView->currentIndex().row(), 1);
}

void HighlightDialog::on_moveUpButton_clicked() {

  //  int selectedItemIndex = m_ui->highlightListView->currentIndex().row();

  //  // getting selected item
  //  QStandardItem *item = new QStandardItem(
  //      m_ui->highlightListView->currentIndex().data(Qt::DisplayRole).toString());

  //  // inserting the selected item on the previous line
  //  m_model->insertRow((selectedItemIndex - 1), item);

  //  // removing selected item
  //  m_model->removeRows(selectedItemIndex, 1);

  //  // selecting moved item
  //  m_ui->highlightListView->setCurrentIndex(
  //      m_ui->highlightListView->model()->index(selectedItemIndex - 1, 0));
}

void HighlightDialog::on_moveDownButton_clicked() {}

void HighlightDialog::on_okButton_clicked() {}

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
  }
}

void HighlightDialog::on_stringLineEdit_returnPressed() {
  on_addButton_clicked();
}

void HighlightDialog::on_highlightListView_clicked(const QModelIndex &index) {
  qDebug() << index.data().toString();
  qDebug() << index.data(Qt::BackgroundColorRole);
}
