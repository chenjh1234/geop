#ifndef CREATEPDLG_H 
#define CREATEPDLG_H
#include <QDialog>
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include "ui_createproject.h"
#include "gdata.h"
 
class createpDlg : public QDialog
{
    Q_OBJECT
public:
    createpDlg(QWidget *p);
    ~createpDlg();
     
 
    void init();
    /**
     * before call exec(), set the title "create project"
     *   
     */
    void setProject();
     /**
     * before call exec(), set the title "create line in project"
     *   
     */
    int setLine();
    /**
     *  get the text in line edit
     * 
     * @author cjh (7/8/2015)
     * 
     * @return QString 
     */
    QString getName();
    /**
     *  set the application data object
     *  
     */

    void setData(gdata *d);
    gdata *data;
public slots:
     
    void accept();

private:

    Ui::Dialog ui;

};
#endif

