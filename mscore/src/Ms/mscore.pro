#-------------------------------------------------
#
# Project created by QtCreator 2014-06-11T14:10:41
#
#-------------------------------------------------

QT       -= gui

TARGET = mscore
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

DEFINES += MSCORE_LIBRARY

LIBS += -lwthttp -lwt -lwtdbo -lwtdbomysql -lboost_signals -lboost_filesystem -lboost_system -lboost_regex -lboost_date_time

SOURCES += \
    Dbo/Mdbo.cpp \
    Core/MManagerBase.cpp \
    Exceptions/MDboSessionIsNullException.cpp \
    Core/MSettingsBase.cpp \
    Widgets/MWidgetFactory.cpp \
    Widgets/MTableViewColumn.cpp \
    Widgets/Delegates/MCheckBoxDelegate.cpp \
    Widgets/Delegates/MDateDelegate.cpp \
    Widgets/Delegates/MDateFieldDelegate.cpp \
    Widgets/Delegates/MFloatFieldDelegate.cpp \
    Widgets/Delegates/MIntFieldDelegate.cpp \
    Widgets/Delegates/MItemDelegate.cpp \
    Widgets/Delegates/MPasswordFieldDelegate.cpp \
    Widgets/Delegates/MThumbnailDelegate.cpp \
    Widgets/Delegates/MValidatorFieldDelegate.cpp \
    IO/MFileInfo.cpp \
    IO/Data/MDataCSV.cpp \
    IO/Data/MDataCSVRow.cpp \
    IO/IO.cpp \
    IO/MStreamedFileResource.cpp \
    IO/MStreamedMemoryResource.cpp \
    Widgets/Dialogs/MFilesUploadDialog.cpp \
    IO/MResource.cpp \
    Core/Dbo/MDboManagerBase.cpp \
    Log/MLogger.cpp \
    Widgets/MLogWidget.cpp \
    Widgets/Delegates/MComboBoxDelegate.cpp

HEADERS +=\
    Mscore_global.h \
    Dbo/MDbo.h \
    Dbo/MDbo_impl.h \
    Core/MManagerBase_impl.h \
    Dbo/MDboQueryModel.h \
    Exceptions/MDboSessionIsNullException.h \
    Dbo/MDboQueryModel.h \
    Dbo/MDboQueryModel_impl.h \
    Core/MManagerBase.h \
    Core/MSettingsBase.h \
    Core/MSettingsBase_impl.h \
    Widgets/MWidgetFactory.h \
    Widgets/MQueryTableViewWidget.h \
    Widgets/MQueryTableViewWidget_impl.h \
    Widgets/MTableViewColumn.h \
    Widgets/Delegates/MCheckBoxDelegate.h \
    Widgets/Delegates/MDateDelegate.h \
    Widgets/Delegates/MDateFieldDelegate.h \
    Widgets/Delegates/MDelegates \
    Widgets/Delegates/MFloatFieldDelegate.h \
    Widgets/Delegates/MIntFieldDelegate.h \
    Widgets/Delegates/MItemDelegate.h \
    Widgets/Delegates/MPasswordFieldDelegate.h \
    Widgets/Delegates/MQueryComboBoxDelegate.h \
    Widgets/Delegates/MThumbnailDelegate.h \
    Widgets/Delegates/MValidatorFieldDelegate.h \
    Widgets/Delegates/MQueryComboBoxDelegate_impl.h \
    IO/MFileInfo.h \
    IO/Data/MDataCSV.h \
    IO/Data/MDataCSVRow.h \
    IO/IO.h \
    IO/MResource.h \
    IO/MStreamedFileResource.h \
    IO/MStreamedMemoryResource.h \
    Widgets/Dialogs/MFilesUploadDialog.h \
    Core/Dbo/MDboManagerBase_impl.h \
    Core/Dbo/MDboManagerBase.h \
    Widgets/MWidgetFactory_impl.h \
    Log/MLogger.h \
    Log/MLogGlobals.h \
    Widgets/MLogWidget.h \
    Widgets/Delegates/MComboBoxDelegate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
