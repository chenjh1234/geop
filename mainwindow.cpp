 
#include <QtWidgets>

#include "mainwindow.h"
#include "listview.h"
#include "treeview.h"
#include <QStandardItemModel>
#include <QModelIndex>
 
MainWindow::MainWindow():QMainWindow()
{
    
    treeDockWidget = new QDockWidget(this);
    treeDockWidget->setObjectName("treeWidget");
    infoDockWidget = new QDockWidget(this);
    infoDockWidget->setObjectName("textView");
    addDockWidget(Qt::LeftDockWidgetArea, treeDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, infoDockWidget);

    textView = new QTextBrowser(this);
    textView->setMinimumWidth(300);
    infoDockWidget->setWidget(textView);
     
    m_data = new gdata();
        
    split = new QSplitter(this);
    setCentralWidget(split);
 
    tabWidget = new QTabWidget();

    listFlow = new listView(tabWidget);
    listLog = new listView(tabWidget);
    listJob = new listView(tabWidget);
    listData = new listView(tabWidget);
    listTable = new listView(tabWidget);

    tabWidget->addTab(listFlow,"Flow");
    //    tabWidget->addTab(listFlow,"Flow");
    tabWidget->addTab(listLog,"Log");
    tabWidget->addTab(listJob,"Job");
    tabWidget->addTab(listData,"Data");
    tabWidget->addTab(listTable,"Table");

    split->addWidget(tabWidget);

    treeProject = new treeView(this);
   // split->addWidget(treeProject);

    treeProject->setMinimumWidth(300);
    treeDockWidget->setWidget(treeProject);

 
    treeProject->setData(m_data);
    treeProject->init();

    listFlow->setData(m_data);
    listJob->setData(m_data);
    listData->setData(m_data);
    listLog->setData(m_data);
    listTable->setData(m_data);

    listFlow->m_file = new  flowMng();
    listLog->m_file = new  logMng();
    listJob->m_file = new  jobMng();
    listData->m_file = new  dataMng();
    listTable->m_file = new  tableMng();


    listFlow->init();
    listJob->init();
    listData->init();
    listLog->init();
    listTable->init();
 
    listFlow->show();
    listJob->show();
    listData->show();
    listLog->show();
    listTable->show();
    //initMod();
    dlgCreate = new createpDlg(this);
    dlgCreate->setData(m_data);
 

    connect(treeProject, SIGNAL(selectedp()), this, SLOT(slotRefreshAll()));
    connect(treeProject, SIGNAL(sigCreateProject()), this, SLOT(slotCreateProject()));
    connect(treeProject, SIGNAL(sigRemoveProject()), this, SLOT(slotRemoveProject()));
    connect(treeProject, SIGNAL(sigCreateLine()), this, SLOT(slotCreateLine()));
    connect(treeProject, SIGNAL(sigRemoveLine()), this, SLOT(slotRemoveLine()));

    connect(listFlow, SIGNAL(sigDisplay(QString,QString)), this, SLOT(slotShowText(QString,QString)));
    connect(listJob, SIGNAL(sigDisplay(QString,QString)), this, SLOT(slotShowText(QString,QString)));
    connect(listData, SIGNAL(sigDisplay(QString,QString)), this, SLOT(slotShowText(QString,QString)));
    connect(listLog, SIGNAL(sigDisplay(QString,QString)), this, SLOT(slotShowText(QString,QString)));
    connect(listTable, SIGNAL(sigDisplay(QString,QString)), this, SLOT(slotShowText(QString,QString)));

    connect(listFlow, SIGNAL(sigFiles(int)), this, SLOT(slotShowFiles(int)));
    connect(listJob, SIGNAL(sigFiles(int)), this, SLOT(slotShowFiles(int)));
    connect(listData, SIGNAL(sigFiles(int)), this, SLOT(slotShowFiles(int)));
    connect(listLog, SIGNAL(sigFiles(int)), this, SLOT(slotShowFiles(int)));
    connect(listTable, SIGNAL(sigFiles(int)), this, SLOT(slotShowFiles(int)));




    setWindowTitle(QObject::tr(APP_NAME));
     
    createActions();

 
    
    createMenus();
    createToolBars();
    createStatusBar();
     
    readSettings();
   
return;  
   

    //connect(textEdit->document(), SIGNAL(contentsChanged()),
    //        this, SLOT(documentWasModified()));

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}
void MainWindow::initMod()
{
    int sz,i;
    bool b;
    string s;
     
    listLog->m_file->setProject((char *)"myproject",(char *)"line2");
    fileInfo *fp;
   // if (m_file->m_config != "log")  return;
    sz = listLog->m_file->size();
    QStandardItemModel *model;
    model = new QStandardItemModel(1,4,parent());
    
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("User"));
    model->setHeaderData(2, Qt::Horizontal, tr("Size"));
    model->setHeaderData(3, Qt::Horizontal, tr("Time"));

//    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
  
    for (i = 0 ; i <sz ; i++) 
    {
       fp =  (fileInfo *)(listLog->m_file->get(i));
       cout << fp->name <<endl;
       cout << fp->path <<endl;
       cout << fp->size <<endl;
       //cout << fp->time <<endl;
       //cout << fp->user <<endl;
       model->insertRow(i,QModelIndex());
       model->setData(model->index(i,0,QModelIndex()),fp->name.c_str());
       model->setData(model->index(i,1,QModelIndex()),fp->user.c_str());
       s =flexVar(fp->size).toString(); 
       model->setData(model->index(i,2,QModelIndex()),s.c_str());
       model->setData(model->index(i,3,QModelIndex()),fp->time.c_str());

    }
    listLog->setModel(model);
   // QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    //setSelectionModel(selectionModel);
  
}

void MainWindow::closeEvent(QCloseEvent *event)

{
    //cjh
 
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()

{
    if (maybeSave()) {
        //textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()

{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()

{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()

{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
void MainWindow::slotCreateProject()
{
    int i,id;
    char *ch;
    static QString str;
    QString str1;
//
    //str = "myproject";
    //str1 = "line2";
    //treeProject->setSelect(str,str1);
    //return;
    //

    dlgCreate->setProject();
    i = dlgCreate->exec();
    if (i > 0) //ok
    { 
        str= dlgCreate->getName();
        //qDebug() << "create project0 = " <<str << str.toUtf8().data();
        //ch = str.toUtf8().data();
        //qDebug() << "create project1 = " << ch ;
        //return;
        id = m_data->m_project.createProject(str.toUtf8().data());
        if (id !=0) 
        {
            str = "Create Project Error ret = " + str.setNum(id);
            QMessageBox::warning(this,"Error",str);
            return;
        }
        //ok:
        treeProject->refresh();//refresh project construct
        treeProject->setSelect(str);//select it
    }
    //qDebug() << "slot create Project return = " << i;
    

}
 
void MainWindow::slotCreateLine()
{
    int i,id;
    char *ch;
    static QString str,strp;

    i = dlgCreate->setLine();
    if (i <0) return;//no project selected

    strp = m_data->m_project.getCurrentProject().c_str();
   
    i = dlgCreate->exec(); 
    if (i > 0) 
    { 
        str= dlgCreate->getName();//str in  lineedit
        //ch = str.toUtf8().data();
        id = m_data->m_project.createLine(str.toUtf8().data());
        if (id !=0) 
        {
            str = "Create Line Error ret = " + str.setNum(id);
            QMessageBox::warning(this,"Error",str);
            return;
        }
        //ok:
        //qDebug() << "creat ok";
        treeProject->refresh();//refresh project construct
        //qDebug() << "refresh ";
        treeProject->setSelect(strp,str);//selectir
         //qDebug() << "select ";
        //m_data->m_project.createProject()
    }
    //qDebug() << "slot create Line return = " << i;
}
void MainWindow::slotRemoveProject()
{
    QString qstr;
    int i;
    string project;
    project = m_data->m_project.getCurrentProject();
    if (project.empty())
    {
        QMessageBox::warning(this,"Error","No project selected");    
        return;
    }
    qstr = "Remove project: ";
    qstr = qstr + project.c_str();
    i = QMessageBox::warning(this,"warning",qstr,QMessageBox::Ok,QMessageBox::Cancel); 
    if (i !=QMessageBox::Ok) 
    {
        qDebug() << " canceld by user";
        return;
    }

    //remove
    i = m_data->m_project.removeProject((char *)project.c_str());
    if (i !=0) 
    {
        qstr = "Remove Project =" ;
        qstr = qstr + project.c_str() + " Error!!!";
        QMessageBox::warning(this,"Error",qstr);    
        return;
    }
    //remove ok:
    qDebug() <<" remove project ok";
    treeProject->refresh();
}
void MainWindow::slotRemoveLine()
{
    QString qstr;
    int i;
    string project,line;
    project = m_data->m_project.getCurrentProject();
    line = m_data->m_project.getCurrentLine();
    if (project.empty())
    {
        QMessageBox::warning(this,"Error","No project selected");    
        return;
    }
    if (line.empty())
    {
        QMessageBox::warning(this,"Error","No project Line selected");    
        return;
    }
    qstr = "Remove Line: ";
    qstr = qstr + project.c_str() +"-" +line.c_str();
    i = QMessageBox::warning(this,"warning",qstr,QMessageBox::Ok,QMessageBox::Cancel); 
    if (i !=QMessageBox::Ok) 
    {
        qDebug() << " canceld by user";
        return;
    }

    //remove
    i = m_data->m_project.removeLine((char *)line.c_str());
    if (i !=0) 
    {
        qstr = "Remove Line =" ;
        qstr = qstr + project.c_str() +"-" +line.c_str() + " Error!!!";
        QMessageBox::warning(this,"Error",qstr);    
        return;
    }
    //remove ok:
    qDebug() <<" remove Line ok";
    treeProject->refresh();
}
void MainWindow::about()
{
    QString title,str,str1;
    title = ORG_NAME;
    str1 = __DATE__;
    str = QString("   App name : ") +QString(APP_NAME) + "\n" +
          QString("App version : ") +QString(APP_VERSION) + "\n" +
          QString("       Date : ") + str1;
   QMessageBox::about(this, title,str);
}

void MainWindow::documentWasModified()
{
    //setWindowModified(textEdit->document()->isModified());
}
//! [16]

//! [17]
void MainWindow::createActions()
{
    createProjectAct = new QAction(QIcon(":/images/createproject.png"), tr("create Project"), this);
    //newAct->setShortcuts(QKeySequence::New);
    createProjectAct->setStatusTip(tr("create Project"));
    connect(createProjectAct, SIGNAL(triggered()), this, SLOT(slotCreateProject()));

    removeProjectAct = new QAction(QIcon(":/images/removeproject.png"), tr("remove Project"), this);
    //newAct->setShortcuts(QKeySequence::New);
    removeProjectAct->setStatusTip(tr("remove Project"));
    connect(removeProjectAct, SIGNAL(triggered()), this, SLOT(slotRemoveProject()));

 
    createLineAct = new QAction(QIcon(":/images/createline.png"), tr("createLine"), this);
    //createLineAct->setShortcuts(QKeySequence::Open);
    createLineAct->setStatusTip(tr("create Line"));
    connect(createLineAct, SIGNAL(triggered()), this, SLOT(slotCreateLine()));
 

    removeLineAct = new QAction(QIcon(":/images/removeline.png"), tr("removeLine"), this);
    //createLineAct->setShortcuts(QKeySequence::Open);
    removeLineAct->setStatusTip(tr("remove Line"));
    connect(removeLineAct, SIGNAL(triggered()), this, SLOT(slotRemoveLine()));

 
 
    exitAct = new QAction(tr("e&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
 
    exitAct->setStatusTip(tr("exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  
    aboutAct = new QAction(QIcon(":/images/about.png"),tr("&about"), this);
    aboutAct->setStatusTip(tr("show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    helpAct = new QAction(QIcon(":/images/help.png"),tr("&help"), this);
    helpAct->setStatusTip(tr("show the projectPad's maunal"));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(slotHelp()));

      
    refreshAct = new QAction(QIcon(":/images/refresh.png"),tr("&refresh"), this);
    refreshAct->setStatusTip(tr("refresh all the directorys"));
    connect(refreshAct, SIGNAL(triggered()), this, SLOT(slotRefreshAll()));


    xtermAct = new QAction(QIcon(":/images/xterm.png"),tr("&Xterm"), this);
    xtermAct->setStatusTip(tr("xterm to the selected project directory"));
    connect(xtermAct, SIGNAL(triggered()), this, SLOT(slotXterm()));

    //connect(textEdit, SIGNAL(copyAvailable(bool)),
    //        cutAct, SLOT(setEnabled(bool)));
    //connect(textEdit, SIGNAL(copyAvailable(bool)),
    //        copyAct, SLOT(setEnabled(bool)));
}
 
void MainWindow::createMenus()
 
{
    fileMenu = menuBar()->addMenu(tr("&Project"));
    fileMenu->addAction(createProjectAct);
    fileMenu->addAction(createLineAct);
    
    fileMenu->addSeparator();
    fileMenu->addAction(removeProjectAct);
    fileMenu->addAction(removeLineAct);

    fileMenu->addSeparator();
    fileMenu->addAction(refreshAct);
     fileMenu->addSeparator();

    fileMenu->addAction(exitAct);
 
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
    qDebug() << "create menu";
    
}
 
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("ToolBar"));
    fileToolBar->addAction(createProjectAct);
    fileToolBar->addAction(createLineAct);
    fileToolBar->addAction(refreshAct);
    fileToolBar->addAction(xtermAct);
    fileToolBar->addAction(helpAct);
     qDebug() << "create toolBar";
  
}
 
void MainWindow::createStatusBar()
 
{
    statusBar()->showMessage(tr("Ready"));
}
 
void MainWindow::readSettings()
{
    QSettings settings(ORG_NAME, APP_NAME);
    QString project,line;
    project = settings.value("project").toString();
    line = settings.value("line").toString();

    qDebug() << "read setting = = " << project <<line;

    if (project.isEmpty()) return;
    
    if (!line.isEmpty()) 
        treeProject->setSelect(project,line);
    else
        treeProject->setSelect(project);
        
}
 
void MainWindow::writeSettings()
{
    QString project,line;
    QSettings settings(ORG_NAME, APP_NAME);
    project = m_data->m_project.getCurrentProject().c_str();
    settings.setValue("project", project);

    line = m_data->m_project.getCurrentLine().c_str();
    settings.setValue("line", line);
    qDebug() << "close window = " << project<< line;
}
 
bool MainWindow::maybeSave()
{
    /*
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }*/
    return true;
}
 
void MainWindow::loadFile(const QString &fileName)
{
    QString str;
    str = fileName;
}
 
bool MainWindow::saveFile(const QString &fileName)
{
    QString str;
    str = fileName;
    return true;
}
 
void MainWindow::setCurrentFile(const QString &fileName)
 
{
     QString str;
    str = fileName;
}
 
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
void MainWindow::slotRefreshAll()
{
    //settitle:
    QString str,str1;
    str1 =  QString(m_data->m_project.getProjectHome())+ ":";
    str = str1 +m_data->m_project.getCurrentProject().c_str();
    str = str + " : " +m_data->m_project.getCurrentLine().c_str();
    treeDockWidget->setWindowTitle(str);

    qDebug()<< "refresh all ";
    listFlow->refresh();
    listJob->refresh();
    listData->refresh();
    listLog->refresh();
    listTable->refresh();

    listFlow->show();
    listJob->show();
    listData->show();
    listLog->show();
    listTable->show();
}
void MainWindow::slotShowText(QString path,QString str)
{
 
    infoDockWidget->setWindowTitle(path);
    textView->setText(str);
}
void MainWindow::slotShowFiles(int num)
{
    QString str;
    str =  QString("Selected files %1").arg(num);
    statusBar()->showMessage(str);
}
void MainWindow::slotXterm()
{
    QString path,qstr;
    QString cmd;
    string str;
    QString project,line;
    project = m_data->m_project.getCurrentProject().c_str();
    line = m_data->m_project.getCurrentLine().c_str();
    if (line.isEmpty()) 
    {
        str = m_data->m_project.getProjectHome(project.toUtf8().data()) ;
    }
    else
    {
        str =  m_data->m_project.getLineHome(line.toUtf8().data()) ;
    }
    qstr = str.c_str();
    //qDebug() << "dir= " <<qstr;
    cmd = "xterm" ;
    run.setWorkingDirectory(qstr);
    run.start(cmd);
 
}
void MainWindow::setTitle()
{
  
    QString str;
    str = "";
    str = str + APP_NAME + "-"+APP_VERSION +"-"+ __DATE__ ;//+"-";
    //str = str + theApp->m_doc->m_filename + ":" + theApp->m_doc->m_project +":" + theApp->m_doc->m_line;

    setWindowTitle(str);
    qDebug() << "title = " << str;

}
void MainWindow::slotHelp()
{
    QString cmd,str;
    //char *ch;
    cmd = "showpdf ";
    //qDebug() << "ddd";
    if (getDocDir() == NULL)
    {
        qDebug() << "No doc directoy";
        return;
    }
    cmd = cmd + getDocDir() + SLASH + "projectpad.pdf";
        //cmd = "ls";
    qDebug() << "cmd=" <<cmd;
    run.start(cmd);

}
