#include "treeview.h"
treeView::treeView(QWidget *p):QTreeView(p)
{
   // init();do
   setSelectionMode(QAbstractItemView::SingleSelection);

}
//=======================
treeView::~treeView()
{

}

//=======================
void treeView::init()
{
    model = NULL;
    selectionModel =NULL;   
    //model = new TreeModel();
    model = new QStandardItemModel(1,3);
    setModel(model); 
    refresh();
    createActions();
}
void treeView::createActions()
{
    createProjectAct = new QAction(QIcon(":/images/createproject.png"),tr("&create Project"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    createProjectAct->setStatusTip(tr("Create a new Project"));
    connect(createProjectAct, SIGNAL(triggered()), this, SLOT(slotCreateProject()));

    createLineAct = new QAction(QIcon(":/images/createline.png"),tr("&create Line"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    createLineAct->setStatusTip(tr("Create a new Line"));
    connect(createLineAct, SIGNAL(triggered()), this, SLOT(slotCreateLine()));

    removeProjectAct = new QAction(QIcon(":/images/removeproject.png"),tr("&remove Project"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    removeProjectAct->setStatusTip(tr("Remove a new Project"));
    connect(removeProjectAct, SIGNAL(triggered()), this, SLOT(slotRemoveProject()));

    removeLineAct = new QAction(QIcon(":/images/removeline.png"),tr("&remove Line"), this);
    //createProjectAct->setShortcuts(QKeySequence::New);
    removeLineAct->setStatusTip(tr("Remove a  Line"));
    connect(removeLineAct, SIGNAL(triggered()), this, SLOT(slotRemoveLine()));
}
void treeView::refresh()
{
    string name,user,date;
    int i,j,id; 
    int num,lnum;
    QString str;
    QStandardItem *item,*itemc;
    QIcon icon(":/images/project.png");
    QIcon iconl(":/images/line.png");

  
    num =  data->m_project.numProjects();
    qDebug() << "project num = " << num << &(data->m_project);
 
    model->clear();
    model->setRowCount(0);
    model->setColumnCount(3);

    //header:
    model->setHeaderData(0, Qt::Horizontal, tr("project"));
    model->setHeaderData(1, Qt::Horizontal, tr("User"));
    model->setHeaderData(2, Qt::Horizontal, tr("date"));

    PROJECTList plist;
    LINEList llist;
    QModelIndex idx;

    str = "";
    //int ic = 0;
    for (i = 0; i <num ;i++)
    {
        plist = data->m_project.getProjectList();
        name = plist[i].name;
        user = plist[i].user;
        date = plist[i].date;
        //str += QString(name.c_str()) + "\t" + QString(user.c_str()) + "\t" + QString(date.c_str())+ "\n";
        model->insertRow(i,QModelIndex());
        model->setData(model->index(i,0,QModelIndex()),name.c_str());
        model->setData(model->index(i,0,QModelIndex()),icon,Qt::DecorationRole);
        model->setData(model->index(i,1,QModelIndex()),user.c_str());
        model->setData(model->index(i,2,QModelIndex()),date.c_str());

        idx = model->index(i,0,QModelIndex());// nouse 
        item = model->itemFromIndex(idx);// item is important
        //ic++;

        data->m_project.setProject((char *)name.c_str());
        llist = data->m_project.getLineList();  
        lnum = llist.size();
        //qDebug() << "idx = " << idx.row() << idx.column();
        if (lnum > 0)
        {
            for (j = 0; j < lnum ; j++) 
            {
                name = llist[j].name;
                user = llist[j].user;
                date = llist[j].date;

                itemc = new QStandardItem();
                itemc->setData(name.c_str(),Qt::DisplayRole);
                itemc->setData(iconl,Qt::DecorationRole);
                        //model->setData(model->index(i,0,QModelIndex()),icon,Qt::DecorationRole);
                //itemc->setText(name.c_str());// yes
                item->setChild(j,0,itemc);// works,only in this way to create child

                itemc = new QStandardItem();
                itemc->setData(user.c_str(),Qt::DisplayRole);
                item->setChild(j,1,itemc);

                itemc = new QStandardItem();
                itemc->setData(date.c_str(),Qt::DisplayRole);
                item->setChild(j,2,itemc); 
                // cannot delete itemc;
       
                // model->insertRow(j+3,itemc);// works but insert the rootItems
                //id =model->insertRow(j,idx);// it cannot work,donot make child;
 
            }
        } 
 
    }
  
  
   // qDebug() << "set mod =" << selectionModel;
    if(selectionModel!= NULL)  delete selectionModel;
    //qDebug() << " delete";
    selectionModel = new QItemSelectionModel(model);
    //qDebug() << " nete";
    setSelectionModel(selectionModel);
    //qDebug() << "set selectmod";
 
}

void treeView::setSelect(QString p)
{
    int num,i;
    QString str;
    QModelIndex idx;
    num = model->rowCount();
    for (i = 0; i <num;i++) 
    {
        idx = model->index(i,0);
        str = idx.data().toString();
        if (str == p) 
        {
            //qDebug() << "yes ";
            selectionModel->select(idx,QItemSelectionModel::Select);
        }
    }

}
void treeView::setSelect(QString p,QString l)
{
    int num,i,ic;
    QString str,strl;
    QModelIndex idx,idxl;
    num = model->rowCount();
    ic = 0;
    for (i = 0; i <num;i++) 
    {
        idx = model->index(i,0);
        str = idx.data().toString();
        if (str == p) 
        {
            //qDebug() << "yes. is the project ";
            //selectionModel->select(idx,QItemSelectionModel::Select);
            setExpanded(idx,true);
            while (1)
            {
                idxl = idx.child(ic, 0); 
                if (idxl.isValid()) 
                {
                    strl = idxl.data().toString();
                    if (strl == l) 
                    {
                        selectionModel->select(idxl,QItemSelectionModel::Select);
                        break;
                    }
                }
                else
                {
                    break;
                }
                ic++;
            }
        }
    }

}

//=======================
void treeView::setData(gdata *d)
{
    data = d;
    return;
}
void treeView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    QTreeView::selectionChanged(selected, deselected);
    QModelIndex index,pindex;
    QString project,line;

    if (selected.indexes().size() == 0 ) return;
    index = selected.indexes().at(0);
    pindex = index.parent();
    if (pindex== QModelIndex()) 
    {
        //qDebug() << "project ="<< index.data().toString();
        project = index.data().toString();
        data->m_project.setProject(project.toUtf8().data());
        emit selectedp();
    }
    else
    {
        project = pindex.data().toString();
        line = index.data().toString();
        data->m_project.setProject(project.toUtf8().data());
        data->m_project.setLine(line.toUtf8().data());
        emit selectedp();

        //qDebug() << "line ="<< pindex.data().toString() <<"   " << index.data().toString();
    }
}
//=======================
void treeView::contextMenuEvent(QContextMenuEvent * ev )
{
     QMenu menu(this);
     qDebug() << "menu ";
     menu.addAction(createProjectAct);
     menu.addAction(createLineAct);
     menu.addAction(createLineAct);
     menu.addSeparator();
     menu.addAction(removeProjectAct);
     menu.addAction(removeLineAct);
     menu.exec( ev->globalPos() );
  
}
void treeView::slotCreateProject()
{
    emit sigCreateProject();
}
void treeView::slotRemoveProject()
{
    emit sigRemoveProject();
}
void treeView::slotCreateLine()
{
    emit sigCreateLine();
}
void treeView::slotRemoveLine()
{
    emit sigRemoveLine();
}
//bool treeView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) 
//{
 //   return true;
//}     
