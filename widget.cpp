#include "widget.h"
#include "ui_widget.h"
#include "QDebug"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    ui->fixLEdit->installEventFilter(this);
    ui->changeLEdit->installEventFilter(this);
    ui->addBatchPBtn->installEventFilter(this);
    ui->addOnePBtn->installEventFilter(this);
    connect(ui->changeLEdit,SIGNAL(textChanged(QString)),this,SLOT(checkTextLength(QString)));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(deleteItemsSlot(QListWidgetItem*)));

    popMenu = new QMenu(this);
    QAction *importSeed = new QAction(tr("Import"),this);
    QAction *cancelSeed = new QAction(tr("Cancel"),this);
    popMenu->addAction(importSeed);
    popMenu->addAction(cancelSeed);
    connect( importSeed,SIGNAL(triggered(bool)), this, SLOT(openImportDialog()) );

    impDialog = new ImportDialog(this);
    impDialog->setModal(true);
    connect(impDialog,SIGNAL(sendData(int,QString)),this,SLOT(receiveData(int,QString)));


    //将tabwidget背景设置透明
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }");



    ui->listWidget->setStyleSheet("QListWidget{border:1px solid gray; color:black; }"
//                               "QListWidget::Item{padding-top:10px; padding-bottom:4px; }"
//                               "QListWidget::Item:hover{background:skyblue; }"
                               "QListWidget::item:selected{background:lightblue; color:red; }"
                               "QListWidget::item:selected:!active{border-width:0px; background:lightblue; }"
                               );

    QDir dir("D:\\xhRecord");
    if(!dir.exists()){
        QDir createDir("D:\\");
        createDir.mkdir("xhRecord");
    }

}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 接收来自importDialog传递来的数据
 * @param quantitly
 * @param filePath
 */
void Widget::receiveData(int quantitly, QString filePath)
{
    importCount = quantitly;
    importPath = filePath;
}

/**
 * @brief 设置单张添加
 */
void Widget::on_addOnePBtn_clicked()
{
    QString filePath = "";
    int lenth = ui->lenSBox->value();               //数据长度
    int interval = ui->intervalSBox->value();       //间隔长度 单卡贴标
    QString fixStr = ui->fixLEdit->text();          //固定部分
    int changeNum = ui->changeLEdit->text().toInt();//改变部分
    dataContentVec.clear();
    //检验各种参数问题
    if(!checkParamValid(ONETYPE)){
        return ;
    }
 //   if(ui->filePathLEdit->text().isEmpty()){
//        filePath = openSaveFile();    //如果地址栏为空则强制打开文件对话框重新选择
        int dataContent;
        int strLen = lenth - fixStr.length();
        QVector<QString> inputVec;

        dataContent = changeNum;
        QString value = QString("%1").arg(dataContent,strLen,10,QLatin1Char('0'));
        inputVec<<fixStr+value;

        if(!checkRepeatInListWidget(inputVec)){
            QMessageBox msgBox2;
            msgBox2.setText("出现重复数据，请谨慎核对！");
            msgBox2.exec();
            return ;
        }
        loadDataInList(inputVec);
        value = QString("%1").arg(dataContent+interval,strLen,10,QLatin1Char('0'));
        ui->changeLEdit->setText(value);
//    }
//    else {
//        filePath = ui->filePathLEdit->text();
//        loadFileInList(filePath);  //加载数据为了将文件内容存入QSet内
//        QFile file(filePath);
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
//            return;
//        QTextStream out(&file);
//        int dataContent;
//        int strLen = lenth - fixStr.length();

//        dataContent = changeNum ;
//        QString value = QString("%1").arg(dataContent,strLen,10,QLatin1Char('0'));
//        //out << fixStr << value << "\n";
//        dataContentVec<<fixStr+value;
//        if(checkRepeat()){
//            out<<dataContentVec[0]<<"\n";
//        }
//        else{
//            QMessageBox msgBox2;
//            msgBox2.setText("出现重复数据，请谨慎核对！");
//            msgBox2.exec();
//            out.flush();
//            file.close(); //关闭文件
//            return ;
//        }
//        out.flush();
//        file.close(); //关闭文件
//        loadFileInList(filePath);

//        value = QString("%1").arg(dataContent+interval,strLen,10,QLatin1Char('0'));
//        ui->changeLEdit->setText(value);
//    }

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
    dataContentVec.clear();

    if(!checkParamValid(BATCHTYPE)){
        return ; //检查参数问题
    }
 //   if(ui->filePathLEdit->text().isEmpty()){

        int dataContent;
        int strLen = lenth - fixStr.length();
        QVector<QString> inputVec;
        for(int i=0;i<quantity;i++){
            dataContent = changeNum + interval * i;
            QString value = QString("%1").arg(dataContent,strLen,10,QLatin1Char('0'));
            inputVec<<fixStr+value;
        }
        if(!checkRepeatInListWidget(inputVec)){
            QMessageBox msgBox2;
            msgBox2.setText("出现重复数据，请谨慎核对！");
            msgBox2.exec();
            return ;
        }
        loadDataInList(inputVec);
        QString value = QString("%1").arg(dataContent+interval*quantity,strLen,10,QLatin1Char('0'));
        ui->changeLEdit->setText(value);

//    }
//    else {
//        filePath = ui->filePathLEdit->text();
//        loadFileInList(filePath);  //加载数据为了将文件内容存入QSet内
//        QFile file(filePath);
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
//            return;
//        QTextStream out(&file);
//        int dataContent;
//        int strLen = lenth - fixStr.length();

//        for(int i=0;i<quantity;i++){
//            dataContent = changeNum + interval * i;
//            QString value = QString("%1").arg(dataContent,strLen,10,QLatin1Char('0'));
//            //out << fixStr << value << "\n";
//            dataContentVec<<fixStr+value;
//        }
//        //查重
//        if(checkRepeat()){
//            for(int i=0;i<dataContentVec.size();i++){
//                out << dataContentVec[i] << "\n";
//            }
//        }
//        else{
//            QMessageBox msgBox2;
//            msgBox2.setText("出现重复数据，请谨慎核对！");
//            msgBox2.exec();
//            out.flush();
//            file.close(); //关闭文件
//            return ;
//        }
//        out.flush();
//        file.close(); //关闭文件
//        loadFileInList(filePath);
//    }

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
    filepath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                            "D:\\xhRecord",
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
     ui->listWidget->setCurrentRow(ui->listWidget->count()-1);

     int count = ui->listWidget->count();
     QString text = QString("数量：%1").arg(count);
     ui->totalCountLabel->setText(text);
}

void Widget::loadDataInList(QVector<QString> data)
{
    for(int i=0;i<data.size();i++){
        QListWidgetItem *listWidgetItem =  new QListWidgetItem(data[i], ui->listWidget);
        listWidgetItem->setTextAlignment(Qt::AlignCenter);
    }
    int count = ui->listWidget->count();
    QString text = QString("数量：%1").arg(count);
    ui->totalCountLabel->setText(text);

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);

}
/**
 * @brief 加载listwidget的数据存入vector数组
 * @return
 */
QVector<QString> Widget::loadLWidgetData()
{
    QVector<QString> listData;
    for(int i=0;i<ui->listWidget->count();i++){
        listData<<ui->listWidget->item(i)->text();
    }
    return listData;
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
 * @brief 用于比对要生成的数据是否跟listwidget中的数据重复
 * @param QVector<QString> data
 * @return
 */
bool Widget::checkRepeatInListWidget(QVector<QString> data)
{
    QSet<QString> listData;
    //获取listwidget中的数据
    for(int i=0;i<ui->listWidget->count();i++){
        listData<<ui->listWidget->item(i)->text();
    }
    //查找是否重复
    QSet<QString>::iterator iter;
    for(int i=0;i<data.size();i++)
    {
        if((iter = listData.find(data[i])) != listData.end())
        {
            return false;
        }
    }
    return true;
}
/**
 * @brief 用于判断各种参数的有效性，例如长度，参数是否为空
 * @return 没问题则返回TRUE 有问题则返回FALSE
 */
bool Widget::checkParamValid(QString type)
{
    int lenth = ui->lenSBox->value();               //数据长度
    QString fixStr = ui->fixLEdit->text();          //固定部分
    int changeNum = ui->changeLEdit->text().toInt();//改变部分
    int quantity = ui->quantitySBox->value();
    int fixLen = fixStr.length();
    int changeLen = ui->changeLEdit->text().length();

    if(type == BATCHTYPE){
        if(quantity == 0 ){                              //判断添加数量是否为零
            QMessageBox msgBox;
             msgBox.setText("Quantity is Empty!");
             msgBox.exec();
             return false;
        }
    }
    if(fixStr == NULL || changeNum == NULL){
         QMessageBox msgBox;
         msgBox.setText("参数为空，请确认参数是否填写！");
         msgBox.exec();
         return false;
    }
    if(fixLen + changeLen != lenth){
        QMessageBox msgBox;
        msgBox.setText("长度校验不正确，请谨慎核对！");
        msgBox.exec();
        return false;
    }
    return true;
}

bool Widget::checkSearchValue()
{
    if(ui->mainLEdit->text() == NULL ||ui->inputLEdit->text()==NULL ){
        QMessageBox msgBox;
         msgBox.setText("Path is Empty!pls check again!");
         msgBox.exec();
         return false;
    }
    if(ui->noFoundLEdit->text() == NULL|| ui->outputLEdit->text()==NULL){
        QMessageBox msgBox;
         msgBox.setText("OutPutPath is Empty!pls check again!");
         msgBox.exec();
         return false;
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
           ui->fixLEdit->setFocus();
       }
       if(event->type() == QEvent::MouseButtonPress){

            ui->fixLEdit->setFocus();
          //  focusNextChild();
       }

   }
   if(watched == ui->addOnePBtn){
       if(event->type() == QEvent::KeyPress){
           on_addOnePBtn_clicked();
           ui->fixLEdit->setFocus();
       }
       if(event->type() == QEvent::MouseButtonPress){

           ui->fixLEdit->setFocus();
       }
   }
   return QWidget::eventFilter(watched,event);
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

/**
 * @brief Listwidget双击时用作删除作用的信号槽
 * @param item
 */
void Widget::deleteItemsSlot(QListWidgetItem *item)
{

    int ret = QMessageBox::warning(this, tr("XHRecord"),
                                     tr("The filr has been modified.\n"
                                        "你确定要删除此数据吗?"),
                                     QMessageBox::Yes
                                     | QMessageBox::No,
                                     QMessageBox::Yes);
    if(ret == QMessageBox::Yes){
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
        QString filePath = ui->filePathLEdit->text();
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        for(int i = 0;i<ui->listWidget->count();i++){
            QString mtext = ui->listWidget->item(i)->text();
            out<<mtext<<"\n";
        }
        out.flush();
        file.close();
        loadFileInList(filePath);
    }
    else if (ret == QMessageBox::No) {
        return ;
    }

}
/**
 * @brief 保存按钮的功能实现
 */
void Widget::on_savePBtn_clicked()
{
    QString filePath =ui->filePathLEdit->text();
    if(filePath.isEmpty()){
        filePath = openSaveFile();
    }
    FileUtil::cleanFile(filePath);
    QVector<QString> widgetData =  loadLWidgetData();
    QString listText;
    FileUtil writeFile(filePath);
    if(writeFile.setModel(writeFile.WRITEAPPEND)){
        foreach (listText, widgetData) {
            writeFile.appenWriteFile(listText);
        }
        writeFile.closeFile();
        return ;
    }
    QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
                           "Failed to save data into file", 0, this);
    msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
    msgBox.exec();

}
/**
 * @brief 模糊搜索功能的实现
 * @param test
 */
void Widget::on_lineEdit_textChanged(const QString &test)
{
    for(int i = 0;i<ui->listWidget->count();i++){
        QString value = ui->listWidget->item(i)->text();

        if(value.contains(test)){
            ui->listWidget->setCurrentRow(i);
            break;
        }
    }
}
/**
 * @brief listWidget的菜单槽函数
 * @param pos
 */
void Widget::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *curItem = ui->listWidget->itemAt(pos);
    if(curItem == NULL)
        return;

    popMenu->exec( QCursor::pos() );
//    delete popMenu;
//    delete importSeed;
//    delete cancelSeed;

}
/**
 * @brief 打开输出对话框
 */
void Widget::openImportDialog()
{

    impDialog->exec();
    int row = ui->listWidget->currentRow();

    QVector<QString> listData = loadLWidgetData();
    if(row+importCount>listData.size()){
        QMessageBox msgBox;
        msgBox.setText("提取的行数超出实际行数！");
        msgBox.exec();
        return ;
    }
    QFile file(importPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
        return;
    QTextStream out(&file);
    for(int i=0;i<importCount;i++){
        out<<listData[row+i]<<"\n";
    }

    out.flush();
    file.close();

}

/**
 * @brief DataSearch中的MainButton槽
 */
void Widget::on_mainPBtn_clicked()
{
    QString filepath = "";
    filepath = QFileDialog::getOpenFileName(this,
        tr("选择文件"),
        "D:\\xhRecord",
        tr("File (*.txt *.prd *.inp)")); //选择路径
    if(!filepath.isEmpty())
    {
        ui->mainLEdit->setText(filepath);

    }
    mainVec.clear();
    FileUtil fileUtil(filepath);
    fileUtil.setModel(fileUtil.READONLY);
    mainVec = fileUtil.readFile2Vec();
    fileUtil.closeFile();

}
/**
 * @brief DataSearch中的inputPBtn槽
 */
void Widget::on_inputPBtn_clicked()
{
    QString filepath = "";
    filepath = QFileDialog::getOpenFileName(this,
        tr("选择文件"),
        "D:\\xhRecord",
        tr("File (*.txt *.prd *.inp)")); //选择路径
    if(!filepath.isEmpty())
    {
        ui->inputLEdit->setText(filepath);
    }
    impVec.clear();
    FileUtil fileUtil(filepath);
    fileUtil.setModel(fileUtil.READONLY);
    impVec = fileUtil.readFile2Vec();
    fileUtil.closeFile();
}
/**
 * @brief DataSearch中的noFoundPBtn槽
 */
void Widget::on_noFoundPBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                 "D:\\xhRecord",
                                 tr("File (*.txt *.prd *.inp)"));
    ui->noFoundLEdit->setText(fileName);
}
/**
 * @brief DataSearch中的outputPBtn槽
 */
void Widget::on_outputPBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                 "D:\\xhRecord",
                                 tr("File (*.txt *.prd *.inp)"));
    ui->outputLEdit->setText(fileName);
}

/**
 * @brief Search按钮槽
 */
void Widget::on_searchPBtn_clicked()
{
    if(!checkSearchValue()){
        return ;
    }
    FileUtil outFile(ui->outputLEdit->text());
    FileUtil notFoFile(ui->noFoundLEdit->text());
    outFile.setModel(outFile.WRITEAPPEND);
    notFoFile.setModel(notFoFile.WRITEAPPEND);
    int lineNum = ui->outputSBox->value();
    QString mainString,impString;

    ui->progressBar->setMaximum(impVec.size());
    ui->progressBar->setValue(0);
    int process = 0;
    bool hasFound ;
    foreach (impString, impVec) {
        process++;
        hasFound = false;
        foreach (mainString, mainVec) {
            if(mainString == impString){
                hasFound = true;
                int index = mainVec.indexOf(mainString);
                for(int i = 0;i<lineNum;i++){
                    QString value = mainVec[index+i];
                    outFile.appenWriteFile(value);
                }
            }
        }
        if(!hasFound){
            notFoFile.appenWriteFile(impString);
        }
        ui->progressBar->setValue(process);
    }

}
