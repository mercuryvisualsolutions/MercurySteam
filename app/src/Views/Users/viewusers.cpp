﻿#include "viewusers.h"
#include "usersdialogs.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Users/usersmanager.h"
#include "../../Users/usersio.h"
#include "../../Auth/authmanager.h"
#include "../../Log/logmanager.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/Widgets/Dialogs/MFilesUploadDialog.h>
#include <Ms/Widgets/MTableViewColumn.h>
#include <Ms/Widgets/MWidgetFactory.h>

#include <iostream>
#include <stdio.h>
#include <tuple>

#include <Wt/Dbo/Query>
#include <Wt/WMessageBox>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

Views::ViewUsers::ViewUsers() :
    WContainerWidget()
{
    _logger = Log::LogManager::instance().getAppSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
    _mnuMain->select(_mnuMainUsersItem);
}

void Views::ViewUsers::updateView()
{
    if(_stkMain->currentWidget() == _qtvUsers)
        updateUsersView();
    else if(_stkMain->currentWidget() == _qtvGroups)
        updateGroupsView();
}

void Views::ViewUsers::updateUsersView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;

    int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::User>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Database::DatabaseManager::instance().session()->find<Users::User>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    _qtvUsers->setQuery(query);

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    _qtvUsers->clearColumns();

    //add columns
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(100, 64, "pics/NoPreview.png"), false, true, 100));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Group_Name", "Group", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::Group>(
     Database::DatabaseManager::instance().session() ,
     AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Users::Group>() :
     Database::DatabaseManager::instance().session()->find<Users::Group>().where("Active = ?").bind(true),
     "Name", editRank), true));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Title_Name", "Title", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::UserTitle>(
     Database::DatabaseManager::instance().session(),
     AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Users::UserTitle>() :
     Database::DatabaseManager::instance().session()->find<Users::UserTitle>().where("Active = ?").bind(true),
     "Name", editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Email_Address", "Email", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Phone_Number", "Phone Number", flags, new Ms::Widgets::Delegates::MValidatorFieldDelegate("[0-9]{1,255}", true, editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Id_Number", "Id Number", flags, new Ms::Widgets::Delegates::MValidatorFieldDelegate("[0-9]{1,255}", true, editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Address", "Address", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

    if(AppSettings::instance().isShowExtraColumns())
    {
        _qtvUsers->addColumn(Ms::Widgets::MTableViewColumn("Create_Rank", "Create Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));

        _addExtraColumns<Users::User>(_qtvUsers, flags, editRank);
    }

    _qtvUsers->updateView();
}

void Views::ViewUsers::updateGroupsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Group>> query;

    int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Database::DatabaseManager::instance().session()->find<Users::Group>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Database::DatabaseManager::instance().session()->find<Users::Group>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    _qtvGroups->setQuery(query);

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    _qtvGroups->clearColumns();

    //add columns
    _qtvGroups->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Users::Group>(_qtvGroups, flags, editRank);

    _qtvGroups->updateView();
}

bool Views::ViewUsers::isUsersViewShown()
{
    return _stkMain->currentWidget() == _qtvUsers;
}

bool Views::ViewUsers::isGroupsViewShown()
{
    return _stkMain->currentWidget() == _qtvGroups;
}

Ms::Widgets::MQueryTableViewWidget<Users::User> *Views::ViewUsers::usersQueryTableView() const
{
    return _qtvUsers;
}

Ms::Widgets::MQueryTableViewWidget<Users::Group> *Views::ViewUsers::groupsQueryTableView() const
{
    return _qtvGroups;
}

Wt::Signal<> &Views::ViewUsers::onTabUsersSelected()
{
    return _onTabUsersSelected;
}

Wt::Signal<> &Views::ViewUsers::onTabGroupsSelected()
{
    return _onTabGroupsSelected;
}

template<typename T>
void Views::ViewUsers::_addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank)
{
    widget->addColumn(Ms::Widgets::MTableViewColumn("View_Rank", "View Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Edit_Rank", "Edit Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Remove_Rank", "Remove Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Created_By", "Created By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Last_Modified_Date", "Last Modified Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Last_Modified_By", "Last Modified By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
    widget->addColumn(Ms::Widgets::MTableViewColumn("Active", "Active", Wt::ItemIsSelectable | Wt::ItemIsUserCheckable, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank)));
}

/*******************--Users--********************/
void Views::ViewUsers::_btnUsersCreateClicked()
{
    Views::DlgCreateUser *dlg = new Views::DlgCreateUser();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            if(!Database::DatabaseManager::instance().dboExists<Users::User>(dlg->userName()))
            {
                Wt::Auth::User authUser = Auth::AuthManager::instance().registerUser(dlg->userName(), dlg->password(), dlg->emailAddress());
                if(authUser.isValid())//added the auth user successfully ?
                {
                    Users::User *user = new Users::User(dlg->userName(), dlg->emailAddress());
                    user->setGroup(dlg->group());
                    user->setTitle(dlg->title());
                    user->setPhoneNumber(dlg->phoneNumber());
                    user->setIdNumber(dlg->idNumber());
                    user->setAddress(dlg->address());
                    user->setActive(dlg->isActive());

                    Wt::Dbo::ptr<Users::User> userPtr = Database::DatabaseManager::instance().createDbo<Users::User>(user);

                    if(userPtr)
                    {
                        Wt::Dbo::ptr<Auth::AuthInfo> authInfo = Auth::AuthManager::instance().getUserAuthInfo(authUser);
                        if(authInfo)
                            authInfo.modify()->setUser(userPtr);

                        //create user directory structure
                        Users::UsersIO::createUserDirectoryStructure(user->name());

                        updateUsersView();

                        _logger->log(std::string("Created user ") + dlg->userName(), Ms::Log::LogMessageType::Info);
                    }
                    else
                    {
                        delete user;

                        _logger->log(std::string("error creating user ") + dlg->userName(), Ms::Log::LogMessageType::Error);
                    }
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewUsers::_btnUsersChangePasswordClicked()
{
    if(_qtvUsers->table()->selectedIndexes().size() < 1)
        return;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    Views::DlgChangeUserPassword *dlg = new Views::DlgChangeUserPassword();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(const Wt::WModelIndex &index : _qtvUsers->table()->selectedIndexes())
            {
                Wt::Dbo::ptr<Users::User> userPtr = _qtvUsers->model()->resultRow(_qtvUsers->proxyModel()->mapToSource(index).row());

                if(editRank >= userPtr->editRank())
                    Auth::AuthManager::instance().setUserPassword(Wt::Auth::Identity::LoginName, userPtr->name(), dlg->password());
            }
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewUsers::_btnUsersRemoveClicked()
{
//    if(tblUsers->selectedIndexes().size() > 0)
//    {
//        std::vector<userData> delRows;
//        try
//        {
//            Wt::Dbo::Transaction transaction(Database::DatabaseManager::instance().session);

//            for(auto &userIndex : tblUsers->selectedIndexes())
//                delRows.push_back(mdlTblUsers->resultRow(mdlTblUsersFilter->mapToSource(userIndex).row()));

//            for(std::vector<int>::size_type i = 0; i < delRows.size(); ++i)
//                delRows.at(i).remove();

//            transaction.commit();

//            mdlTblUsers->reload();
//        }
//        catch(...)
//        {
//            std::cerr << "Exception occured while trying to remove users" << std::endl;
//        }
//    }
}

void Views::ViewUsers::_btnUsersImportThumbnailsClicked()
{
    if(_qtvUsers->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by user name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                Wt::Dbo::ptr<Users::User> userPtr;

                if(AppSettings::instance().isLoadInactiveData())
                    userPtr = Database::DatabaseManager::instance().session()->find<Users::User>().where("Name = ?").bind(rawFileName);
                else
                    userPtr = Database::DatabaseManager::instance().session()->find<Users::User>().where("Name = ? AND Active = ?").bind(rawFileName).bind(true);

                if(userPtr)//found a user that has the same name as the thumbnail ?
                {
                    std::string userDir = Users::UsersIO::getAbsoluteUserDir(userPtr->name());
                    std::string localFile = userDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                    if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                        Ms::IO::removeFile(localFile);//delete it

                    if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                    {
                        userPtr.modify()->modify()->setThumbnail(Users::UsersIO::getRelativeUserDir(userPtr->name()) + Ms::IO::dirSeparator() +
                                                                 "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table users") + e.what(),
                             Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info);
        }

        _qtvUsers->reload();

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewUsers::_createUsersTableView()
{
    _qtvUsers = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::User>(&Database::DatabaseManager::instance());
    _qtvUsers->setRowHeight(64);
    _qtvUsers->setDefaultFilterColumnIndex(1);
    _qtvUsers->setIgnoreNumFilterColumns(1);
    _qtvUsers->setImportOptionVisible(false);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvUsers->createToolButton("", "icons/Add.png", "Create A New User");
        btn->clicked().connect(this, &Views::ViewUsers::_btnUsersCreateClicked);
    }

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvUsers->createToolButton("", "icons/Password.png", "Change User Password");
        btn->clicked().connect(this, &Views::ViewUsers::_btnUsersChangePasswordClicked);
    }

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvUsers->createToolButton("", "icons/Remove.png", "Remove Selected User");
//        btn->clicked().connect(this, &Views::ViewUsers::_btnUsersRemoveClicked);
//    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvUsers->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewUsers::_btnUsersImportThumbnailsClicked);
    }

    updateUsersView();
}

/*******************--Groups--********************/

void Views::ViewUsers::_btnGroupsCreateClicked()
{
    Views::DlgCreateGroup *dlg = new Views::DlgCreateGroup();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            if(!Database::DatabaseManager::instance().dboExists<Users::Group>(dlg->groupName()))
            {
                Users::Group *group = new Users::Group(dlg->groupName(), dlg->rank());
                Wt::Dbo::ptr<Users::Group> groupPtr = Database::DatabaseManager::instance().createDbo<Users::Group>(group);

                if(groupPtr)
                {
                    _logger->log(std::string("Created group ") + dlg->groupName(), Ms::Log::LogMessageType::Info);

                    updateGroupsView();
                }
                else
                {
                    delete group;

                    _logger->log(std::string("error creating group ") + dlg->groupName(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::ViewUsers::_btnGroupsRemoveClicked()
{
//    if(tblGroups->selectedIndexes().size() > 0)
//    {
//        std::vector<Wt::Dbo::ptr<Users::Group>> delRows;
//        try
//        {
//            Wt::Dbo::Transaction transaction(Database::DatabaseManager::instance().session);
//            for(auto &groupIndex : tblGroups->selectedIndexes())
//                delRows.push_back(mdlTblGroups->resultRow(groupIndex.row()));

//            for(std::vector<int>::size_type i = 0; i < delRows.size(); ++i)
//                delRows.at(i).remove();

//            transaction.commit();

//            mdlTblGroups->reload();
//        }
//        catch(...)
//        {
//            std::cerr << "Exception occured while trying to remove groups" << std::endl;
//        }
//    }
}

void Views::ViewUsers::_createGroupsTableView()
{
    _qtvGroups = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Group>(&Database::DatabaseManager::instance());
    _qtvGroups->setMinimumSize(1000, 600);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvGroups->createToolButton("", "icons/Add.png", "Create A New Group");
        btn->clicked().connect(this, &Views::ViewUsers::_btnGroupsCreateClicked);

        _qtvGroups->setImportOptionVisible(true);
    }
    else
        _qtvGroups->setImportOptionVisible(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvGroups->createToolButton("", "icons/Remove.png", "Remove Selected Group");
//        btn->clicked().connect(this, &Views::ViewUsers::_btnGroupsRemoveClicked);
//    }

    updateGroupsView();
}

void Views::ViewUsers::_mnuMainUsersItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvUsers);
    updateUsersView();

    _onTabUsersSelected();
}

void Views::ViewUsers::_mnuMainGroupsItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvGroups);
    updateGroupsView();

    _onTabGroupsSelected();
}

void Views::ViewUsers::_prepareView()
{
    /*******************--Layouts--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);
    setLayout(_layMain);

    /*******************--Main--********************/
    _cntTxtUsersPanelTitle = new Wt::WContainerWidget();
    _cntTxtUsersPanelTitle->setStyleClass("toolbar");
    _cntTxtUsersPanelTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtUsersPanelTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtUsersPanelTitle);

    _txtUsersPanelTitle = new Wt::WText("<b><i>Users And Groups</i></b>");
    _txtUsersPanelTitle->setStyleClass("soft-text");

    _cntTxtUsersPanelTitle->addWidget(_txtUsersPanelTitle);

    _navBarMain = new Wt::WNavigationBar();

    _mnuMain = new Wt::WMenu(Wt::Horizontal);

    _navBarMain->addMenu(_mnuMain);

    _mnuMainUsersItem = new Wt::WMenuItem("Users");
    _mnuMainUsersItem->triggered().connect(this, &Views::ViewUsers::_mnuMainUsersItemTriggered);
    _mnuMain->addItem(_mnuMainUsersItem);

    _mnuMainGroupsItem = new Wt::WMenuItem("Groups");
    _mnuMainGroupsItem->triggered().connect(this, &Views::ViewUsers::_mnuMainGroupsItemTriggered);
    _mnuMain->addItem(_mnuMainGroupsItem);

    _layMain->addWidget(_navBarMain);

    _stkMain = new Wt::WStackedWidget();
    _layMain->addWidget(_stkMain, 1);

    /*******************--Users--********************/
    _createUsersTableView();

    _stkMain->addWidget(_qtvUsers);

    /*******************--Groups--********************/
    _createGroupsTableView();

    _stkMain->addWidget(_qtvGroups);
}
