#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = 0);
    ~ImportDialog();

private:
    Ui::ImportDialog *ui;
signals:
    void sendData(int quantitly,QString filePath);
private slots:
    void on_pushButton_clicked();
    void on_openPBtn_clicked();
    void on_pushButton_2_clicked();
};

#endif // IMPORTDIALOG_H
