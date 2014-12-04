#include "viewusers.h"
#include "usersdialogs.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Users/usersio.h"
#include "../../Auth/authmanager.h"
#include "../../Log/logmanager.h"
#include "../Dialogs/dlgcreatedbodata.h"
#include "../Dialogs/dlgcreatenote.h"
#include "../Dialogs/dlgcreatetag.h"

#include <Ms/Widgets/Delegates/MDelegates>
#include <Ms/Widgets/Dialogs/MFilesUploadDialog.h>
#include <Ms/Widgets/MQueryTableViewColumn.h>
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
    _logger = Session::SessionManager::instance().logger();
    _propertiesPanel = Session::SessionManager::instance().propertiesPanel();

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
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::User>> query;

    int viewRank = Session::SessionManager::instance().user()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::User>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Session::SessionManager::instance().dboSession().find<Users::User>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    _qtvUsers->setQuery(query);

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    _qtvUsers->clearColumns();

    //add columns
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(100, 64, "pics/NoPreview.png"), false, true, 100));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Group_Name", "Group", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::Group>(
     &Session::SessionManager::instance().dboSession() ,
     AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::Group>() :
     Session::SessionManager::instance().dboSession().find<Users::Group>().where("Active = ?").bind(true),
     "Name", editRank), true));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Title_Name", "Title", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::UserTitle>(
     &Session::SessionManager::instance().dboSession(),
     AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::UserTitle>() :
     Session::SessionManager::instance().dboSession().find<Users::UserTitle>().where("Active = ?").bind(true),
     "Name", editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Email_Address", "Email", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Phone_Number", "Phone Number", flags, new Ms::Widgets::Delegates::MValidatorFieldDelegate("[0-9]{1,255}", true, editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Id_Number", "Id Number", flags, new Ms::Widgets::Delegates::MValidatorFieldDelegate("[0-9]{1,255}", true, editRank)));
    _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Address", "Address", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

    if(AppSettings::instance().isShowExtraColumns())
    {
        _qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Create_Rank", "Create Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));

        _qtvUsers->addBaseColumns(flags, editRank);
    }

    transaction.commit();

    _qtvUsers->updateView();
}

void Views::ViewUsers::updateGroupsView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Group>> query;

    int viewRank = Session::SessionManager::instance().user()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::Group>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Session::SessionManager::instance().dboSession().find<Users::Group>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    _qtvGroups->setQuery(query);

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    _qtvGroups->clearColumns();

    //add columns
    _qtvGroups->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvGroups->addColumn(Ms::Widgets::MQueryTableViewColumn("Rank", "Rank", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    if(AppSettings::instance().isShowExtraColumns())
        _qtvGroups->addBaseColumns(flags, editRank);

    transaction.commit();

    _qtvGroups->updateView();
}

void Views::ViewUsers::updatePropertiesView()
{
    if(_stkProperties->currentWidget() == _viewData)
    {
        _updatePropertiesDataView();
    }
    else if(_stkProperties->currentWidget() == _viewTags)
    {
        _updatePropertiesTagsView();
        _updatePropertiesAssignedTagsView();
    }
    else if(_stkProperties->currentWidget() == _viewNotes)
    {
        _updatePropertiesNotesView();
    }
    else if(_stkProperties->currentWidget() == _viewPrivileges)
    {
        _updatePropertiesPrivilegesView();
        _updatePropertiesAssignedPrivilegesView();
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

void Views::ViewUsers::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateUsersAndGroupsPriv = user->hasPrivilege("Create Users And Groups");

    _btnCreateUser->setHidden(!hasCreateUsersAndGroupsPriv);
    _btnChangeUserPassword->setHidden(!hasEditPriv);
    _btnImportUsersThumbnails->setHidden(!hasEditPriv);
    _btnCreateGroup->setHidden(!hasCreateUsersAndGroupsPriv);

    _qtvGroups->setImportCSVFeatureEnabled(hasCreateUsersAndGroupsPriv);

    _viewData->adjustUIPrivileges(user);
    _viewTags->adjustUIPrivileges(user);
    _viewNotes->adjustUIPrivileges(user);
    _viewPrivileges->adjustUIPrivileges(user);
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
void Views::ViewUsers::_addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
{
    Views::Dialogs::DlgCreateDBOData *dlg = new Views::Dialogs::DlgCreateDBOData();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto &ptr : dboVec)
            {
                Database::DboData *data = new Database::DboData(dlg->key(), dlg->value());
                Wt::Dbo::ptr<Database::DboData> dataPtr = Session::SessionManager::instance().dboSession().createDbo<Database::DboData>(data);

                if(dataPtr.get())
                    Session::SessionManager::instance().dboSession().modifyDbo<T>(ptr)->addData(dataPtr);
                else
                    delete data;
            }

            transaction.commit();

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
           Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

           for(auto &ptr : dboVec)
           {
                Database::Note *note = new Database::Note(dlg->content());
                Wt::Dbo::ptr<Database::Note> notePtr = Session::SessionManager::instance().dboSession().createDbo<Database::Note>(note);

                if(notePtr.get())
                    Session::SessionManager::instance().dboSession().modifyDbo<T>(ptr)->addNote(notePtr);
                else
                    delete note;
           }

           transaction.commit();

           _updatePropertiesNotesView();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

/*******************--Users--********************/
void Views::ViewUsers::_btnUsersCreateClicked()
{
    Views::DlgCreateUser *dlg = new Views::DlgCreateUser();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            try
            {
                if(!Session::SessionManager::instance().dboSession().dboExists<Users::User>(dlg->userName()))
                {
                    Users::User *user = new Users::User(dlg->userName(), dlg->emailAddress());
                    user->setGroup(dlg->group());
                    user->setTitle(dlg->title());
                    user->setPhoneNumber(dlg->phoneNumber());
                    user->setIdNumber(dlg->idNumber());
                    user->setAddress(dlg->address());
                    user->setActive(dlg->isActive());

                    Wt::Dbo::ptr<Users::User> userPtr = Session::SessionManager::instance().dboSession().createDbo<Users::User>(user);

                    Wt::Auth::User authUser = Session::SessionManager::instance().dboSession().users().findWithIdentity(Wt::Auth::Identity::LoginName, userPtr->name());

                    Auth::AuthManager::instance().passwordService().updatePassword(authUser, dlg->password());

                    //create user directory structure
                    Users::UsersIO::createUserDirectoryStructure(user->name());

                    updateUsersView();
                }
                else
                {
                    _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
                }
            }
            catch(Wt::Dbo::Exception ex)
            {
                _logger->log(std::string("Error occured while trying to add default users ") + ex.what() , Ms::Log::LogMessageType::Error);
            }
            catch(...)
            {
                _logger->log("Error occured while trying to add default work users", Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewUsers::_btnUsersChangePasswordClicked()
{
    if(_qtvUsers->table()->selectedIndexes().size() != 1)
    {
        _logger->log(std::string("Please select one user") , Ms::Log::LogMessageType::Warning);

        return;
    }

    int editRank = Session::SessionManager::instance().user()->editRank();

    Views::DlgChangeUserPassword *dlg = new Views::DlgChangeUserPassword();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Users::User> userPtr = _qtvUsers->selectedItems().at(0);

            if(editRank >= userPtr->editRank())
            {
                Wt::Auth::User authUser = Session::SessionManager::instance().dboSession().users().find(userPtr);

                Auth::AuthManager::instance().passwordService().updatePassword(authUser, dlg->password());
            }

            transaction.commit();
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
//            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession().session);

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
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::vector<std::string> delFiles;//holds thumbnails files for later deletion

            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
            {
                try
                {
                    //get the original raw file name without the extension
                    int lastIndex = pair.second.find_last_of(".");
                    std::string rawFileName = pair.second.substr(0, lastIndex);

                    //match thumbnail by user name

                    Wt::Dbo::ptr<Users::User> userPtr;

                    if(AppSettings::instance().isLoadInactiveData())
                        userPtr = Session::SessionManager::instance().dboSession().find<Users::User>().where("Name = ?").bind(rawFileName);
                    else
                        userPtr = Session::SessionManager::instance().dboSession().find<Users::User>().where("Name = ? AND Active = ?").bind(rawFileName).bind(true);

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

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
                _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            _qtvUsers->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewUsers::_createUsersTableView()
{
    _qtvUsers = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::User>(Session::SessionManager::instance().dboSession());
    _qtvUsers->tableSelectionChanged().connect(this, &Views::ViewUsers::updatePropertiesView);
    _qtvUsers->setRowHeight(64);
    _qtvUsers->setDefaultFilterColumnIndex(1);
    _qtvUsers->setIgnoreNumFilterColumns(1);
    _qtvUsers->setImportCSVFeatureEnabled(false);

    _btnCreateUser = _qtvUsers->createToolButton("", "icons/Add.png", "Create A New User");
    _btnCreateUser->clicked().connect(this, &Views::ViewUsers::_btnUsersCreateClicked);

    _btnChangeUserPassword = _qtvUsers->createToolButton("", "icons/Password.png", "Change User Password");
    _btnChangeUserPassword->clicked().connect(this, &Views::ViewUsers::_btnUsersChangePasswordClicked);

//    Wt::WPushButton *btn = _qtvUsers->createToolButton("", "icons/Remove.png", "Remove Selected User");
//    btn->clicked().connect(this, &Views::ViewUsers::_btnUsersRemoveClicked);

    _btnImportUsersThumbnails = _qtvUsers->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    _btnImportUsersThumbnails->clicked().connect(this, &Views::ViewUsers::_btnUsersImportThumbnailsClicked);

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
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Users::Group>(dlg->groupName()))
            {
                Users::Group *group = new Users::Group(dlg->groupName(), dlg->rank());
                Wt::Dbo::ptr<Users::Group> groupPtr = Session::SessionManager::instance().dboSession().createDbo<Users::Group>(group);

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

            transaction.commit();
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
//            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession().session);
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
    _qtvGroups = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Group>(Session::SessionManager::instance().dboSession());
    _qtvGroups->tableSelectionChanged().connect(this, &Views::ViewUsers::updatePropertiesView);
    _qtvGroups->setMinimumSize(1000, 600);

    _btnCreateGroup = _qtvGroups->createToolButton("", "icons/Add.png", "Create A New Group");
    _btnCreateGroup->clicked().connect(this, &Views::ViewUsers::_btnGroupsCreateClicked);

//    Wt::WPushButton *btn = _qtvGroups->createToolButton("", "icons/Remove.png", "Remove Selected Group");
//    btn->clicked().connect(this, &Views::ViewUsers::_btnGroupsRemoveClicked);

    updateGroupsView();
}

/*******************--Properties--********************/
void Views::ViewUsers::_mnuNavBarPropertiesDataItemTriggered()
{
    _updatePropertiesDataView();
    _stkProperties->setCurrentWidget(_viewData);
}

void Views::ViewUsers::_mnuNavBarPropertiesTagsItemTriggered()
{
    _updatePropertiesTagsView();
    _updatePropertiesAssignedTagsView();
    _stkProperties->setCurrentWidget(_viewTags);
}

void Views::ViewUsers::_mnuNavBarPropertiesNotesItemTriggered()
{
    _updatePropertiesNotesView();
    _stkProperties->setCurrentWidget(_viewNotes);
}

void Views::ViewUsers::_mnuNavBarPropertiesPrivilegesItemTriggered()
{
    _updatePropertiesPrivilegesView();
    _updatePropertiesAssignedPrivilegesView();
    _stkProperties->setCurrentWidget(_viewPrivileges);
}

void Views::ViewUsers::_addDataRequested()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() != 1)
        {
            _logger->log("Please select only one user.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            _addDataToDbo<Users::User>(_qtvUsers->selectedItems());
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() != 1)
        {
            _logger->log("Please select only one group.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            _addDataToDbo<Users::Group>(_qtvGroups->selectedItems());
    }

    transaction.commit();
}

void Views::ViewUsers::_removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec)
{

}

void Views::ViewUsers::_createTagRequested()
{
    std::string tagType = "";

    if(_stkMain->currentWidget() == _qtvUsers)
        tagType = "User";
    else if(_stkMain->currentWidget() == _qtvGroups)
        tagType = "Group";

    if(tagType == "")
        return;

    Views::Dialogs::DlgCreateTag *dlg = new Views::Dialogs::DlgCreateTag();
    dlg->finished().connect(std::bind([=]()
    {
       if(dlg->result() == Wt::WDialog::Accepted)
       {
           Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

           Database::Tag *tag = new Database::Tag(dlg->tagName(), dlg->tagContent());
           tag->setType(tagType);
           tag->setActive(dlg->isActive());

            Wt::Dbo::ptr<Database::Tag> tagPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Tag>(tag);

            if(tagPtr.get())
            {
                _updatePropertiesTagsView();

                _logger->log(std::string("Created " + tagType + " tag ") + dlg->tagName(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete tag;

                _logger->log(std::string("Error creating " + tagType + " tag ") + dlg->tagName(), Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewUsers::_assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Users::User>(_qtvUsers->selectedItems(), tagVec);

            _updatePropertiesAssignedTagsView();
        }
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Users::Group>(_qtvGroups->selectedItems(), tagVec);

            _updatePropertiesAssignedTagsView();
        }
    }

    transaction.commit();
}

void Views::ViewUsers::_unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Users::User>(_qtvUsers->selectedItems(), tagVec);

            _updatePropertiesAssignedTagsView();
        }
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Users::Group>(_qtvGroups->selectedItems(), tagVec);

            _updatePropertiesAssignedTagsView();
        }
    }

    transaction.commit();
}

void Views::ViewUsers::_filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec, bool exactSelection, bool inverse)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string strFilterQuery = "";

    if(tagVec.size() == 0)
        return;

    std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Database::Tag>(tagVec);

    std::string inverseOperator = inverse ? "NOT " : "";

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        std::string matchBy = exactSelection ? " GROUP BY ut.user_Name HAVING COUNT(DISTINCT ut.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Name " + inverseOperator + "IN (SELECT ut.user_Name FROM rel_user_assigned_tags ut WHERE tag_id IN (" + idValues.at(0) + ")" + matchBy + ")";

        _qtvUsers->setCustomFilterString(strFilterQuery);
        _qtvUsers->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        std::string matchBy = exactSelection ? " GROUP BY gt.group_Name HAVING COUNT(DISTINCT gt.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Name " + inverseOperator + "IN (SELECT gt.group_Name FROM rel_group_assigned_tags gt WHERE tag_id IN (" + idValues.at(0) + ")" + matchBy + ")";

        _qtvGroups->setCustomFilterString(strFilterQuery);
        _qtvGroups->setCustomFilterActive(true);
    }

    transaction.commit();
}

void Views::ViewUsers::_clearTagsFilterRequested()
{
    if(_stkMain->currentWidget() == _qtvUsers)
    {
        _qtvUsers->setCustomFilterString("");
        _qtvUsers->setCustomFilterActive(false);
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        _qtvGroups->setCustomFilterString("");
        _qtvGroups->setCustomFilterActive(false);
    }
}

void Views::ViewUsers::_addNoteRequested()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() != 1)
        {
            _logger->log("Please select only one user.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            _addNoteToDbo<Users::User>(_qtvUsers->selectedItems());
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() != 1)
        {
            _logger->log("Please select only one group.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            _addNoteToDbo<Users::Group>(_qtvGroups->selectedItems());
    }

    transaction.commit();
}

void Views::ViewUsers::_removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec)
{

}

void Views::ViewUsers::_assignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    if(_qtvGroups->table()->selectedIndexes().size() > 0)
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        for(auto &grpPtr : _qtvGroups->selectedItems())
        {
            for(auto &prvPtr : privVec)
            {
                Session::SessionManager::instance().dboSession().modifyDbo<Users::Group>(grpPtr)->addPrivilege(prvPtr);
            }
        }

        transaction.commit();

        _updatePropertiesAssignedPrivilegesView();
    }
}

void Views::ViewUsers::_unassignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    if(_qtvGroups->table()->selectedIndexes().size() > 0)
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        for(auto &grpPtr : _qtvGroups->selectedItems())
        {
            for(auto &prvPtr : privVec)
            {
                Session::SessionManager::instance().dboSession().modifyDbo<Users::Group>(grpPtr)->removePrivilege(prvPtr);
            }
        }

        transaction.commit();

        _updatePropertiesAssignedPrivilegesView();
    }
}

void Views::ViewUsers::_filterByPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec, bool exactSelection, bool inverse)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string strFilterQuery = "";

    if(privVec.size() == 0)
        return;

    std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Privilege>(privVec);

    std::string matchBy = exactSelection ? " GROUP BY gp.group_Name HAVING COUNT(DISTINCT gp.privilege_Name) = " + std::to_string(privVec.size()) : "";

    std::string inverseOperator = inverse ? "NOT " : "";

    strFilterQuery = "Name " + inverseOperator + "IN (SELECT gp.group_Name FROM rel_group_privileges gp WHERE privilege_Name IN (" + idValues.at(0) + ")" + matchBy + ")";

    _qtvGroups->setCustomFilterString(strFilterQuery);
    _qtvGroups->setCustomFilterActive(true);

    transaction.commit();
}

void Views::ViewUsers::_clearFilterPrivilegesFilterRequested()
{
    _qtvGroups->setCustomFilterString("");
    _qtvGroups->setCustomFilterActive(false);
}

void Views::ViewUsers::_createPropertiesView()
{
    _cntPropertiesMain = new Wt::WContainerWidget();

    _layCntPropertiesMain = new Wt::WVBoxLayout();
    _layCntPropertiesMain->setContentsMargins(0,0,0,0);
    _layCntPropertiesMain->setSpacing(0);

    _cntPropertiesMain->setLayout(_layCntPropertiesMain);

    _navBarProperties = new Wt::WNavigationBar();

    _layCntPropertiesMain->addWidget(_navBarProperties);

    _mnuNavBarProperties = new Wt::WMenu(Wt::Horizontal);

    _navBarProperties->addMenu(_mnuNavBarProperties);

    _mnuNavBarPropertiesDataItem = new Wt::WMenuItem("Data");
    _mnuNavBarPropertiesDataItem->triggered().connect(this, &Views::ViewUsers::_mnuNavBarPropertiesDataItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesDataItem);

    _mnuNavBarPropertiesTagsItem = new Wt::WMenuItem("Tags");
    _mnuNavBarPropertiesTagsItem->triggered().connect(this, &Views::ViewUsers::_mnuNavBarPropertiesTagsItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesTagsItem);

    _mnuNavBarPropertiesNotesItem = new Wt::WMenuItem("Notes");
    _mnuNavBarPropertiesNotesItem->triggered().connect(this, &Views::ViewUsers::_mnuNavBarPropertiesNotesItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesNotesItem);

    _mnuNavBarPropertiesPrivilegesItem = new Wt::WMenuItem("Privileges");
    _mnuNavBarPropertiesPrivilegesItem->triggered().connect(this, &Views::ViewUsers::_mnuNavBarPropertiesPrivilegesItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesPrivilegesItem);

    _stkProperties = new Wt::WStackedWidget();

    _layCntPropertiesMain->addWidget(_stkProperties, 1);

    _viewData = new Views::ViewDboData();
    _viewData->addDataRequested().connect(this, &Views::ViewUsers::_addDataRequested);
    _viewData->removeDataRequested().connect(this, &Views::ViewUsers::_removeDataRequested);

    _stkProperties->addWidget(_viewData);

    _viewTags = new Views::ViewTags();
    _viewTags->createTagRequested().connect(this, &Views::ViewUsers::_createTagRequested);
    _viewTags->assignTagsRequested().connect(this, &Views::ViewUsers::_assignTagsRequested);
    _viewTags->unassignTagsRequested().connect(this, &Views::ViewUsers::_unassignTagsRequested);
    _viewTags->filterByTagsRequested().connect(this, &Views::ViewUsers::_filterByTagsRequested);
    _viewTags->clearTagsFilterRequested().connect(this, &Views::ViewUsers::_clearTagsFilterRequested);

    _stkProperties->addWidget(_viewTags);

    _viewNotes = new Views::ViewNotes();
    _viewNotes->addNoteRequested().connect(this, &Views::ViewUsers::_addNoteRequested);
    _viewNotes->removeNotesRequested().connect(this, &Views::ViewUsers::_removeNotesRequested);

    _stkProperties->addWidget(_viewNotes);

    _viewPrivileges = new Views::ViewPrivileges();
    _viewPrivileges->assignPrivilegesRequested().connect(this, &Views::ViewUsers::_assignPrivilegesRequested);
    _viewPrivileges->unassignPrivilegesRequested().connect(this, &Views::ViewUsers::_unassignPrivilegesRequested);
    _viewPrivileges->filterByPrivilegesRequested().connect(this, &Views::ViewUsers::_filterByPrivilegesRequested);
    _viewPrivileges->clearPrivilegesFilterRequested().connect(this, &Views::ViewUsers::_clearFilterPrivilegesFilterRequested);

    _stkProperties->addWidget(_viewPrivileges);

    _mnuNavBarPropertiesDataItem->select();//default selected item

    _propertiesPanel->addPropertiesView(this->id(), _cntPropertiesMain);
}

void Views::ViewUsers::_updatePropertiesDataView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvPropertiesData = _viewData->qtvData();

    _qtvPropertiesData->clearColumns();

    //add columns
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Session::SessionManager::instance().dboSession().find<Database::DboData>();

    bool update = false;

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("User_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Group_Name", "Group Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }

    if(update)
    {
        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesData->setQuery(query);

    transaction.commit();

    if(AppSettings::instance().isShowExtraColumns())
        _qtvPropertiesData->addBaseColumns(flags, editRank);

    _qtvPropertiesData->updateView();
}

void Views::ViewUsers::_updatePropertiesTagsView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesTags = _viewTags->qtvTags();

    _qtvPropertiesTags->clearColumns();

    //add columns
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Session::SessionManager::instance().dboSession().find<Database::Tag>();

    if(_stkMain->currentWidget() == _qtvUsers)
        query.where("(Type IN ('User', 'Global'))");
    else if(_stkMain->currentWidget() == _qtvGroups)
        query.where("(Type IN ('Group', 'Global'))");

    int viewRank = Session::SessionManager::instance().user()->viewRank();

    if(!AppSettings::instance().isLoadInactiveData())
        query.where("Active = ?").bind(true);

    query.where("View_Rank <= ?").bind(viewRank);

    _qtvPropertiesTags->setQuery(query);

    transaction.commit();

    if(AppSettings::instance().isShowExtraColumns())
        _qtvPropertiesTags->addBaseColumns(flags, editRank);

    _qtvPropertiesTags->updateView();
}

void Views::ViewUsers::_updatePropertiesAssignedTagsView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesAssignedTags = _viewTags->qtvAssignedTags();

    _qtvPropertiesAssignedTags->clearColumns();

    //add columns
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Session::SessionManager::instance().dboSession().find<Database::Tag>();

    bool update = false;

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string usersSelectSql = "(pt.user_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + "))";

            query.where("(id IN (SELECT pt.tag_id FROM rel_user_assigned_tags pt WHERE " + usersSelectSql + "))");
        }
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string groupsSelectSql = "pt.group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")";

            query.where("(id IN (SELECT pt.tag_id FROM rel_group_assigned_tags pt WHERE " + groupsSelectSql + "))");
        }
    }

    if(update)
    {
        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesAssignedTags->setQuery(query);

    transaction.commit();

    if(AppSettings::instance().isShowExtraColumns())
        _qtvPropertiesAssignedTags->addBaseColumns(flags, editRank);

    _qtvPropertiesAssignedTags->updateView();
}

void Views::ViewUsers::_updatePropertiesNotesView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvPropertiesNotes = _viewNotes->qtvNotes();

    _qtvPropertiesNotes->clearColumns();

    //add columns
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Session::SessionManager::instance().dboSession().find<Database::Note>();

    bool update = false;

    if(_stkMain->currentWidget() == _qtvUsers)
    {
        if(_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("User_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::User>(_qtvUsers->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _qtvGroups)
    {
        if(_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Group_Name", "Group Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }

    if(update)
    {
        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(!AppSettings::instance().isLoadInactiveData())
            query.where("Active = ?").bind(true);

        query.where("View_Rank <= ?").bind(viewRank);
    }
    else
        query.where("id = ?").bind(-1);

    _qtvPropertiesNotes->setQuery(query);

    transaction.commit();

    if(AppSettings::instance().isShowExtraColumns())
        _qtvPropertiesNotes->addBaseColumns(flags, editRank);

    _qtvPropertiesNotes->updateView();
}

void Views::ViewUsers::_updatePropertiesPrivilegesView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query;

    int viewRank = Session::SessionManager::instance().user()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::Privilege>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Session::SessionManager::instance().dboSession().find<Users::Privilege>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsPrivileges = _viewPrivileges->qtvPrivileges();

    _qtvGroupsPrivileges->setQuery(query);

    transaction.commit();

    _qtvGroupsPrivileges->clearColumns();

    //add columns
    _qtvGroupsPrivileges->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

    if(AppSettings::instance().isShowExtraColumns())
        _qtvGroupsPrivileges->addBaseColumns(Wt::ItemIsSelectable, 0);

    _qtvGroupsPrivileges->removeColumn(Ms::Widgets::MQueryTableViewColumn("Active"));

    _qtvGroupsPrivileges->updateView();
}

void Views::ViewUsers::_updatePropertiesAssignedPrivilegesView()
{
    if(_qtvGroups->table()->selectedIndexes().size() == 0)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string groupsSelectSql = "gp.group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(_qtvGroups->selectedItems()).at(0) + ")";

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query = Session::SessionManager::instance().dboSession().find<Users::Privilege>().where(
                "Name IN (SELECT gp.privilege_Name FROM rel_group_privileges gp WHERE " + groupsSelectSql + ")");

    if(!AppSettings::instance().isLoadInactiveData())
        query.where("Active = ?").bind(true);

    Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvPropertiesGroupsAssignedPrivileges = _viewPrivileges->qtvAssignedPrivileges();

    _qtvPropertiesGroupsAssignedPrivileges->setQuery(query);

    transaction.commit();

    _qtvPropertiesGroupsAssignedPrivileges->clearColumns();

    //add columns
    _qtvPropertiesGroupsAssignedPrivileges->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), true));

    if(AppSettings::instance().isShowExtraColumns())
        _qtvPropertiesGroupsAssignedPrivileges->addBaseColumns(Wt::ItemIsSelectable, 0);

    _qtvPropertiesGroupsAssignedPrivileges->removeColumn(Ms::Widgets::MQueryTableViewColumn("Active"));

    _qtvPropertiesGroupsAssignedPrivileges->updateView();
}

/*******************--Main--********************/
void Views::ViewUsers::_mnuMainUsersItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvUsers);

    if(_stkProperties->currentWidget() == _viewPrivileges)
        _mnuNavBarPropertiesDataItem->select();

    _mnuNavBarPropertiesPrivilegesItem->hide();

    updateUsersView();

    _onTabUsersSelected();
}

void Views::ViewUsers::_mnuMainGroupsItemTriggered()
{
    _stkMain->setCurrentWidget(_qtvGroups);

    _mnuNavBarPropertiesPrivilegesItem->show();

    updateGroupsView();

    _onTabGroupsSelected();
}

void Views::ViewUsers::_prepareView()
{
    /*******************--Main--********************/
    setTitle("<b><i>Users And Groups</i></b>");

    Wt::WVBoxLayout* _layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    _layMain->setContentsMargins(14,0,14,14);

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

