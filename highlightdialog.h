#ifndef HIGHLIGHTDIALOG_H
#define HIGHLIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class HighlightDialog;
}

class HighlightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighlightDialog(QWidget *parent = nullptr);
    ~HighlightDialog();

private:
    Ui::HighlightDialog *ui;
};

#endif // HIGHLIGHTDIALOG_H
