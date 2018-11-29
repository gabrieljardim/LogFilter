#include "mainwindow.h"
#include "filehandler.h"
#include "highlightdialog.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStringList>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);

  reopenLastFile();
}

MainWindow::~MainWindow() { delete m_ui; }

void MainWindow::on_actionOpen_file_triggered() {
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), "C://", "All files (*.*);;Texto (*.txt)");

  startFileWatcher(filePath);

  FileHandler::saveOpenedFilePath(filePath);

  updateLabel(filePath);
}

void MainWindow::on_actionExit_triggered() { QApplication::quit(); }

void MainWindow::on_actionHighlights_triggered() {

  HighlightDialog highlight;
  highlight.setModal(true);
  highlight.exec();
}

void MainWindow::updateLabel(QString fileName) {

  // TODO: Make a method to load the file a first time and then only load in
  // that slot what has changed..

  qDebug() << "File changed, updating...";

  QStringList fileLines(FileHandler::getFileContent(fileName));

  QStandardItemModel *model = new QStandardItemModel();
  QList<QStandardItem *> list;

  m_ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_ui->listView->setViewMode(QListView::ListMode);

  for (int i = 0; i < fileLines.size(); i++) {
    list << new QStandardItem(fileLines.at(i));
  }

  model->appendColumn(list);

  // POC
  //  QModelIndex vIndex = model->index(0, 0);
  //  QMap<int, QVariant> vMap = model->itemData(vIndex);
  //  vMap.insert(Qt::BackgroundRole, QVariant(QBrush(Qt::red)));
  //  vMap.insert(Qt::ForegroundRole, QVariant(QBrush(Qt::white)));
  //  model->setItemData(vIndex, vMap);

  m_ui->listView->setModel(model);
}

void MainWindow::reopenLastFile() {
  QString lastLogPath = FileHandler::getLastLogFile();

  if (lastLogPath.size() > 0) {
    updateLabel(lastLogPath);

    startFileWatcher(lastLogPath);
  }
}

void MainWindow::startFileWatcher(QString filePath) {
  m_fileWatcher = new QFileSystemWatcher(this);

  m_fileWatcher->addPath(filePath);

  connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this,
          &MainWindow::updateLabel);
}

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::information(
      this, "Help",
      "<a href='https://github.com/gabrieljardim/LogFilter'>LogFilter github "
      "repository.</a>");
}
