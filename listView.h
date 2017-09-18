#ifndef LISTVIEW_H 
#define LISTVIEW_H 

#include <QWidget>
#include <QListView>
#include <QTreeView>
#include <QString>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QContextMenuEvent>
//#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include <QMap>
#include <QProcess>
#include <QFileDialog>
#include "gdata.h"
#include "filemng.h"

class listView :public QTreeView
{
    Q_OBJECT
public:
    listView(QWidget *p);
    ~listView();
    QString getConfig();
    void refresh();
    void setData(gdata *d);
    void init();//leave it public
    fileMng *m_file;//leave is public
    QString getTypeStr(QString  s);// dir,filter
    void getSelections();

public slots:
    void slotDisplayFile();
    void slotEditFile();
    void slotRemoveFile();
    void slotCopyTo();
    void slotCopyFrom();
    void slotRename();

protected:
    void contextMenuEvent(QContextMenuEvent * ev );
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void mouseDoubleClickEvent ( QMouseEvent * event );
private:
    
    gdata *data;
    QStandardItemModel  *model;
    QSortFilterProxyModel *pmodel;
    QProcess run;
    //base class of "Job,Flow,data,table,Log.
private:
    void createActions();
    QAction *removeFileAct;
    QAction *displayFileAct;
    QAction *editFileAct;
    QAction *copyToAct;
    QAction *copyFromAct;
    QAction *renameAct;

    QMap <QString,QString> m_selected;
    QMap <QString,QString> m_selections;


signals:
    void sigDisplay(QString,QString);
    void sigFiles(int);


 


   // virtual bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);
};



#endif

