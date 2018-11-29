#include "highlightdialog.h"
#include "ui_highlightdialog.h"

HighlightDialog::HighlightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HighlightDialog)
{
    ui->setupUi(this);
}

HighlightDialog::~HighlightDialog()
{
    delete ui;
}
