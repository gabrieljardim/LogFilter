#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemWatcher>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionOpen_file_triggered();

  void on_actionExit_triggered();

  void on_actionHighlights_triggered();

  void updateLabel(QString);

  void on_actionAbout_triggered();

private:
  Ui::MainWindow *m_ui;
  QFileSystemWatcher *m_fileWatcher;

  void reopenLastFile();

  void startFileWatcher(QString filePath);
};

#endif // MAINWINDOW_H
