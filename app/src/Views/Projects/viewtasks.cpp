#include "viewtasks.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewTasks::ViewTasks()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewTasks::qtvTasks() const
{
    return _qtvTasks;
}

Wt::Signal<> &Views::ViewTasks::createTaskRequested()
{
    return _createTaskRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &Views::ViewTasks::removeTasksRequested()
{
    return _removeTasksRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &Views::ViewTasks::openfilesViewRequested()
{
    return _openfilesViewRequested;
}

void Views::ViewTasks::_btnCreateTaskClicked()
{
    _createTaskRequested();
}

void Views::ViewTasks::_btnRemoveTasksClicked()
{
    _removeTasksRequested(_qtvTasks->selectedItems());
}

void Views::ViewTasks::_btnOpenFilesViewClicked()
{
    _openfilesViewRequested(_qtvTasks->selectedItems());
}

void Views::ViewTasks::_createTasksTableView()
{
    _qtvTasks = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectTask>(&Database::DatabaseManager::instance());

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Add.png", "Create A New Task");
        btn->clicked().connect(this, &Views::ViewTasks::_btnCreateTaskClicked);

        _qtvTasks->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvTasks->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Remove.png", "Remove Selected Tasks");
        //btn->clicked().connect(this, &Views::ViewTasks::_btnRemoveTaskClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvTasks->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewTasks::_btnOpenFilesViewClicked);
    }
}

void Views::ViewTasks::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createTasksTableView();

    _layMain->addWidget(_qtvTasks);
}
