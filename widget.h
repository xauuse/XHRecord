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
#include <QKeyEvent>
#include <QMenu>
#include "importdialog.h"
#include "fileutil.h"

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

protected:

private slots:
    void on_addOnePBtn_clicked();

    void on_addBatchPBtn_clicked();

    void on_openFilePBtn_clicked();


private:
    Ui::Widget *ui;
    ImportDialog *impDialog;
    QMenu *popMenu;

    QSet<QString> fileContentSet;
    QVector<QString> dataContentVec;
    const  QString ONETYPE = "one";
    const  QString BATCHTYPE = "batch";
    QString openSaveFile();

    void loadFileInList(QString filePath);      //将文件内容存入ListWidget
    void loadDataInList(QVector<QString> data); //将数据插入进ListWidget
    QVector<QString> loadLWidgetData();         //加载listwidget的数据存入vector数组
    //   bool loadListData2File();

    bool checkRepeat();
    bool checkRepeatInListWidget(QVector<QString> data);
    bool checkParamValid(QString type);
    bool checkSearchValue();                    //检查DataSearch参数的是有为空的

    int importCount;
    QString importPath;

    QVector<QString> mainVec;  //用于数据查找的两个Vector数组
    QVector<QString> impVec;


private slots:
   bool eventFilter(QObject *watched, QEvent *event);
   void checkTextLength(QString text);
   void deleteItemsSlot(QListWidgetItem *item);
   void on_savePBtn_clicked();
   void on_lineEdit_textChanged(const QString &arg1);
   void on_listWidget_customContextMenuRequested(const QPoint &pos);
   void openImportDialog();
   void receiveData(int quantitly,QString filePath);
   void on_mainPBtn_clicked();
   void on_inputPBtn_clicked();
   void on_noFoundPBtn_clicked();
   void on_outputPBtn_clicked();
   void on_searchPBtn_clicked();
};

#endif // WIDGET_H
