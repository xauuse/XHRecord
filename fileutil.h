#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <QFile>
#include <QTextStream>
#include <QVector>


class FileUtil
{


public:
//    FileUtil();
    FileUtil(QString path);
    QVector<QString> readFile2Vec(); //读
    bool setModel(int model);
    void appenWriteFile(QString text);    //写文件，追加
    void coverWriteFile(QString text);    //写文件全部覆盖
    void closeFile();

    int READONLY = 0;
    int WRITEONLY = 1;
    int WRITEAPPEND = 2;
    int READWRITE = 3;
    static void cleanFile(QString path);
private:
    QString path;
    QFile file;
};

#endif // FILEUTIL_H
