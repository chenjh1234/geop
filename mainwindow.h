#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
//class QPlainTextEdit;
QT_END_NAMESPACE
#include <QSplitter>
#include <QString>
#include <QTabWidget>
#include <QDockWidget>
#include <QTextBrowser>
#include <QProcess>
#include "treeview.h"
#include "listview.h"
#include "createpdlg.h"
#include <iostream>
#include "gdata.h"
using namespace std;
 
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
     
    

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void slotHelp();
    void documentWasModified();
    void slotCreateProject();
    void slotCreateLine();
    void slotRemoveProject();
    void slotRemoveLine();
    void slotRefreshAll();
    void slotShowText(QString,QString);
    void slotShowFiles(int);
    void slotXterm();


private:
    void initMod();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void setTitle();

   // QPlainTextEdit *textEdit;
    QTextBrowser *textView;
    QSplitter *split;
    QTabWidget *tabWidget;
    QDockWidget *treeDockWidget,*infoDockWidget;
    treeView *treeProject;
    listView *listFlow,*listJob,*listData,*listLog,*listTable;
    createpDlg *dlgCreate;
    gdata *m_data;

    QString curFile;
     
    QProcess run;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *createProjectAct;
    QAction *createLineAct;
    QAction *removeProjectAct;
    QAction *removeLineAct;
    QAction *exitAct;
    QAction *refreshAct;
    QAction *xtermAct;
     
    QAction *aboutAct;
    QAction *helpAct;
};
//! [0]

#endif
