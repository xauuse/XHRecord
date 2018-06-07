#include "fileutil.h"



FileUtil::FileUtil(QString path)
{

    this->path = path;
    file.setFileName(path);

}

QVector<QString> FileUtil::readFile2Vec()
{
    QTextStream in(&file);
    QVector<QString> fileData;

    while(!in.atEnd()){
        QString line = in.readLine();
        fileData<<line;
    }

    return fileData;
}

bool FileUtil::setModel(int model)
{ 
    if(model == READONLY){
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        return true;
    }
    else if (model == WRITEONLY) {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        return true;
    }
    else if (model == WRITEAPPEND) {
        file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append);
        return true;
    }
    else if (model == READWRITE){
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        return true;
    }
    return false;
}

void FileUtil::appenWriteFile(QString text)
{
    QTextStream out(&file);
    out << text <<"\n";
    out.flush();
}

void FileUtil::coverWriteFile(QString text)
{
    QTextStream out(&file);
    out << text ;
    out.flush();
}

void FileUtil::closeFile()
{
    file.close();
}

void FileUtil::cleanFile(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        file.close();
    }
}
