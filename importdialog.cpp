#include "importdialog.h"
#include "ui_importdialog.h"
#include <QFileDialog>
#include <QMessageBox>

ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::on_pushButton_clicked()
{
    int count = ui->countSBox->value();
    QString path = ui->filePathLEdit->text();
    if(count == 0||path.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("有参数为空，请确认参数是否填写！");
        msgBox.exec();
        return ;
    }
    emit sendData(count,path);
    close();
}

void ImportDialog::on_openPBtn_clicked()
{
    QString filepath = "";
    filepath = QFileDialog::getOpenFileName(this,
        tr("选择保存的路径"),
        "D:\\xhRecord",
        tr("File (*.txt *.prd *.inp)")); //选择路径
    if(!filepath.isEmpty())
    {
        ui->filePathLEdit->setText(filepath);
    }

}

void ImportDialog::on_pushButton_2_clicked()
{
    close();
}
