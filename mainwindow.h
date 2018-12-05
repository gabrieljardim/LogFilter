#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_actionOpen_file_triggered();

  void on_actionExit_triggered();

  void on_actionHighlights_triggered();

  void onFileChanged(QString);

  void on_actionAbout_triggered();

private:
  Ui::MainWindow *m_ui;
  QFileSystemWatcher *m_fileWatcher;
  QStandardItemModel *m_model;
  unsigned int m_lastLineLoaded;

  void reopenLastFile();

  void startFileWatcher(QString filePath);
};

#endif // MAINWINDOW_H
