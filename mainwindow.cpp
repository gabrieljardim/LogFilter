#include "mainwindow.h"
#include "filehandler.h"
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
      m_highlightDataList(FileHandler::getHighlightList()) {
  m_ui->setupUi(this);

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
      this, tr("Open File"), "C://", "All files (*.*);;Texto (*.txt)");

  startFileWatcher(filePath);

  FileHandler::saveOpenedFilePath(filePath);

  onFileChanged(filePath);
}

void MainWindow::on_actionExit_triggered() { QApplication::quit(); }

void MainWindow::on_actionHighlights_triggered() { m_highlightDialog.exec(); }

void MainWindow::onFileChanged(QString fileName) {

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

  m_ui->listView->setModel(model);

  // apply highlights after view update
  applyHighlights();
}

void MainWindow::applyHighlights() {

  QModelIndex modelIndex;
  QMap<int, QVariant> map;
  QListIterator<HighlightData> i(m_highlightDataList);

  while (i.hasNext()) {
    HighlightData highlight = i.next();

    for (int row = 0; row < m_ui->listView->model()->rowCount(); row++) {

      // gets listview item
      modelIndex = m_ui->listView->model()->index(row, 0);

      if (modelIndex.data().toString().contains(highlight.text())) {

        qDebug() << "Applying color properties to" << highlight.text()
                 << "row.";

        // doing this to be possible to use color properties
        map = m_ui->listView->model()->itemData(modelIndex);

        // setting colors
        map.insert(Qt::BackgroundRole, QVariant(QBrush(highlight.backColor())));
        map.insert(Qt::ForegroundRole, QVariant(QBrush(highlight.foreColor())));

        // re-adding itens to listview
        m_ui->listView->model()->setItemData(modelIndex, map);
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
