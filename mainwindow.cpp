#include "mainwindow.h"
#include "filehandler.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QStringList>
#include <QStringListModel>

const int g_renderOffset = 100;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow),
      m_fileWatcher(new QFileSystemWatcher(this)),
      m_highlightDataList(FileHandler::getHighlightList()),
      m_model(new QStandardItemModel()), m_lastLineLoaded(0) {
  m_ui->setupUi(this);

  m_ui->logListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_ui->logListView->setViewMode(QListView::ListMode);
  m_ui->logListView->setModel(m_model);
  QScrollBar *bar = m_ui->logListView->verticalScrollBar();

  connect(bar, SIGNAL(valueChanged(int)), this, SLOT(onScrollChanged(int)));

  reopenLastFile();

  // setup highlight dialog
  m_highlightDialog.setModal(true);

  connect(&m_highlightDialog, &HighlightDialog::highlightsChanged, this,
          &MainWindow::onHighlightsChanged);

  // applying highlights on boot
  applyHighlights();
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

void MainWindow::on_actionHighlights_triggered() { m_highlightDialog.exec(); }

void MainWindow::onFileChanged(QString fileName) {
  qDebug() << "File changed, updating...";

  QStringList fileLines(FileHandler::getFileContent(fileName));
  m_fileLinesCount = fileLines.size();

  if (m_fileLinesCount <= 0 || m_fileLinesCount < m_lastLineLoaded) {
    m_model->clear();
  }

  qDebug() << "File has" << m_fileLinesCount << "lines";
  qDebug() << "Last line read" << m_lastLineLoaded;

  for (int i = m_lastLineLoaded; i < m_fileLinesCount; i++) {
    m_model->appendRow(new QStandardItem(fileLines.at(i)));
  }

  m_lastLineLoaded = m_fileLinesCount;

  // Move scroll to the bottom if enabled
  on_actionAuto_scroll_changed();

  m_ui->logListView->setModel(m_model);

  // apply highlights after view update
  applyHighlights();
}

void MainWindow::applyHighlights() {

  QModelIndex modelIndex;
  QMap<int, QVariant> map;
  QListIterator<HighlightData> i(m_highlightDataList);

  // sets the iterator to the end for backward iteration
  i.toBack();

  while (i.hasPrevious()) {
    HighlightData highlight = i.previous();

    for (int row = 0; row < m_ui->logListView->model()->rowCount(); row++) {

      // gets listview item
      modelIndex = m_ui->logListView->model()->index(row, 0);

      if (modelIndex.data().toString().contains(highlight.text())) {

        qDebug() << "Applying color properties to" << highlight.text()
                 << "row.";

        // doing this to be possible to use color properties
        map = m_ui->logListView->model()->itemData(modelIndex);

        // setting colors
        map.insert(Qt::BackgroundRole, QVariant(QBrush(highlight.backColor())));
        map.insert(Qt::ForegroundRole, QVariant(QBrush(highlight.foreColor())));

        // re-adding itens to listview
        m_ui->logListView->model()->setItemData(modelIndex, map);
      }
    }
  }
}

void MainWindow::onHighlightsChanged(QList<HighlightData> highlightDataList) {

  m_highlightDataList = highlightDataList;
  applyHighlights();
}

void MainWindow::reopenLastFile() {
  QString lastLogPath = FileHandler::getLastLogFile();

  if (lastLogPath.size() > 0) {
    onFileChanged(lastLogPath);

    startFileWatcher(lastLogPath);
  }
}

void MainWindow::startFileWatcher(QString filePath) {
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

void MainWindow::on_actionAuto_scroll_changed() {
//  if (m_ui->actionAuto_scroll->isChecked()) {
//    m_ui->listView->scrollToBottom();
//  }
}

void MainWindow::onScrollChanged(int value) {
  qDebug() << "Render" << value << "to" << (g_renderOffset + value);
}
