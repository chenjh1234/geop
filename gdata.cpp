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
