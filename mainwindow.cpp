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
    : QMainWindow(parent), m_ui(new Ui::MainWindow),
      m_model(new QStandardItemModel()), m_lastLineLoaded(0) {
  m_ui->setupUi(this);

  m_ui->logListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_ui->logListView->setViewMode(QListView::ListMode);
  m_ui->logListView->setModel(m_model);

  reopenLastFile();
}

MainWindow::~MainWindow() { delete m_ui; }

void MainWindow::on_actionOpen_file_triggered() {
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), "C://", "All files (*.*);;Text (*.txt)");

  if (filePath.isEmpty()) {
      qDebug() << "No file selected.";
      return;
  }

  startFileWatcher(filePath);

  FileHandler::saveOpenedFilePath(filePath);

  onFileChanged(filePath);
}

void MainWindow::on_actionExit_triggered() { QApplication::quit(); }

void MainWindow::on_actionHighlights_triggered() {

  HighlightDialog highlight;
  highlight.setModal(true);
  highlight.exec();
}

void MainWindow::onFileChanged(QString filePath) {
    qDebug() << "File changed, updating...";

    QStringList fileLines(FileHandler::getFileContent(filePath));
    unsigned int fileLinesSize = static_cast<unsigned>(fileLines.size());

    // file wiped or some like this
    if (fileLinesSize <= m_lastLineLoaded) {
        qDebug() << "File wiped? Cleaning view.";
        m_model->clear();
    }

    for (unsigned int i = m_lastLineLoaded; i < fileLinesSize; i++) {
      m_model->appendRow(new QStandardItem(fileLines.at(static_cast<signed>(i))));
    }

    m_lastLineLoaded = fileLinesSize;

    qDebug() << "Last line loaded: " << m_lastLineLoaded;

    // POC
    //  QModelIndex vIndex = model->index(0, 0);
    //  QMap<int, QVariant> vMap = model->itemData(vIndex);
    //  vMap.insert(Qt::BackgroundRole, QVariant(QBrush(Qt::red)));
    //  vMap.insert(Qt::ForegroundRole, QVariant(QBrush(Qt::white)));
    //  model->setItemData(vIndex, vMap);
}

void MainWindow::reopenLastFile() {
  QString lastLogPath = FileHandler::getLastLogFile();

  if (lastLogPath.size() > 0) {
    onFileChanged(lastLogPath);

    startFileWatcher(lastLogPath);
  }
}

void MainWindow::startFileWatcher(QString filePath) {
  m_fileWatcher = new QFileSystemWatcher(this);

  m_fileWatcher->addPath(filePath);

  connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this,
          &MainWindow::onFileChanged);
}

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::information(
      this, "Help",
      "<a href='https://github.com/gabrieljardim/LogFilter'>LogFilter github "
      "repository.</a>");
}
