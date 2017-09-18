#include "createpdlg.h"
 
createpDlg::createpDlg(QWidget *p):QDialog(p)
{
    ui.setupUi(this);
    init();
}
createpDlg::~createpDlg()
{

} 
void createpDlg::init()
{
  
}
void createpDlg::setData(gdata *d)
{
    data = d;
}
void createpDlg::setProject()
{
    QString str;
    str = "Create Project";
    ui.label->setText(str);
    setWindowTitle(str);
}
int createpDlg::setLine()
{
    QString str,project;
    string s; 
    int i;
    //qDebug() <<"befor get = " << &(data->m_project);
    //i= data->m_project.numProjects();
     //qDebug() <<"after num = " << i;
    s= data->m_project.getCurrentProject();
    //qDebug() <<"after get = " << s.c_str();
    project = data->str2Qstr(s);
    if (project.isEmpty()) 
    {
        QMessageBox::warning(this,"No selected Project","No selected Project");
        return -1;     
    }
    str = "Create line In project: " + project; 
    //qDebug() << "project str = " << str;
    ui.label->setText(str);
    setWindowTitle(str);
    return 0;
}
void createpDlg::accept()
{
    QDialog::accept();
   qDebug()<< "yes accept";
}
QString createpDlg::getName()
{
    return ui.lineEdit->text();
}

