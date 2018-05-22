#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include "QFileDialog"
#include "QTextStream"
#include <QVector>
#include <QSet>
#include <QListWidgetItem>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void keyPressEvent(QKeyEvent* keyevent);

private slots:
    void on_addOnePBtn_clicked();

    void on_addBatchPBtn_clicked();

    void on_openFilePBtn_clicked();

    QString openSaveFile();

    void loadFileInList(QString filePath);

    bool checkRepeat();
private:
    Ui::Widget *ui;
    QSet<QString> fileContentSet;
    QVector<QString> dataContentVec;

private slots:
   bool eventFilter(QObject *watched, QEvent *event);
   void checkTextLength(QString text);
   void deleteItemsSlot(QListWidgetItem *item);
};

#endif // WIDGET_H
