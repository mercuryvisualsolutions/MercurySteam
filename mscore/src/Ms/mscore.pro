#-------------------------------------------------
#
# Project created by QtCreator 2014-06-11T14:10:41
#
#-------------------------------------------------

QT       -= core gui

TARGET = mscore
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

LIBS += -lwthttp -lwt -lwtdbo -lwtdbomysql -lboost_signals -lboost_filesystem -lboost_system -lboost_regex -lboost_date_time

SOURCES += \
    Core/MManagerBase.cpp \
    Core/MSettingsBase.cpp \
    Widgets/MWidgetFactory.cpp \
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
    Log/MLogger.cpp \
    Widgets/MLogWidget.cpp \
    Widgets/Delegates/MComboBoxDelegate.cpp \
    Exceptions/MNullPointerException.cpp \
    Widgets/MPropertiesPanel.cpp \
    Dbo/MdboBase.cpp \
    Widgets/MContainerWidget.cpp \
    Widgets/MTableViewWidget.cpp \
    Widgets/MQueryTableViewColumn.cpp \
    Core/MTableViewColumn.cpp \
    Core/Dbo/MDboSession.cpp

HEADERS +=\
    Mscore_global.h \
    Dbo/MDboQueryModel.h \
    Dbo/MDboQueryModel.h \
    Dbo/MDboQueryModel_impl.h \
    Core/MManagerBase.h \
    Core/MSettingsBase.h \
    Core/MSettingsBase_impl.h \
    Widgets/MWidgetFactory.h \
    Widgets/MQueryTableViewWidget.h \
    Widgets/MQueryTableViewWidget_impl.h \
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
    Widgets/MWidgetFactory_impl.h \
    Log/MLogger.h \
    Log/MLogGlobals.h \
    Widgets/MLogWidget.h \
    Widgets/Delegates/MComboBoxDelegate.h \
    Exceptions/MNullPointerException.h \
    Widgets/MPropertiesPanel.h \
    Dbo/MDboBase.h \
    Dbo/MDboBase_impl.h \
    Widgets/MContainerWidget.h \
    Widgets/MTableViewWidget.h \
    Widgets/MQueryTableViewColumn.h \
    Core/MTableViewColumn.h \
    Core/Dbo/MDboSession_impl.h \
    Core/Dbo/MDboSession.h

unix {
    target.path = /usr/lib64
    INSTALLS += target
}
