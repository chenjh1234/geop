#ifndef GDATA_H 
#define GDATA_H 
 
#include "projectmng.h"
#include "filemng.h"
#include <QString>
#include <QDebug>
#define ORG_NAME "GEOSEIS"
#define APP_NAME "ProjectPad"
#define APP_VERSION "1.00"
#define APP_DATE "2015.07"


class gdata  
{
public:
    gdata();
    ~gdata();
    void init();
    projectMng m_project;
    jobMng m_job;
    flowMng m_flow;
    dataMng m_sdata;
    logMng m_log;
    QString str2Qstr(string s);
    QString getWKDir();

};

#endif
