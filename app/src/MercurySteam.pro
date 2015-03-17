TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

QT -= core gui

QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-parameter

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mscore/build/linux/x86_64/debug/release/ -lmscore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mscore/build/linux/x86_64/debug/debug/ -lmscore
else:unix: LIBS += -L$$PWD/../../mscore/build/linux/x86_64/debug/ -lmscore

INCLUDEPATH += $$PWD/../../mscore/src
DEPENDPATH += $$PWD/../../mscore/src

LIBS += -lwthttp -lwt -lwtdbo -lwtdbomysql -lboost_signals -lboost_filesystem -lboost_system -lboost_regex -lboost_date_time -ljpeg -lpng

SOURCES += main.cpp \
    Views/Projects/viewprojects.cpp \
    Views/Users/viewusers.cpp \
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
    Views/Users/dlgcreateuser.cpp \
    Views/Users/dlgcreategroup.cpp \
    Views/Settings/dlgcreateassettype.cpp \
    Views/Settings/dlgcreatetasktype.cpp \
    Views/Settings/dlgcreateworkstatus.cpp \
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
    Views/Dialogs/dlgcreatetag.cpp \
    Views/Database/viewdbodata.cpp \
    Views/Database/viewtags.cpp \
    Views/Database/viewnotes.cpp \
    Views/Projects/viewsequences.cpp \
    Views/Projects/viewshots.cpp \
    Views/Projects/viewassets.cpp \
    Views/Projects/viewtasks.cpp \
    Views/Users/viewprivileges.cpp \
    Views/Projects/dlgtaskselectdbo.cpp \
    Views/Projects/dlgcreateandeditsequence.cpp \
    Views/Projects/dlgcreateandeditshot.cpp \
    Views/Projects/dlgcreateandeditasset.cpp \
    Views/Projects/dlgcreateandeditproject.cpp \
    Views/Projects/dlgcreateandedittask.cpp \
    Projects/projectdbo.cpp \
    Projects/projecttaskactivitytype.cpp \
    Projects/projecttaskactivity.cpp \
    Views/Settings/dlgcreatetaskactivitytype.cpp \
    Views/Projects/viewtaskactivities.cpp \
    Views/Projects/dlgcreateandedittaskactivity.cpp \
    Views/Projects/dlgselectactivitytemplate.cpp \
    Views/Settings/dlgcreatetemplate.cpp \
    Views/Settings/viewactivitytemplate.cpp \
    Views/Settings/dlgcreateandeditactivitytemplateitem.cpp \
    Views/Settings/dlgcreateandedittasktemplateitem.cpp \
    Views/Projects/dlgselecttasktemplate.cpp \
    Views/Settings/viewtasktemplate.cpp \
    Projects/projectactivitytemplate.cpp \
    Projects/projectactivitytemplateactivityitem.cpp \
    Projects/projecttasktemplatetaskitem.cpp \
    Projects/projecttasktemplate.cpp \
    Application/msapplication.cpp \
    Database/userdatabase.cpp \
    Database/token.cpp \
    Database/dbosession.cpp \
    Database/notification.cpp

HEADERS += \
    Views/Projects/viewprojects.h \
    Views/Users/viewusers.h \
    Settings/appsettings.h \
    Views/Settings/viewsettings.h \
    Views/Reports/viewreports.h \
    Database/dbospecialization.h \
    Database/dbtables.h \
    Database/projectdbospecialization.h \
    Database/userdbospecialization.h \
    Views/Users/usersdialogs.h \
    Views/Projects/projectsdialogs.h \
    Views/Users/dlgcreateuser.h \
    Views/Users/dlgcreategroup.h \
    Views/Settings/dlgcreateassettype.h \
    Views/Settings/dlgcreatetasktype.h \
    Views/Settings/settingsdialogs.h \
    Views/Settings/dlgcreateworkstatus.h \
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
    Log/logglobals.h \
    Session/sessionmanager.h \
    Log/logger.h \
    Log/logmanager.h \
    Views/Dialogs/dlgcreatedbodata.h \
    Views/Dialogs/dlgcreatenote.h \
    Widgets/Delegates/workstatusquerycomboboxdelegate.h \
    Widgets/Delegates/workstatusquerycomboboxdelegate_impl.h \
    Views/Dialogs/dlgcreatetag.h \
    Views/Database/viewdbodata.h \
    Views/Database/viewtags.h \
    Views/Database/viewnotes.h \
    Views/Projects/viewsequences.h \
    Views/Projects/viewshots.h \
    Views/Projects/viewassets.h \
    Views/Projects/viewtasks.h \
    Views/Users/viewprivileges.h \
    Views/Projects/dlgtaskselectdbo.h \
    Views/Projects/dlgcreateandeditsequence.h \
    Views/Projects/dlgcreateandeditshot.h \
    Views/Projects/dlgcreateandeditasset.h \
    Views/Projects/dlgcreateandeditproject.h \
    Views/Projects/dlgcreateandedittask.h \
    Views/Settings/dlgcreatetaskactivitytype.h \
    Views/Projects/viewtaskactivities.h \
    Views/Projects/dlgcreateandedittaskactivity.h \
    Views/Projects/dlgselectactivitytemplate.h \
    Views/Settings/dlgcreatetemplate.h \
    Views/Settings/viewactivitytemplate.h \
    Views/Settings/dlgcreateandeditactivitytemplateitem.h \
    Views/Settings/dlgcreateandedittasktemplateitem.h \
    Views/Projects/dlgselecttasktemplate.h \
    Views/Settings/viewtasktemplate.h \
    Application/msapplication.h \
    Database/userdatabase.h \
    Database/dbosession.h \
    Database/dbosession_impl.h
