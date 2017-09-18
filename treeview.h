#ifndef TREEVIEW_H 
#define TREEVIEW_H 
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QContextMenuEvent>

//#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QAction>
#include <QMenu>
#include "gdata.h"
//#include "treemodel.h"

class treeView :public QTreeView
{
    Q_OBJECT

public:
    treeView(QWidget *p);
    ~treeView();
    void init();//leave it public
    void refresh();
    //void refresh1();
    gdata *data;
    QStandardItemModel *model;
    //TreeModel *model;
    void setData(gdata *d);
    void setSelect(QString p);
    void setSelect(QString p,QString l);
    QItemSelectionModel *selectionModel;
public slots:
    void slotCreateProject();
    void slotRemoveProject();
    void slotCreateLine();
    void slotRemoveLine();
signals:
    void selectedp();
    void sigCreateProject();
    void sigRemoveProject();
    void sigCreateLine();
    void sigRemoveLine();

protected:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void createActions();
    QAction *createProjectAct;
    QAction *createLineAct;
    QAction *removeProjectAct;
    QAction *removeLineAct;




    


protected:
    //virtual bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);
};



#endif
