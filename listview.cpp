#include <listview.h>


listView::listView(QWidget *p):QTreeView(p)
{
// we donot init here 
    //setSelectionMode(QAbstractItemView::SingleSelection);
    //setSelectionMode(QAbstractItemView::MultiSelection);
    //setSelectionMode(QAbstractItemView::ContiguousSelection);
   setSelectionMode(QAbstractItemView::ExtendedSelection);
    
}
//=======================
listView::~listView()
{

}

//=======================
void listView::init()
{
   //in main window: after 1:setdata 2:new m_file,3:than init

    model = new QStandardItemModel(1,4,parent());
    createActions();
    refresh();
}
void listView::createActions()
{
    removeFileAct = new QAction(tr("&removeFile"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    removeFileAct->setStatusTip(tr("remove File"));
    connect(removeFileAct, SIGNAL(triggered()), this, SLOT(slotRemoveFile()));


    displayFileAct = new QAction(tr("&displayFile"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    displayFileAct->setStatusTip(tr("display File"));
    connect(displayFileAct, SIGNAL(triggered()), this, SLOT(slotDisplayFile()));


    editFileAct = new QAction(tr("&editFile"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    editFileAct->setStatusTip(tr("edit File"));
    connect(editFileAct, SIGNAL(triggered()), this, SLOT(slotEditFile()));

    copyToAct = new QAction(tr("&copyTo"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    copyToAct->setStatusTip(tr("copy selected files to other directory"));
    connect(copyToAct, SIGNAL(triggered()), this, SLOT(slotCopyTo()));

    copyFromAct = new QAction(tr("&copyFrom"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    copyFromAct->setStatusTip(tr("copy from other directorys to this form"));
    connect(copyFromAct, SIGNAL(triggered()), this, SLOT(slotCopyFrom()));

     renameAct = new QAction(tr("&rename"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    renameAct->setStatusTip(tr("Rename "));
    connect(renameAct, SIGNAL(triggered()), this, SLOT(slotRename()));
}

void listView::refresh()
{
    //return;
    int sz,i;
    bool b;
    string project,line;
    QIcon icon(":/images/file.png");
    char *p,*l;
    model->clear();
    model->setRowCount(0);
    model->setColumnCount(4);

    //header:
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("User"));
    model->setHeaderData(2, Qt::Horizontal, tr("Size"));
    model->setHeaderData(3, Qt::Horizontal, tr("Time"));
// get files:
    project = data->m_project.getCurrentProject();
    line = data->m_project.getCurrentLine();
    if (project.empty()) return;
    p = (char *)project.c_str();
    l = (char *)line.c_str();
    if (line.empty())
    {
        m_file->setProject(p);
    }
    else
        m_file->setProject(p,l);
     
    fileInfo *fp;
    sz = m_file->size();

//    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
  
    for (i = 0 ; i <sz ; i++) 
    {
       fp =  (fileInfo *)(m_file->get(i));
       cout << fp->name <<endl;
       cout << fp->path <<endl;
       cout << fp->size <<endl;
       //cout << fp->time <<endl;
       //cout << fp->user <<endl;
       model->insertRow(i,QModelIndex());
       model->setData(model->index(i,0,QModelIndex()),fp->name.c_str());
       model->setData(model->index(i,0,QModelIndex()),icon,Qt::DecorationRole);
       model->setData(model->index(i,1,QModelIndex()),fp->user.c_str());
      //s =flexVar(fp->size).toString(); 
       //model->setData(model->index(i,2,QModelIndex()),s.c_str());
       model->setData(model->index(i,2,QModelIndex()),fp->size);
       model->setData(model->index(i,3,QModelIndex()),fp->time.c_str());

    }
    //pmodel = new QSortFilterProxyModel;
    //pmodel->setDynamicSortFilter(true);
    //pmodel->setSourceModel(model);

    setModel(model);
    setSortingEnabled(true);
    setEditTriggers(0);
    //update();
   // setModel(model);
   // QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    //setSelectionModel(selectionModel);
}

//=======================
void listView::setData(gdata *d)
{
    data = d;
    return;
}
QString listView::getConfig()
{
    string str;
    QString qstr;
    str = m_file->getConfig();
    qstr = str.c_str();
    return qstr;
}

void listView::contextMenuEvent(QContextMenuEvent * ev )
{
     QMenu menu(this);
     qDebug() << "menu ";


     menu.addAction(displayFileAct);
     menu.addAction(editFileAct);
     menu.addSeparator();
     menu.addAction(removeFileAct);
     menu.addSeparator();
     menu.addAction(copyToAct);
     menu.addAction(copyFromAct);

     //menu.addAction(renameAct);
  
     menu.exec( ev->globalPos() );
  
}
void listView::slotDisplayFile()
{
    QString path,str,filetype;
    qDebug() << " this is slotDisplay";
    filetype = m_file->getConfig().c_str();
    qDebug() << " filetype =" << filetype;
    if (m_selected.isEmpty()) return;
    if (filetype == "data") return;
 
     
    //path = m_selected.first();
    path = m_selected.constBegin().value();
    //qDebug() << "Display path = " <<path;

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr(" "),
                         tr("Cannot read file %1:\n%2.")
                         .arg(path)
                         .arg(file.errorString()));
        return;
    }
    str = file.readAll();
    file.close();
    emit sigDisplay(path,str);
 
}
void listView::slotEditFile()
{
    QString path,str;
    QString filetype,cmd;
    QString project,line;
    project = data->m_project.getCurrentProject().c_str();
    line = data->m_project.getCurrentLine().c_str();


    filetype = m_file->getConfig().c_str();
    if (m_selected.isEmpty()) return;
    //path = m_selected.first();
    path = m_selected.constBegin().value();
    
    if (filetype == "flow") 
    {
        //cmd = "jobp " +path;
        cmd = QString("jobp ") + "\"" + path +"\"";// for win test
        cmd = cmd + " " + project+ " " +line;
        //cmd = "ls";
        //qDebug() << "cmd=" <<cmd;
        run.start(cmd);
        //qDebug() <<"err = " << run.readAllStandardError();
        //qDebug() <<"  out =" << run.readAllStandardOutput();
         
    }
    if (filetype == "data") 
    {
        cmd = QString("dataview ") + "\"" + path +"\"";
        //cmd = cmd + " " + project+ " " +line;
        //cmd = "ls";
        //qDebug() << "cmd=" <<cmd;
        run.start(cmd);
        //qDebug() <<"err = " << run.readAllStandardError();
        //qDebug() <<"  out =" << run.readAllStandardOutput();

    }
}
QString  listView::getTypeStr(QString type)
{
    QString project,line,qstr,config;
    QString filter, target;
    string str;

    config = m_file->getConfig().c_str();

    project = data->m_project.getCurrentProject().c_str();
    line = data->m_project.getCurrentLine().c_str();
    if (project.isEmpty()) return "./";
    if (line.isEmpty()) 
    {
        str = data->m_project.getProjectHome(project.toUtf8().data());
    }
    else
        str = data->m_project.getLineHome(line.toUtf8().data());

    qstr = str.c_str();
    target = qstr;



    if (config == "log")  
    {
        filter = "LOG (*.log *.LOG);;All files (*.* *)";
        target = qstr + SLASH +"log";
    }
    if (config == "flow")
    {
        filter = "Flows (*.flow *.FLOW);;All files (*.* *)";
        target = qstr + SLASH + "flow";
    }
    if (config == "data")  
    {
        filter = "Seis Data (*.cseis *.CSEIS);;All files (*.* *)";
        target = qstr + SLASH + "data";
    }
    if (config == "table")  
    {
        filter = "Table (*.tb *.TB);;All files (*.* *)";
        target = qstr +SLASH + "table";
    }
    if (config == "job")  
    {
        filter = "Job(*.job *.JOB);;All files (*.* *)";
        target = qstr +SLASH +"job";
    }

    if (type == "filter") 
        return filter;

    if (type == "dir") 
        return target; 

    return target;
}

void listView::slotCopyFrom()
{
    QString caption,dir,filter,path,target,name;
    QStringList flist;
    int num,i;
    //getSelections();

    caption = "copy files from";
    dir = "";
    target = getTypeStr(QString("dir"));
    filter = getTypeStr(QString("filter"));
     
    flist	= QFileDialog::getOpenFileNames ( this, caption,target,filter);
    num = flist.size();
    if (num == 0) return;
    bool b;


    for (i = 0; i < num ; i++) 
    {
        path = flist[i];
        name = QFileInfo(path).fileName();
        //qDebug() << "source = " <<path;
        //qDebug() << "target = " <<target;
        b = QFile::copy(path,target+SLASH+name);// we have to give the name;
        //qDebug() << "copy = " <<b;
    }
    refresh();


}
void listView::slotRename()
{
}
void listView::slotCopyTo()
{
    QString caption,dir,filter,path,target,name,ret;
    caption = "Select a directory to copy files to"; 
    dir = "";
    dir = getTypeStr(QString("dir"));
    target = QFileDialog::getExistingDirectory(this, caption,dir);

    getSelections();

    if (m_selections.isEmpty()) return;// nothing selected
    if(target.isEmpty()) return;
  
     bool b;

    QMap<QString, QString>::const_iterator i = m_selections.constBegin();
    while (i != m_selections.constEnd()) 
    {
        path = i.value();
        name = i.key();
        b = QFile::copy(path,target+SLASH+name);
        ++i;
    }
}
void listView::slotRemoveFile()
{
    QString path,str,qstr;
    int id,num;
    getSelections();

    if (m_selections.isEmpty()) return;
    num = m_selections.count();
    qstr = "Do you really want to remove files number = " +str.number(num);
 
    id = QMessageBox::warning(this,"Remove files",qstr,QMessageBox::Ok,QMessageBox::Cancel); 
    if (id !=QMessageBox::Ok) 
    {
        qDebug() << " canceld by user";
        return;
    }

    QMap<QString, QString>::const_iterator i = m_selections.constBegin();
    while (i != m_selections.constEnd()) 
    {
        path = i.value();
        QFile::remove(path);
        ++i;
    }
  
    refresh();

}
void listView::mouseDoubleClickEvent ( QMouseEvent * event )
{
    QTreeView::mouseDoubleClickEvent(event);
    slotEditFile();
}
void listView::getSelections()
{
    int i,num;
    fileInfo *fi;
    string s;
    QString file,path,file1;
    QModelIndex idx;
    QModelIndexList	slist,rlist;

    m_selections.clear();
    slist = selectedIndexes();
    num = slist.size();

    for (i = 0; i < num; i++) 
    {
        idx = slist.at(i);
        if (idx.column()==0 ) 
        {
            file = idx.data().toString();
            fi = (fileInfo *)m_file->get((char *)file.toUtf8().data());
            path = fi->path.c_str();
            m_selections[file] = path;
            //qDebug() << " selection = " <<file <<path;
        }
    }

}
void listView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
   
    QTreeView::selectionChanged(selected, deselected);
 
    int num,i;
    fileInfo *fi;
    string s;

    QModelIndex index;
    QString file,path,file1;

    num = selectedIndexes().size()/4;
    emit sigFiles(num);
    qDebug() << "selected files =" <<num;

    m_selected.clear();
    num = selected.size();// is new selected uselly num = 1;
    qDebug() << "new selected =" <<num;
    // num = selected.count();
    //qDebug() << "selected count=" <<num << selectedIndexes().size();
    if (num == 0 ) return;
    for (i = 0; i <num ; i++) 
    { 
        //qDebug() << "selected i=" <<i;   
        index = selected.indexes().at(i); 
        qDebug() << "selected i=" <<i;
        //if (!index.isValid()) break;
        
        file = index.data().toString();
        qDebug() << "selected file =" << file;
        //s =file.toUtf8().data();
        //id = m_file->find((char *)s.c_str());
        fi = (fileInfo *)m_file->get((char *)file.toUtf8().data());
        //fi = (fileInfo *)m_file->get(id);
        

        //qDebug() << "find = " <<m_file->find((char *)s.c_str()) <<s.c_str();
        if (fi == NULL) break;
        qDebug() << "fi=" << fi; 
        path = fi->path.c_str();
        m_selected[file] = path;
        qDebug() << "selected =" <<file << path <<fi->name.c_str();
        slotDisplayFile();
    }

    //getSelections();
}

