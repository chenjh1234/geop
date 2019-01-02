#include <gdata.h>

gdata::gdata()
{
    qDebug() << "i am in data";
     
    init();

}
//-----------------
gdata::~gdata()
{

}
//-------------------
void gdata::init()
{
    //m_project = new projectMng();
    cout << "wwwwww\n";

}
QString gdata::str2Qstr(string s)
{
    QString str;
    if (s.empty()) 
        str = "";
    else
        str = s.c_str(); 
    return str;
}
QString gdata::getWKDir()
{
    QString path,qstr;
    QString cmd;
    string str;
    QString project,line;
    project = m_project.getCurrentProject().c_str();
    line = m_project.getCurrentLine().c_str();
    if (line.isEmpty()) 
    {
        str = m_project.getProjectHome(project.toUtf8().data()) ;
    }
    else
    {
        str =  m_project.getLineHome(line.toUtf8().data()) ;
    }
    qstr = str.c_str();
    return qstr;
}
