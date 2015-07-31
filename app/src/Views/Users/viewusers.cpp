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
    m_logger = Session::SessionManager::instance().logger();
    m_propertiesPanel = Session::SessionManager::instance().propertiesPanel();

    prepareView();
    m_mnuMain->select(m_mnuMainUsersItem);
}

void Views::ViewUsers::updateView()
{
    if(m_stkMain->currentWidget() == m_qtvUsers)
        updateUsersView();
    else if(m_stkMain->currentWidget() == m_qtvGroups)
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

    m_qtvUsers->setQuery(query);

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    m_qtvUsers->clearColumns();

    //add columns
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(100, 64, "pics/NoPreview.png"), false, true, 100));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Group_Name", "Group", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::Group>(
     &Session::SessionManager::instance().dboSession() ,
     AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::Group>() :
     Session::SessionManager::instance().dboSession().find<Users::Group>().where("Active = ?").bind(true),
     "Name", editRank), true));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Title_Name", "Title", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::UserTitle>(
     &Session::SessionManager::instance().dboSession(),
     AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::UserTitle>() :
     Session::SessionManager::instance().dboSession().find<Users::UserTitle>().where("Active = ?").bind(true),
     "Name", editRank)));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Email_Address", "Email", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Phone_Number", "Phone Number", flags, new Ms::Widgets::Delegates::MValidatorFieldDelegate("[0-9]{1,255}", true, editRank)));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Id_Number", "Id Number", flags, new Ms::Widgets::Delegates::MValidatorFieldDelegate("[0-9]{1,255}", true, editRank)));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Available", "Available", flags, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank)));
    m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Address", "Address", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));

    if(AppSettings::instance().isShowExtraColumns())
    {
        m_qtvUsers->addColumn(Ms::Widgets::MQueryTableViewColumn("Create_Rank", "Create Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));

        m_qtvUsers->addBaseColumns(flags, editRank);
    }

    transaction.commit();

    m_qtvUsers->updateView();
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

    m_qtvGroups->setQuery(query);

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    m_qtvGroups->clearColumns();

    //add columns
    m_qtvGroups->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    m_qtvGroups->addColumn(Ms::Widgets::MQueryTableViewColumn("Rank", "Rank", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    if(AppSettings::instance().isShowExtraColumns())
        m_qtvGroups->addBaseColumns(flags, editRank);

    transaction.commit();

    m_qtvGroups->updateView();
}

void Views::ViewUsers::updatePropertiesView()
{
    if(m_stkProperties->currentWidget() == m_viewData)
    {
        updatePropertiesDataView();
    }
    else if(m_stkProperties->currentWidget() == m_viewTags)
    {
        updatePropertiesTagsView();
        updatePropertiesAssignedTagsView();
    }
    else if(m_stkProperties->currentWidget() == m_viewNotes)
    {
        updatePropertiesNotesView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPrivileges)
    {
        updatePropertiesPrivilegesView();
        updatePropertiesAssignedPrivilegesView();
    }
}

bool Views::ViewUsers::isUsersViewShown()
{
    return m_stkMain->currentWidget() == m_qtvUsers;
}

bool Views::ViewUsers::isGroupsViewShown()
{
    return m_stkMain->currentWidget() == m_qtvGroups;
}

Ms::Widgets::MQueryTableViewWidget<Users::User> *Views::ViewUsers::usersQueryTableView() const
{
    return m_qtvUsers;
}

Ms::Widgets::MQueryTableViewWidget<Users::Group> *Views::ViewUsers::groupsQueryTableView() const
{
    return m_qtvGroups;
}

void Views::ViewUsers::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateUsersAndGroupsPriv = user->hasPrivilege("Create Users And Groups");

    m_btnCreateUser->setHidden(!hasCreateUsersAndGroupsPriv);
    m_btnChangeUserPassword->setHidden(!hasEditPriv);
    m_btnImportUsersThumbnails->setHidden(!hasEditPriv);
    m_btnCreateGroup->setHidden(!hasCreateUsersAndGroupsPriv);

    m_qtvGroups->setImportCSVFeatureEnabled(hasCreateUsersAndGroupsPriv);

    m_viewData->adjustUIPrivileges(user);
    m_viewTags->adjustUIPrivileges(user);
    m_viewNotes->adjustUIPrivileges(user);
    m_viewPrivileges->adjustUIPrivileges(user);
}

Wt::Signal<> &Views::ViewUsers::onTabUsersSelected()
{
    return m_onTabUsersSelected;
}

Wt::Signal<> &Views::ViewUsers::onTabGroupsSelected()
{
    return m_onTabGroupsSelected;
}

template<typename T>
void Views::ViewUsers::addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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

            updatePropertiesDataView();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

template<typename T>
void Views::ViewUsers::addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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

           updatePropertiesNotesView();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

/*******************--Users--********************/
void Views::ViewUsers::btnUsersCreateClicked()
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
                    m_logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
                }
            }
            catch(Wt::Dbo::Exception ex)
            {
                m_logger->log(std::string("Error occured while trying to add default users ") + ex.what() , Ms::Log::LogMessageType::Error);
            }
            catch(...)
            {
                m_logger->log("Error occured while trying to add default work users", Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewUsers::btnUsersChangePasswordClicked()
{
    if(m_qtvUsers->table()->selectedIndexes().size() != 1)
    {
        m_logger->log(std::string("Please select one user") , Ms::Log::LogMessageType::Warning);

        return;
    }

    int editRank = Session::SessionManager::instance().user()->editRank();

    Views::DlgChangeUserPassword *dlg = new Views::DlgChangeUserPassword();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Users::User> userPtr = m_qtvUsers->selectedItems().at(0);

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

void Views::ViewUsers::btnUsersRemoveClicked()
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

void Views::ViewUsers::btnUsersImportThumbnailsClicked()
{
    if(m_qtvUsers->model()->rowCount() == 0)
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
                    m_logger->log(std::string("Error occured while trying to import thumbnails to table users") + e.what(),
                                 Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
                }

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
                m_logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            m_qtvUsers->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewUsers::createUsersTableView()
{
    m_qtvUsers = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::User>(Session::SessionManager::instance().dboSession());
    m_qtvUsers->tableSelectionChanged().connect(this, &Views::ViewUsers::updatePropertiesView);
    m_qtvUsers->setRowHeight(64);
    m_qtvUsers->setDefaultFilterColumnIndex(1);
    m_qtvUsers->setIgnoreNumFilterColumns(1);
    m_qtvUsers->setImportCSVFeatureEnabled(false);

    m_btnCreateUser = m_qtvUsers->createToolButton("", "icons/Add.png", "Create A New User");
    m_btnCreateUser->clicked().connect(this, &Views::ViewUsers::btnUsersCreateClicked);

    m_btnChangeUserPassword = m_qtvUsers->createToolButton("", "icons/Password.png", "Change User Password");
    m_btnChangeUserPassword->clicked().connect(this, &Views::ViewUsers::btnUsersChangePasswordClicked);

//    Wt::WPushButton *btn = m_qtvUsers->createToolButton("", "icons/Remove.png", "Remove Selected User");
//    btn->clicked().connect(this, &Views::ViewUsers::btnUsersRemoveClicked);

    m_btnImportUsersThumbnails = m_qtvUsers->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    m_btnImportUsersThumbnails->clicked().connect(this, &Views::ViewUsers::btnUsersImportThumbnailsClicked);

    updateUsersView();
}

/*******************--Groups--********************/

void Views::ViewUsers::btnGroupsCreateClicked()
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
                    m_logger->log(std::string("Created group ") + dlg->groupName(), Ms::Log::LogMessageType::Info);

                    updateGroupsView();
                }
                else
                {
                    delete group;

                    m_logger->log(std::string("error creating group ") + dlg->groupName(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::ViewUsers::btnGroupsRemoveClicked()
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

void Views::ViewUsers::createGroupsTableView()
{
    m_qtvGroups = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Users::Group>(Session::SessionManager::instance().dboSession());
    m_qtvGroups->tableSelectionChanged().connect(this, &Views::ViewUsers::updatePropertiesView);
    m_qtvGroups->setMinimumSize(1000, 600);

    m_btnCreateGroup = m_qtvGroups->createToolButton("", "icons/Add.png", "Create A New Group");
    m_btnCreateGroup->clicked().connect(this, &Views::ViewUsers::btnGroupsCreateClicked);

//    Wt::WPushButton *btn = m_qtvGroups->createToolButton("", "icons/Remove.png", "Remove Selected Group");
//    btn->clicked().connect(this, &Views::ViewUsers::btnGroupsRemoveClicked);

    updateGroupsView();
}

/*******************--Properties--********************/
void Views::ViewUsers::mnuNavBarPropertiesDataItemTriggered()
{
    updatePropertiesDataView();
    m_stkProperties->setCurrentWidget(m_viewData);
}

void Views::ViewUsers::mnuNavBarPropertiesTagsItemTriggered()
{
    updatePropertiesTagsView();
    updatePropertiesAssignedTagsView();
    m_stkProperties->setCurrentWidget(m_viewTags);
}

void Views::ViewUsers::mnuNavBarPropertiesNotesItemTriggered()
{
    updatePropertiesNotesView();
    m_stkProperties->setCurrentWidget(m_viewNotes);
}

void Views::ViewUsers::mnuNavBarPropertiesPrivilegesItemTriggered()
{
    updatePropertiesPrivilegesView();
    updatePropertiesAssignedPrivilegesView();
    m_stkProperties->setCurrentWidget(m_viewPrivileges);
}

void Views::ViewUsers::addDataRequested()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() != 1)
        {
            m_logger->log("Please select only one user.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            addDataToDbo<Users::User>(m_qtvUsers->selectedItems());
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() != 1)
        {
            m_logger->log("Please select only one group.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            addDataToDbo<Users::Group>(m_qtvGroups->selectedItems());
    }

    transaction.commit();
}

void Views::ViewUsers::removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec)
{

}

void Views::ViewUsers::createTagRequested()
{
    std::string tagType = "";

    if(m_stkMain->currentWidget() == m_qtvUsers)
        tagType = "User";
    else if(m_stkMain->currentWidget() == m_qtvGroups)
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
                updatePropertiesTagsView();

                m_logger->log(std::string("Created " + tagType + " tag ") + dlg->tagName(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete tag;

                m_logger->log(std::string("Error creating " + tagType + " tag ") + dlg->tagName(), Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewUsers::assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Users::User>(m_qtvUsers->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Users::Group>(m_qtvGroups->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }

    transaction.commit();
}

void Views::ViewUsers::unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Users::User>(m_qtvUsers->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Users::Group>(m_qtvGroups->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }

    transaction.commit();
}

void Views::ViewUsers::filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec, bool exactSelection, bool inverse)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string strFilterQuery = "";

    if(tagVec.size() == 0)
        return;

    std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Database::Tag>(tagVec);

    std::string inverseOperator = inverse ? "NOT " : "";

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        std::string matchBy = exactSelection ? " GROUP BY ut.user_Name HAVING COUNT(DISTINCT ut.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Name " + inverseOperator + "IN (SELECT ut.user_Name FROM rel_user_assigned_tags ut WHERE tag_id IN (" + idValues.at(0) + ")" + matchBy + ")";

        m_qtvUsers->setCustomFilterString(strFilterQuery);
        m_qtvUsers->setCustomFilterActive(true);
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        std::string matchBy = exactSelection ? " GROUP BY gt.group_Name HAVING COUNT(DISTINCT gt.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Name " + inverseOperator + "IN (SELECT gt.group_Name FROM rel_group_assigned_tags gt WHERE tag_id IN (" + idValues.at(0) + ")" + matchBy + ")";

        m_qtvGroups->setCustomFilterString(strFilterQuery);
        m_qtvGroups->setCustomFilterActive(true);
    }

    transaction.commit();
}

void Views::ViewUsers::clearTagsFilterRequested()
{
    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        m_qtvUsers->setCustomFilterString("");
        m_qtvUsers->setCustomFilterActive(false);
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        m_qtvGroups->setCustomFilterString("");
        m_qtvGroups->setCustomFilterActive(false);
    }
}

void Views::ViewUsers::addNoteRequested()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() != 1)
        {
            m_logger->log("Please select only one user.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            addNoteToDbo<Users::User>(m_qtvUsers->selectedItems());
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() != 1)
        {
            m_logger->log("Please select only one group.", Ms::Log::LogMessageType::Warning);

            return;
        }
        else
            addNoteToDbo<Users::Group>(m_qtvGroups->selectedItems());
    }

    transaction.commit();
}

void Views::ViewUsers::removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec)
{

}

void Views::ViewUsers::assignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    if(m_qtvGroups->table()->selectedIndexes().size() > 0)
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        for(auto &grpPtr : m_qtvGroups->selectedItems())
        {
            for(auto &prvPtr : privVec)
            {
                Session::SessionManager::instance().dboSession().modifyDbo<Users::Group>(grpPtr)->addPrivilege(prvPtr);
            }
        }

        transaction.commit();

        updatePropertiesAssignedPrivilegesView();
    }
}

void Views::ViewUsers::unassignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec)
{
    if(m_qtvGroups->table()->selectedIndexes().size() > 0)
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        for(auto &grpPtr : m_qtvGroups->selectedItems())
        {
            for(auto &prvPtr : privVec)
            {
                Session::SessionManager::instance().dboSession().modifyDbo<Users::Group>(grpPtr)->removePrivilege(prvPtr);
            }
        }

        transaction.commit();

        updatePropertiesAssignedPrivilegesView();
    }
}

void Views::ViewUsers::filterByPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec, bool exactSelection, bool inverse)
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string strFilterQuery = "";

    if(privVec.size() == 0)
        return;

    std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Privilege>(privVec);

    std::string matchBy = exactSelection ? " GROUP BY gp.group_Name HAVING COUNT(DISTINCT gp.privilege_Name) = " + std::to_string(privVec.size()) : "";

    std::string inverseOperator = inverse ? "NOT " : "";

    strFilterQuery = "Name " + inverseOperator + "IN (SELECT gp.group_Name FROM rel_group_privileges gp WHERE privilege_Name IN (" + idValues.at(0) + ")" + matchBy + ")";

    m_qtvGroups->setCustomFilterString(strFilterQuery);
    m_qtvGroups->setCustomFilterActive(true);

    transaction.commit();
}

void Views::ViewUsers::clearFilterPrivilegesFilterRequested()
{
    m_qtvGroups->setCustomFilterString("");
    m_qtvGroups->setCustomFilterActive(false);
}

void Views::ViewUsers::createPropertiesView()
{
    m_cntPropertiesMain = new Wt::WContainerWidget();

    m_layCntPropertiesMain = new Wt::WVBoxLayout();
    m_layCntPropertiesMain->setContentsMargins(0,0,0,0);
    m_layCntPropertiesMain->setSpacing(0);

    m_cntPropertiesMain->setLayout(m_layCntPropertiesMain);

    m_navBarProperties = new Wt::WNavigationBar();

    m_layCntPropertiesMain->addWidget(m_navBarProperties);

    m_mnuNavBarProperties = new Wt::WMenu(Wt::Horizontal);

    m_navBarProperties->addMenu(m_mnuNavBarProperties);

    m_mnuNavBarPropertiesDataItem = new Wt::WMenuItem("Data");
    m_mnuNavBarPropertiesDataItem->triggered().connect(this, &Views::ViewUsers::mnuNavBarPropertiesDataItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesDataItem);

    m_mnuNavBarPropertiesTagsItem = new Wt::WMenuItem("Tags");
    m_mnuNavBarPropertiesTagsItem->triggered().connect(this, &Views::ViewUsers::mnuNavBarPropertiesTagsItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesTagsItem);

    m_mnuNavBarPropertiesNotesItem = new Wt::WMenuItem("Notes");
    m_mnuNavBarPropertiesNotesItem->triggered().connect(this, &Views::ViewUsers::mnuNavBarPropertiesNotesItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesNotesItem);

    m_mnuNavBarPropertiesPrivilegesItem = new Wt::WMenuItem("Privileges");
    m_mnuNavBarPropertiesPrivilegesItem->triggered().connect(this, &Views::ViewUsers::mnuNavBarPropertiesPrivilegesItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesPrivilegesItem);

    m_stkProperties = new Wt::WStackedWidget();

    m_layCntPropertiesMain->addWidget(m_stkProperties, 1);

    m_viewData = new Views::ViewDboData();
    m_viewData->addDataRequested().connect(this, &Views::ViewUsers::addDataRequested);
    m_viewData->removeDataRequested().connect(this, &Views::ViewUsers::removeDataRequested);

    m_stkProperties->addWidget(m_viewData);

    m_viewTags = new Views::ViewTags();
    m_viewTags->createTagRequested().connect(this, &Views::ViewUsers::createTagRequested);
    m_viewTags->assignTagsRequested().connect(this, &Views::ViewUsers::assignTagsRequested);
    m_viewTags->unassignTagsRequested().connect(this, &Views::ViewUsers::unassignTagsRequested);
    m_viewTags->filterByTagsRequested().connect(this, &Views::ViewUsers::filterByTagsRequested);
    m_viewTags->clearTagsFilterRequested().connect(this, &Views::ViewUsers::clearTagsFilterRequested);

    m_stkProperties->addWidget(m_viewTags);

    m_viewNotes = new Views::ViewNotes();
    m_viewNotes->addNoteRequested().connect(this, &Views::ViewUsers::addNoteRequested);
    m_viewNotes->removeNotesRequested().connect(this, &Views::ViewUsers::removeNotesRequested);

    m_stkProperties->addWidget(m_viewNotes);

    m_viewPrivileges = new Views::ViewPrivileges();
    m_viewPrivileges->assignPrivilegesRequested().connect(this, &Views::ViewUsers::assignPrivilegesRequested);
    m_viewPrivileges->unassignPrivilegesRequested().connect(this, &Views::ViewUsers::unassignPrivilegesRequested);
    m_viewPrivileges->filterByPrivilegesRequested().connect(this, &Views::ViewUsers::filterByPrivilegesRequested);
    m_viewPrivileges->clearPrivilegesFilterRequested().connect(this, &Views::ViewUsers::clearFilterPrivilegesFilterRequested);

    m_stkProperties->addWidget(m_viewPrivileges);

    m_mnuNavBarPropertiesDataItem->select();//default selected item

    m_propertiesPanel->addPropertiesView(this->id(), m_cntPropertiesMain);
}

void Views::ViewUsers::updatePropertiesDataView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvPropertiesData = m_viewData->qtvData();

    _qtvPropertiesData->clearColumns();

    //add columns
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Session::SessionManager::instance().dboSession().find<Database::DboData>();

    bool update = false;

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("User_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::User>(m_qtvUsers->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(m_qtvGroups->selectedItems()).at(0) + ")");
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

void Views::ViewUsers::updatePropertiesTagsView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesTags = m_viewTags->qtvTags();

    _qtvPropertiesTags->clearColumns();

    //add columns
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Session::SessionManager::instance().dboSession().find<Database::Tag>();

    if(m_stkMain->currentWidget() == m_qtvUsers)
        query.where("(Type IN ('User', 'Global'))");
    else if(m_stkMain->currentWidget() == m_qtvGroups)
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

void Views::ViewUsers::updatePropertiesAssignedTagsView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesAssignedTags = m_viewTags->qtvAssignedTags();

    _qtvPropertiesAssignedTags->clearColumns();

    //add columns
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Session::SessionManager::instance().dboSession().find<Database::Tag>();

    bool update = false;

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string usersSelectSql = "(pt.user_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::User>(m_qtvUsers->selectedItems()).at(0) + "))";

            query.where("(id IN (SELECT pt.tag_id FROM rel_user_assigned_tags pt WHERE " + usersSelectSql + "))");
        }
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::string groupsSelectSql = "pt.group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(m_qtvGroups->selectedItems()).at(0) + ")";

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

void Views::ViewUsers::updatePropertiesNotesView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvPropertiesNotes = m_viewNotes->qtvNotes();

    _qtvPropertiesNotes->clearColumns();

    //add columns
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Session::SessionManager::instance().dboSession().find<Database::Note>();

    bool update = false;

    if(m_stkMain->currentWidget() == m_qtvUsers)
    {
        if(m_qtvUsers->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("User_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::User>(m_qtvUsers->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("User_Name", "User Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_qtvGroups)
    {
        if(m_qtvGroups->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(m_qtvGroups->selectedItems()).at(0) + ")");
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

void Views::ViewUsers::updatePropertiesPrivilegesView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query;

    int viewRank = Session::SessionManager::instance().user()->viewRank();

    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Users::Privilege>().where("View_Rank <= ?").bind(viewRank);
    else
        query = Session::SessionManager::instance().dboSession().find<Users::Privilege>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

    Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsPrivileges = m_viewPrivileges->qtvPrivileges();

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

void Views::ViewUsers::updatePropertiesAssignedPrivilegesView()
{
    if(m_qtvGroups->table()->selectedIndexes().size() == 0)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string groupsSelectSql = "gp.group_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Users::Group>(m_qtvGroups->selectedItems()).at(0) + ")";

    Wt::Dbo::Query<Wt::Dbo::ptr<Users::Privilege>> query = Session::SessionManager::instance().dboSession().find<Users::Privilege>().where(
                "Name IN (SELECT gp.privilege_Name FROM rel_group_privileges gp WHERE " + groupsSelectSql + ")");

    if(!AppSettings::instance().isLoadInactiveData())
        query.where("Active = ?").bind(true);

    Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvPropertiesGroupsAssignedPrivileges = m_viewPrivileges->qtvAssignedPrivileges();

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
void Views::ViewUsers::mnuMainUsersItemTriggered()
{
    m_stkMain->setCurrentWidget(m_qtvUsers);

    if(m_stkProperties->currentWidget() == m_viewPrivileges)
        m_mnuNavBarPropertiesDataItem->select();

    m_mnuNavBarPropertiesPrivilegesItem->hide();

    updateUsersView();

    m_onTabUsersSelected();
}

void Views::ViewUsers::mnuMainGroupsItemTriggered()
{
    m_stkMain->setCurrentWidget(m_qtvGroups);

    m_mnuNavBarPropertiesPrivilegesItem->show();

    updateGroupsView();

    m_onTabGroupsSelected();
}

void Views::ViewUsers::prepareView()
{
    /*******************--Main--********************/
    setTitle("<b><i>Users And Groups</i></b>");

    Wt::WVBoxLayout* m_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    m_layMain->setContentsMargins(14,0,14,14);

    m_navBarMain = new Wt::WNavigationBar();

    m_mnuMain = new Wt::WMenu(Wt::Horizontal);

    m_navBarMain->addMenu(m_mnuMain);

    m_mnuMainUsersItem = new Wt::WMenuItem("Users");
    m_mnuMainUsersItem->triggered().connect(this, &Views::ViewUsers::mnuMainUsersItemTriggered);
    m_mnuMain->addItem(m_mnuMainUsersItem);

    m_mnuMainGroupsItem = new Wt::WMenuItem("Groups");
    m_mnuMainGroupsItem->triggered().connect(this, &Views::ViewUsers::mnuMainGroupsItemTriggered);
    m_mnuMain->addItem(m_mnuMainGroupsItem);

    m_layMain->addWidget(m_navBarMain);

    m_stkMain = new Wt::WStackedWidget();
    m_stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    m_layMain->addWidget(m_stkMain, 1);

    /*******************--Users--********************/
    createUsersTableView();

    m_stkMain->addWidget(m_qtvUsers);

    /*******************--Groups--********************/
    createGroupsTableView();

    m_stkMain->addWidget(m_qtvGroups);

    /*******************--Properties--********************/
    createPropertiesView();
}

