#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QScrollBar>
#include <QStandardItemModel>

#include "highlightdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void applyHighlights();

private slots:
  void on_actionOpen_file_triggered();

  void on_actionExit_triggered();

  void on_actionHighlights_triggered();

  void on_actionAbout_triggered();

  void onFileChanged(QString fileName);

  void onHighlightsChanged(QList<HighlightData> highlightDataList);

  void on_actionAuto_scroll_changed();

  void onScrollChanged(int);

private:
  Ui::MainWindow *m_ui;
  QFileSystemWatcher *m_fileWatcher;
  HighlightDialog m_highlightDialog;
  QList<HighlightData> m_highlightDataList;
  QStandardItemModel *m_model;
  int m_lastLineLoaded;
  int m_fileLinesCount;

  void reopenLastFile();
  void startFileWatcher(QString filePath);
  void clearHighlights();
};

#endif // MAINWINDOW_H
