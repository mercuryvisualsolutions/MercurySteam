#include "viewusers.h"
#include "usersdialogs.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Users/usersmanager.h"
#include "../../Users/usersio.h"
#include "../../Auth/authmanager.h"
#include "../../Log/logmanager.h"
#include "../Dialogs/dlgcreatedbodata.h"
#include "../Dialogs/dlgcreatenote.h"
#include "../Dialogs/dlgcreatetag.h"

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
    Ms::Widgets::MContainerWidget()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
    _propertiesPanel = Session::SessionManager::instance().getSessionPropertiesPanel(Wt::WApplication::instance()->sessionId());

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
    _qtvGroups->addColumn(Ms::Widgets::MTableViewColumn("Rank", "Rank", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Users::Group>(_qtvGroups, flags, editRank);

    _qtvGroups->updateView();
}

void Views::ViewUsers::updatePropertiesView()
{
    if(_viewProperties->currentView() == "Data")
    {
        _updatePropertiesDataView();
    }
    else if(_viewProperties->currentView() == "Tags")
    {
        _updatePropertiesTagsView();
        _updatePropertiesAssignedTagsView();
    }
    else if(_viewProperties->currentView() == "Notes")
    {
        _updatePropertiesNotesView();
    }
    else if(_viewProperties->currentView() == "Groups Privileges")
    {
        _viewProperties->updateGroupsPrivilegesView();
        _updatePropertiesGroupsAssignedPrivilegesView();
    }
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

template<typename T>
void Views::ViewUsers::_addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
{
    Views::Dialogs::DlgCreateDBOData *dlg = new Views::Dialogs::DlgCreateDBOData();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
           for(auto &ptr : dboVec)
           {
                Database::DboData *data = new Database::DboData(dlg->key(), dlg->value());
                Wt::Dbo::ptr<Database::DboData> dataPtr = Database::DatabaseManager::instance().createDbo<Database::DboData>(data);

                if(dataPtr.get())
                    Database::DatabaseManager::instance().modifyDbo<T>(ptr)->addData(dataPtr);
                else
                    delete data;
           }

           _updatePropertiesDataView();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

template<typename T>
void Views::ViewUsers::_addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
{
    Views::Dialogs::DlgCreateNote *dlg = new Views::Dialogs::DlgCreateNote();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
           for(auto &ptr : dboVec)
           {
                Database::Note *note = new Database::Note(dlg->content());
                Wt::Dbo::ptr<Database::Note> notePtr = Database::DatabaseManager::instance().createDbo<Database::Note>(note);

                if(notePtr.get())
                    Database::DatabaseManager::instance().modifyDbo<T>(ptr)->addNote(notePtr);
                else
                    delete note;
           }

           _updatePropertiesNotesView();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

template<typename T>
void Views::ViewUsers::_addTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
{
    Views::Dialogs::DlgCreateTag *dlg = new Views::Dialogs::DlgCreateTag();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
           for(auto &ptr : dboVec)
           {
               Database::Tag *tag = new Database::Tag(dlg->tagName(), dlg->tagContent());
               tag->setType("Custom");
               tag->setActive(dlg->isActive());

                Wt::Dbo::ptr<Database::Tag> tagPtr = Database::DatabaseManager::instance().createDbo<Database::Tag>(tag);

                if(tagPtr.get())
                {
                    Database::DatabaseManager::instance().modifyDbo<T>(ptr)->addTag(tagPtr);

                     _logger->log(std::string("Created tag ") + dlg->tagName(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete tag;

                    _logger->log(std::string("Error creating tag ") + dlg->tagName(), Ms::Log::LogMessageType::Error);
                }
           }

           _updatePropertiesTagsView();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

template<typename T>
void Views::ViewUsers::_assignTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(dboVec.size() > 0)
    {
        for(auto &dboPtr : dboVec)
        {
            for(auto &tagPtr : tagVec)
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dboPtr)->assignTag(tagPtr);
            }
        }

        _updatePropertiesAssignedTagsView();
    }
}

template<typename T>
void Views::ViewUsers::_unAssignTagFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(dboVec.size() > 0)
    {
        for(auto &dboPtr : dboVec)
        {
            for(auto &tagPtr : tagVec)
            {
                Database::DatabaseManager::instance().modifyDbo<T>(dboPtr)->unassignTag(tagPtr);
            }
        }

        _updatePropertiesAssignedTagsView();
    }
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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
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
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
        }

        _qtvUsers->updateView();

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewUsers::_createUsersTableView()
{
    _qtvUsers = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::User>(&Database::DatabaseManager::instance());
    _qtvUsers->tableSelectionChanged().connect(this, &Views::ViewUsers::updatePropertiesView);
    _qtvUsers->setRowHeight(64);
    _qtvUsers->setDefaultFilterColumnIndex(1);
    _qtvUsers->setIgnoreNumFilterColumns(1);
    _qtvUsers->setImportCSVFetureEnabled(false);

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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
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
    _qtvGroups->tableSelectionChanged().connect(this, &Views::ViewUsers::updatePropertiesView);
    _qtvGroups->setMinimumSize(1000, 600);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvGroups->createToolButton("", "icons/Add.png", "Create A New Group");
        btn->clicked().connect(this, &Views::ViewUsers::_btnGroupsCreateClicked);

        _qtvGroups->setImportCSVFetureEnabled(true);
    }
    else
        _qtvGroups->setImportCSVFetureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvGroups->createToolButton("", "icons/Remove.png", "Remove Selected Group");
//        btn->clicked().connect(this, &Views::ViewUsers::_btnGroupsRemoveClicked);
//    }

    updateGroupsView();
}

/*******************--Properties--********************/
void Views::ViewUsers::_btnAddPropertiesDataClicked()
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Users::User>(_qtvUsers->selectedItems());
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
            _addDataToDbo<Users::Group>(_qtvGroups->selectedItems());
    }
}

void Views::ViewUsers::_btnRemovePropertiesDataClicked(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec)
{

}

void Views::ViewUsers::_btnCreatePropertiesTagClicked()
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
            _addTagToDbo<Users::User>(_qtvUsers->selectedItems());
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
            _addTagToDbo<Users::Group>(_qtvGroups->selectedItems());
    }
}

void Views::ViewUsers::_btnAssignPropertiesTagClicked(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Users::User>(_qtvUsers->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Users::Group>(_qtvGroups->selectedItems(), tagVec);
    }
}

void Views::ViewUsers::_btnUnAssignPropertiesTagClicked(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Users::User>(_qtvUsers->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Users::Group>(_qtvGroups->selectedItems(), tagVec);
    }
}

void Views::ViewUsers::_btnFilterPropertiesTagClicked(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    std::string strFilterQuery = "";

    if(tagVec.size() == 0)
        return;

    std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Database::Tag>(tagVec);

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        strFilterQuery = "Name IN (SELECT ut.user_Name FROM rel_user_assigned_tags ut WHERE tag_id IN (" + idValues.at(0) + "))";

        _qtvUsers->setCustomFilterString(strFilterQuery);
        _qtvUsers->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        strFilterQuery = "Name IN (SELECT gt.group_Name FROM rel_group_tags gt WHERE tag_id IN (" + idValues.at(0) + "))";

        _qtvGroups->setCustomFilterString(strFilterQuery);
        _qtvGroups->setCustomFilterActive(true);
    }
}

void Views::ViewUsers::_btnClearFilterPropertiesTagClicked()
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        _qtvUsers->setCustomFilterString("");
        _qtvUsers->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        _qtvGroups->setCustomFilterString("");
        _qtvGroups->setCustomFilterActive(true);
    }
}

void Views::ViewUsers::_btnAddPropertiesNoteClicked()
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Users::User>(_qtvUsers->selectedItems());
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
            _addNoteToDbo<Users::Group>(_qtvGroups->selectedItems());
    }
}

void Views::ViewUsers::_btnRemovePropertiesNoteClicked(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec)
{

}

void Views::ViewUsers::_btnAddPropertiesGroupsPrivilegesClicked(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    if(_qtvGroups->table()->selectedIndexes().size() > 0)
    {
        for(auto &grpPtr : _qtvGroups->selectedItems())
        {
            for(auto &prvPtr : privVec)
            {
                Database::DatabaseManager::instance().modifyDbo<Users::Group>(grpPtr)->addPrivilege(prvPtr);
            }
        }

        _updatePropertiesGroupsAssignedPrivilegesView();
    }
}

void Views::ViewUsers::_btnRemovePropertiesGroupsPrivilegesClicked(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    if(_qtvGroups->table()->selectedIndexes().size() > 0)
    {
        for(auto &grpPtr : _qtvGroups->selectedItems())
        {
            for(auto &prvPtr : privVec)
            {
                Database::DatabaseManager::instance().modifyDbo<Users::Group>(grpPtr)->removePrivilege(prvPtr);
            }
        }

        _updatePropertiesGroupsAssignedPrivilegesView();
    }
}

void Views::ViewUsers::_btnFilterPropertiesGroupsPrivilegesClicked(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    std::string strFilterQuery = "";

    if(privVec.size() == 0)
        return;

    std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Users::Privilege>(privVec);

    strFilterQuery = "Name IN (SELECT gp.group_Name FROM rel_group_privileges gp WHERE privilege_Name IN (" + idValues.at(0) + "))";

    _qtvGroups->setCustomFilterString(strFilterQuery);
    _qtvGroups->setCustomFilterActive(true);
}

void Views::ViewUsers::_btnClearFilterPropertiesGroupsPrivilegesClicked()
{
    _qtvGroups->setCustomFilterString("");
    _qtvGroups->setCustomFilterActive(true);
}

void Views::ViewUsers::_onViewPropertiesSubViewExposed(const std::string &viewName)
{
    if(viewName == "Data")
    {
        _updatePropertiesDataView();
    }
    else if(viewName == "Tags")
    {
        _updatePropertiesTagsView();
        _updatePropertiesAssignedTagsView();
    }
    else if(viewName == "Notes")
    {
        _updatePropertiesNotesView();
    }
    else if(viewName == "Groups Privileges")
    {
        _viewProperties->updateGroupsPrivilegesView();
        _updatePropertiesGroupsAssignedPrivilegesView();
    }
}

void Views::ViewUsers::_createPropertiesView()
{
    _viewProperties = new Views::ViewProperties();
    _viewProperties->addDataRequested().connect(this, &Views::ViewUsers::_btnAddPropertiesDataClicked);
    _viewProperties->removeDataRequested().connect(this, &Views::ViewUsers::_btnRemovePropertiesDataClicked);
    _viewProperties->createTagRequested().connect(this, &Views::ViewUsers::_btnCreatePropertiesTagClicked);
    _viewProperties->assignTagRequested().connect(this, &Views::ViewUsers::_btnAssignPropertiesTagClicked);
    _viewProperties->unAssignTagRequested().connect(this, &Views::ViewUsers::_btnUnAssignPropertiesTagClicked);
    _viewProperties->filterByTagRequested().connect(this, &Views::ViewUsers::_btnFilterPropertiesTagClicked);
    _viewProperties->clearTagFilterRequested().connect(this, &Views::ViewUsers::_btnClearFilterPropertiesTagClicked);
    _viewProperties->addNoteRequested().connect(this, &Views::ViewUsers::_btnAddPropertiesNoteClicked);
    _viewProperties->removeNoteRequested().connect(this, &Views::ViewUsers::_btnRemovePropertiesNoteClicked);
    _viewProperties->addGroupPrivilegeRequested().connect(this, &Views::ViewUsers::_btnAddPropertiesGroupsPrivilegesClicked);
    _viewProperties->removeGroupPrivilegeRequested().connect(this, &Views::ViewUsers::_btnRemovePropertiesGroupsPrivilegesClicked);
    _viewProperties->filterGroupByPrivilegeRequested().connect(this, &Views::ViewUsers::_btnFilterPropertiesGroupsPrivilegesClicked);
    _viewProperties->clearGroupPrivilegeFilterRequested().connect(this, &Views::ViewUsers::_btnClearFilterPropertiesGroupsPrivilegesClicked);
    _viewProperties->viewExposed().connect(this, &Views::ViewUsers::_onViewPropertiesSubViewExposed);

    _propertiesPanel->addPropertiesView(this->id(), _viewProperties);
}

void Views::ViewUsers::_updatePropertiesDataView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvPropertiesData = _viewProperties->qtvData();

    _qtvPropertiesData->clearColumns();

    //add columns
    _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Database::DatabaseManager::instance().session()->find<Database::DboData>();

    bool update = false;

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("User_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MTableViewColumn("Group_Name", "Group Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }

    if(update)
    {
        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesData->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::DboData>(_qtvPropertiesData, flags, editRank);

    _qtvPropertiesData->updateView();
}

void Views::ViewUsers::_updatePropertiesTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
            return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesTags = _viewProperties->qtvTags();

    _qtvPropertiesTags->clearColumns();

    //add columns
    _qtvPropertiesTags->addColumn(Ms::Widgets::MTableViewColumn("id", "ID", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Database::DatabaseManager::instance().session()->find<Database::Tag>();

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            std::string usersSelectSql = "(pt.user_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + "))";

            query.where("(id IN (SELECT pt.tag_id FROM rel_user_tags pt WHERE " + usersSelectSql + ") OR (Type = 'Global'))");
        }
        else
            query.where("Type = ?").bind("Global");
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            std::string groupsSelectSql = "pt.group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")";

            query.where("(id IN (SELECT pt.tag_id FROM rel_group_tags pt WHERE " + groupsSelectSql + ") OR (Type = 'Global'))");
        }
        else
            query.where("Type = ?").bind("Global");
    }

    int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

    if(!AppSettings::instance().isLoadInactiveData())
        query.where("Active = ?").bind(true);

    query.where("View_Rank <= ?").bind(viewRank);

    _qtvPropertiesTags->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::Tag>(_qtvPropertiesTags, flags, editRank);

    _qtvPropertiesTags->updateView();
}

void Views::ViewUsers::_updatePropertiesAssignedTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesAssignedTags = _viewProperties->qtvAssignedTags();

    _qtvPropertiesAssignedTags->clearColumns();

    //add columns
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("id", "ID", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Database::DatabaseManager::instance().session()->find<Database::Tag>();

    bool update = false;

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string usersSelectSql = "(pt.user_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + "))";

            query.where("(id IN (SELECT pt.tag_id FROM rel_user_assigned_tags pt WHERE " + usersSelectSql + "))");
        }
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string groupsSelectSql = "pt.group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")";

            query.where("(id IN (SELECT pt.tag_id FROM rel_group_assigned_tags pt WHERE " + groupsSelectSql + "))");
        }
    }

    if(update)
    {
        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesAssignedTags->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::Tag>(_qtvPropertiesAssignedTags, flags, editRank);

    _qtvPropertiesAssignedTags->updateView();
}

void Views::ViewUsers::_updatePropertiesNotesView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Auth::AuthManager::instance().currentUser()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvPropertiesNotes = _viewProperties->qtvNotes();

    _qtvPropertiesNotes->clearColumns();

    //add columns
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Database::DatabaseManager::instance().session()->find<Database::Note>();

    bool update = false;

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("User_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MTableViewColumn("Group_Name", "Group Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }

    if(update)
    {
        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesNotes->setQuery(query);

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Database::Note>(_qtvPropertiesNotes, flags, editRank);

    _qtvPropertiesNotes->updateView();
}

void Views::ViewUsers::_updatePropertiesGroupsAssignedPrivilegesView()
{
    if(_qtvGroups->table()->selectedIndexes().size() == 0)
        return;

    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    std::string groupsSelectSql = "gp.group_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")";

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query = Database::DatabaseManager::instance().session()->find<Users::Privilege>().where(
                "Name IN (SELECT gp.privilege_Name FROM rel_group_privileges gp WHERE " + groupsSelectSql + ")");

    if(!AppSettings::instance().isLoadInactiveData())
        query.where("Active = ?").bind(true);

    Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvPropertiesGroupsAssignedPrivileges = _viewProperties->qtvGroupsAssignedPrivileges();

    _qtvPropertiesGroupsAssignedPrivileges->setQuery(query);

    _qtvPropertiesGroupsAssignedPrivileges->clearColumns();

    //add columns
    _qtvPropertiesGroupsAssignedPrivileges->addColumn(Ms::Widgets::MTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

    if(AppSettings::instance().isShowExtraColumns())
        _addExtraColumns<Users::Privilege>(_qtvPropertiesGroupsAssignedPrivileges, Wt::ItemIsSelectable, 0);

    _qtvPropertiesGroupsAssignedPrivileges->removeColumn(Ms::Widgets::MTableViewColumn("Active"));

    _qtvPropertiesGroupsAssignedPrivileges->updateView();
}

/*******************--Main--********************/
void Views::ViewUsers::_mnuMainUsersItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvUsers);

    if(_viewProperties->currentView() == "Groups Privileges")
        _viewProperties->setCurrentView("Data");

    _viewProperties->setViewHidden("Groups Privileges", true);

    updateUsersView();

    _onTabUsersSelected();
}

void Views::ViewUsers::_mnuMainGroupsItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvGroups);

    _viewProperties->setViewHidden("Groups Privileges", false);

    updateGroupsView();

    _onTabGroupsSelected();
}

void Views::ViewUsers::_prepareView()
{
    /*******************--Main--********************/
    setTitle("<b><i>Users And Groups</i></b>");

    Wt::WVBoxLayout* _layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    _layMain->setContentsMargins(14,14,14,14);

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
    _stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    _layMain->addWidget(_stkMain, 1);

    /*******************--Users--********************/
    _createUsersTableView();

    _stkMain->addWidget(_qtvUsers);

    /*******************--Groups--********************/
    _createGroupsTableView();

    _stkMain->addWidget(_qtvGroups);

    /*******************--Properties--********************/
    _createPropertiesView();
}

