#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "QMessageBox"
#include "QFileSystemWatcher"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //QFileDialog* dialog = new QFileDialog(this, Qt::Widget);
    //dialog->show();
    //connect(dialog, SIGNAL(fileSelected(QString)), this, SLOT(onFileSelected(QString)));
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "C://",
                                                    "Texto (*.txt)");

    QFileSystemWatcher* fileWatcher = new QFileSystemWatcher(this);

    fileWatcher->addPath(fileName);

    connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateLabel(QString)));

    updateLabel(fileName);

}


void MainWindow::updateLabel(QString fileName)
{
    QFile textFile(fileName);


    if(!textFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", textFile.errorString());
        return;
    }


    QString fileContent(textFile.readAll());

    this->ui->label->setText(fileContent);

}
