#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemWatcher>
#include <QMainWindow>

#include "highlightdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void applyHighlights();

private slots:
  void on_actionOpen_file_triggered();

  void on_actionExit_triggered();

  void on_actionHighlights_triggered();

  void on_actionAbout_triggered();

  void onFileChanged(QString fileName);

  void onHighlightsChanged(QList<HighlightData> highlightDataList);

private:
  Ui::MainWindow *m_ui;
  QFileSystemWatcher *m_fileWatcher;
  HighlightDialog m_highlightDialog;
  QList<HighlightData> m_highlightDataList;

  void reopenLastFile();

  void startFileWatcher(QString filePath);
};

#endif // MAINWINDOW_H
