#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    ui->fixLEdit->installEventFilter(this);
    ui->changeLEdit->installEventFilter(this);
    ui->addBatchPBtn->installEventFilter(this);
    connect(ui->changeLEdit,SIGNAL(textChanged(QString)),this,SLOT(checkTextLength(QString)));

}

Widget::~Widget()
{
    delete ui;
}


/**
 * @brief 设置回车键切换控件功能
 * @param keyevent
 */
void Widget::keyPressEvent(QKeyEvent* keyevent)
{
    if( keyevent->key() == Qt::Key_Return || keyevent->key() == Qt::Key_Enter)
    {
        focusNextChild();
    }
}

void Widget::on_addOnePBtn_clicked()
{


}
/**
 * @brief 批量处理按钮的方法
 */
void Widget::on_addBatchPBtn_clicked()
{
    QString filePath = "";
    int lenth = ui->lenSBox->value();               //数据长度
    int interval = ui->intervalSBox->value();       //间隔长度 单卡贴标
    QString fixStr = ui->fixLEdit->text();          //固定部分
    int changeNum = ui->changeLEdit->text().toInt();//改变部分
    int quantity = ui->quantitySBox->value();

    if(quantity == 0 ){                              //判断添加数量是否为零
        QMessageBox msgBox;
         msgBox.setText("Quantity is Empty!");
         msgBox.exec();
         return ;
    }
    if(ui->filePathLEdit->text().isEmpty()){
        filePath = openSaveFile();    //如果地址栏为空则打开文件对话框重新选择
    }
    else {
        filePath = ui->filePathLEdit->text();
    }
    loadFileInList(filePath);  //加载数据为了将文件内容存入QSet内
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
        return;
    QTextStream out(&file);
    int dataContent;
    int strLen = lenth - fixStr.length();

    for(int i=0;i<quantity;i++){
        dataContent = changeNum + interval * i;
        QString value = QString("%1").arg(dataContent,strLen,10,QLatin1Char('0'));
        //out << fixStr << value << "\n";
        dataContentVec<<fixStr+value;
    }

    if(checkRepeat()){
        for(int i=0;i<dataContentVec.size();i++){
            out << dataContentVec[i] << "\n";
        }
    }
    else{
        QMessageBox msgBox2;
        msgBox2.setText("出现重复数据，请谨慎核对！");
        msgBox2.exec();
        out.flush();
        file.close(); //关闭文件
        return ;
    }
    out.flush();
    file.close(); //关闭文件
    loadFileInList(filePath);
}
/**
 * @brief 打开文件夹按钮的槽
 */
void Widget::on_openFilePBtn_clicked()
{
    QString filePath = openSaveFile();
    loadFileInList(filePath);
}
/**
 * @brief 打开选择文件夹
 * @return
 */
QString Widget::openSaveFile()
{
    QString filepath = "";
    filepath = QFileDialog::getOpenFileName(this,
        tr("选择文件"),
        "",
        tr("File (*.txt *.prd *.inp)")); //选择路径
    if(!filepath.isEmpty())
    {
        ui->filePathLEdit->setText(filepath);
        return filepath;
    }
    return NULL;
}

/**
 * @brief 用于加载文件内容进入ListWidget
 * @param filePath
 */
void Widget::loadFileInList(QString filePath)
{
    //      new QListWidgetItem(tr("Oak"), listWidget);
    ui->listWidget->clear();
    QFile file(filePath);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
     fileContentSet.clear(); //先全部清除
     QTextStream in(&file);
     QString line = in.readLine().trimmed();
     while (!line.isNull()) {

         QListWidgetItem *listWidgetItem =  new QListWidgetItem(line, ui->listWidget);
         listWidgetItem->setTextAlignment(Qt::AlignCenter);
         fileContentSet<<line;
         line = in.readLine().trimmed();

     }
}
/**
 * @brief 用于检查是否文件内已有重复数据
 * @return
 */
bool Widget::checkRepeat()
{
    QSet<QString>::iterator iter;
    for(int i=0;i<dataContentVec.size();i++)
    {
        if((iter = fileContentSet.find(dataContentVec[i])) != fileContentSet.end())
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief 自定义事件过滤器，主要用于判断固定部分和改变部分长度的判断
 * @param watched
 * @param event
 * @return
 */
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    //固定部分
   if(watched == ui->fixLEdit){
       if(event->type() == QEvent::FocusIn){
           int totalLen = ui->lenSBox->value();
           int changeLen = ui->changeLEdit->text().length();
           int fixLen = totalLen - changeLen;
           ui->fixLEdit->setMaxLength(fixLen);
       }
   }
   //改变部分
   if(watched == ui->changeLEdit){
       if(event->type() == QEvent::FocusIn){
           int totalLen = ui->lenSBox->value();
           int fixLen = ui->fixLEdit->text().length();
           int changeLen = totalLen - fixLen;
           ui->changeLEdit->setMaxLength(changeLen);
       }

   }

   if(watched == ui->addBatchPBtn){
       if(event->type() == QEvent::KeyPress){
           on_addBatchPBtn_clicked();
       }
   }
   return QWidget::eventFilter(watched,event);
}

/**
 * @brief 用于检查改动部分的数据长度，如果跟固定部分的长度相同，则跳到下一个焦点
 * @param text
 */
void Widget::checkTextLength(QString text)
{
    int totalLen = ui->lenSBox->value();
    int fixLen = ui->fixLEdit->text().length();
    int changeLen = totalLen - fixLen;
    int currentLen = text.length();
    if(changeLen == currentLen){
        focusNextChild();
    }
}



