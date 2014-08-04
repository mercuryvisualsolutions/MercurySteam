TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

QT += core
QT -= gui

QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-parameter

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mscore/build/linux/x86_64/debug/release/ -lmscore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mscore/build/linux/x86_64/debug/debug/ -lmscore
else:unix: LIBS += -L$$PWD/../../mscore/build/linux/x86_64/debug/ -lmscore

INCLUDEPATH += $$PWD/../../mscore/src
DEPENDPATH += $$PWD/../../mscore/src

DEFINES += QT_NO_KEYWORDS

LIBS += -lwthttp -lwt -lwtdbo -lwtdbomysql -lboost_signals -lboost_filesystem -lboost_system -lboost_regex -lboost_date_time

SOURCES += main.cpp \
    Views/Projects/viewprojects.cpp \
    Views/Users/viewusers.cpp \
    Database/databasemanager.cpp \
    Users/group.cpp \
    Settings/appsettings.cpp \
    Views/Settings/viewsettings.cpp \
    Views/Reports/viewreports.cpp \
    Users/usertitle.cpp \
    Projects/project.cpp \
    Projects/projectasset.cpp \
    Projects/projectsequence.cpp \
    Projects/projectshot.cpp \
    Projects/projectaskktype.cpp \
    Projects/projecttask.cpp \
    Projects/projectworkstatus.cpp \
    Projects/projectworkstatustype.cpp \
    Projects/projectassettype.cpp \
    Views/Projects/dlgcreateproject.cpp \
    Views/Projects/dlgcreatesequence.cpp \
    Views/Projects/dlgcreateshot.cpp \
    Views/Projects/dlgcreateasset.cpp \
    Views/Projects/dlgcreatetask.cpp \
    Views/Users/dlgcreateuser.cpp \
    Views/Users/dlgcreategroup.cpp \
    Views/Settings/dlgcreateassettype.cpp \
    Views/Settings/dlgcreatetasktype.cpp \
    Views/Settings/dlgcreateworkstatus.cpp \
    Projects/projectsmanager.cpp \
    Users/usersmanager.cpp \
    Views/App/viewapp.cpp \
    Views/Main/viewmain.cpp \
    Auth/authmanager.cpp \
    Users/user.cpp \
    Users/usersio.cpp \
    Projects/projectsio.cpp \
    Views/Settings/dlgcreateusertitle.cpp \
    Views/Search/viewsearch.cpp \
    Views/Files/dlgfilesmanager.cpp \
    Views/Files/dlgcreaterepo.cpp \
    Views/Dialogs/dlginput.cpp \
    Views/Users/dlgchangeuserpssword.cpp \
    Database/dbodata.cpp \
    Users/privilege.cpp \
    Views/Log/viewlog.cpp \
    Database/note.cpp \
    Database/tag.cpp \
    Views/MyDashboard/viewmydashboard.cpp \
    Session/sessionmanager.cpp \
    Log/logger.cpp \
    Log/logmanager.cpp \
    Database/dbo.cpp \
    Views/Dialogs/dlgcreatedbodata.cpp \
    Views/Dialogs/dlgcreatenote.cpp \
    Views/Properties/viewproperties.cpp \
    Views/Dialogs/dlgcreatetag.cpp

HEADERS += \
    Views/Projects/viewprojects.h \
    Views/Users/viewusers.h \
    Database/databasemanager.h \
    Settings/appsettings.h \
    Views/Settings/viewsettings.h \
    Views/Reports/viewreports.h \
    Database/dbospecialization.h \
    Database/dbtables.h \
    Database/projectdbospecialization.h \
    Database/userdbospecialization.h \
    Views/Users/usersdialogs.h \
    Views/Projects/dlgcreateproject.h \
    Views/Projects/projectsdialogs.h \
    Views/Projects/dlgcreatesequence.h \
    Views/Projects/dlgcreateshot.h \
    Views/Projects/dlgcreateasset.h \
    Views/Projects/dlgcreatetask.h \
    Views/Users/dlgcreateuser.h \
    Views/Users/dlgcreategroup.h \
    Views/Settings/dlgcreateassettype.h \
    Views/Settings/dlgcreatetasktype.h \
    Views/Settings/settingsdialogs.h \
    Views/Settings/dlgcreateworkstatus.h \
    Projects/projectsmanager.h \
    Users/usersmanager.h \
    Views/App/viewapp.h \
    Views/Main/viewmain.h \
    Auth/authmanager.h \
    Users/usersio.h \
    Projects/projectsio.h \
    Views/Settings/dlgcreateusertitle.h \
    Views/Search/viewsearch.h \
    Views/Files/dlgfilesmanager.h \
    Views/Files/dlgcreaterepo.h \
    Views/Dialogs/dlginput.h \
    Database/dbschemaattributes.h \
    Views/Users/dlgchangeuserpssword.h \
    Views/Log/viewlog.h \
    Views/MyDashboard/viewmydashboard.h \
    Database/globaldbospecialization.h \
    Database/databasemanager_impl.h \
    Log/logglobals.h \
    Session/sessionmanager.h \
    Log/logger.h \
    Log/logmanager.h \
    Views/Dialogs/dlgcreatedbodata.h \
    Views/Dialogs/dlgcreatenote.h \
    Widgets/Delegates/workstatusquerycomboboxdelegate.h \
    Widgets/Delegates/workstatusquerycomboboxdelegate_impl.h \
    Views/Properties/viewproperties.h \
    Views/Dialogs/dlgcreatetag.h
