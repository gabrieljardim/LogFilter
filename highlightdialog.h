#ifndef HIGHLIGHTDIALOG_H
#define HIGHLIGHTDIALOG_H

#include <QColor>
#include <QDialog>
#include <QList>
#include <QStandardItemModel>

#include "highlightdata.h"

namespace Ui {
class HighlightDialog;
}

class HighlightDialog : public QDialog {
  Q_OBJECT

public:
  explicit HighlightDialog(QWidget *parent = nullptr);
  ~HighlightDialog();

  QList<HighlightData> highlightList();

signals:
  void highlightsChanged(QList<HighlightData>);

private slots:
  void on_addButton_clicked();

  void on_deleteButton_clicked();

  void on_moveUpButton_clicked();

  void on_moveDownButton_clicked();

  void on_okButton_clicked();

  void on_cancelButton_clicked();

  void on_foregroundColorButton_clicked();

  void on_backgroundColorButton_clicked();

  void on_stringLineEdit_returnPressed();

  void on_highlightListView_clicked(const QModelIndex &index);

private:
  Ui::HighlightDialog *m_ui;
  QStandardItemModel *m_model;
  QList<QStandardItem *> m_list;
  QList<HighlightData> m_highlightList;

  QColor m_foregroundColor;
  QColor m_backgroundColor;

  void loadHighlightConfig(QList<HighlightData> highlightList);
  void addItemToModel(QString text, QColor foreground, QColor background);
};

#endif // HIGHLIGHTDIALOG_H
