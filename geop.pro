 
include (../geoseis.include)

TEMPLATE = app
QT += widgets
QT += testlib

CONFIG += debug
TARGET = geop
DESTDIR =$(GEOSEIS)/bin 

FORMS = createproject.ui


win32{
    INCLUDEPATH += $(GEOSEIS)/include 
    LIBS +=  \
        -L$(GEOSEIS)/lib\
        -lprojectBase
}

!win32{
        INCLUDEPATH += ../projectbase

        LIBS +=  \
        -L$(GEOSEIS)/lib\
        -lprojectBase
}

HEADERS       = mainwindow.h\
		listview.h\
		treeview.h\
		createpdlg.h\
		gdata.h

SOURCES       = geop.cpp \
                mainwindow.cpp\
		listview.cpp\
		treeview.cpp\
		createpdlg.cpp\
		gdata.cpp
#! [0]
RESOURCES     = geop.qrc
#! [0]

 


