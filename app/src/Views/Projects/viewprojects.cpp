#include "viewprojects.h"

#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsmanager.h"
#include "../../Projects/projectsio.h"
#include "Users/usersmanager.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "../../Auth/authmanager.h"
#include "../Dialogs/dlgcreatedbodata.h"
#include "../Dialogs/dlgcreatenote.h"
#include "../../Widgets/Delegates/workstatusquerycomboboxdelegate.h"
#include "../Dialogs/dlgcreatetag.h"

#include <Ms/Widgets/MQueryTableViewColumn.h>
#include <Ms/Widgets/Delegates/MDelegates>

#include <iostream>
#include <stdio.h>

#include <Wt/Dbo/Query>
#include <Wt/WMessageBox>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

Views::ViewProjects::ViewProjects()
    : Ms::Widgets::MContainerWidget()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());
    _propertiesPanel = Session::SessionManager::instance().getSessionPropertiesPanel(Wt::WApplication::instance()->sessionId());

    _prepareView();

    _mnuMain->select(_mnuMainProjectsItem);

    updateProjectsView();
    _stkMain->setCurrentWidget(_cntProjects);
}

void Views::ViewProjects::updateView()
{
    if(_mnuMain->currentItem() == _mnuMainProjectsItem)
        updateProjectsView();
    else if(_mnuMain->currentItem() == _mnuMainSequencesItem)
        updateSequencesView();
    else if(_mnuMain->currentItem() == _mnuMainShotsItem)
        updateShotsView();
    else if(_mnuMain->currentItem() == _mnuMainAssetsItem)
        updateAssetsView();
    else if(_mnuMain->currentItem() == _mnuMainTasksItem)
        updateTasksView();
}

void Views::ViewProjects::updateProjectsView()
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::Project>> query;

        int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        _qtvProjects->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvProjects->clearColumns();

        //add columns
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Widgets::Delegates::WorkStatusQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Manager_Name", "Manager", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::User>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Users::User>() :
         Database::DatabaseManager::instance().session()->find<Users::User>().where("Active = ?").bind(true),
         "Name", editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvProjects->addBaseColumns(flags, editRank);

        _qtvProjects->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating table view" << std::endl;
    }
}

void Views::ViewProjects::updateSequencesView()
{
    _viewSequences->updateView(_qtvProjects->selectedItems());
}

void Views::ViewProjects::updateShotsView()
{
    _viewShots->updateView(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0 ?
                               _viewSequences->qtvSequences()->selectedItems() : _viewSequences->qtvSequences()->items());
}

void Views::ViewProjects::updateAssetsView()
{
    _viewAssets->updateView(_qtvProjects->selectedItems());
}

void Views::ViewProjects::updateTasksView()
{
    std::vector<Wt::Dbo::ptr<Projects::Project>> prjVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> seqVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> shotVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> assetVec;

    if(_qtvProjects->table()->selectedIndexes().size() > 0)
    {
        prjVec = _qtvProjects->selectedItems();

        seqVec = _viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0 ?
                    _viewSequences->qtvSequences()->selectedItems() : _viewSequences->qtvSequences()->items();

        shotVec = _viewShots->qtvShots()->table()->selectedIndexes().size() > 0 ?
                    _viewShots->qtvShots()->selectedItems() : _viewShots->qtvShots()->items();

        assetVec = _viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0 ?
                    _viewAssets->qtvAssets()->selectedItems() : _viewAssets->qtvAssets()->items();
    }

    _viewTasks->updateView(prjVec, seqVec, shotVec, assetVec);
}

void Views::ViewProjects::updatePropertiesView()
{
    if(_stkProperties->currentWidget() == _viewPropertiesData)
    {
        _updatePropertiesDataView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesTags)
    {
        _updatePropertiesTagsView();
        _updatePropertiesAssignedTagsView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesNotes)
    {
        _updatePropertiesNotesView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesSequences)
    {
        _updatePropertiesSequencesView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesShots)
    {
        _updatePropertiesShotsView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesAssets)
    {
        _updatePropertiesAssetsView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesTasks)
    {
        _updatePropertiesTasksView();
    }
    else if(_stkProperties->currentWidget() == _viewPropertiesTaskActivities)
    {
        _updatePropertiesTaskActivitiesView();
    }
}

bool Views::ViewProjects::isProjectsViewShown()
{
    return _mnuMain->currentItem() == _mnuMainProjectsItem;
}

bool Views::ViewProjects::isSequencesViewShown()
{
    return _mnuMain->currentItem() == _mnuMainSequencesItem;
}

bool Views::ViewProjects::isShotsViewShown()
{
    return _mnuMain->currentItem() == _mnuMainShotsItem;
}

bool Views::ViewProjects::isAssetsViewShown()
{
    return _mnuMain->currentItem() == _mnuMainAssetsItem;
}

bool Views::ViewProjects::isTasksViewShown()
{
    return _mnuMain->currentItem() == _mnuMainTasksItem;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::Project> *Views::ViewProjects::projectsQueryTableView() const
{
    return _qtvProjects;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *Views::ViewProjects::sequencesQueryTableView() const
{
    return _viewSequences->qtvSequences();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *Views::ViewProjects::shotsQueryTableView() const
{
    return _viewShots->qtvShots();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *Views::ViewProjects::assetsQueryTableView() const
{
    return _viewAssets->qtvAssets();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewProjects::tasksQueryTableView() const
{
    return _viewTasks->qtvTasks();
}

Wt::Signal<> &Views::ViewProjects::onTabProjectsSelected()
{
    return _onTabProjectsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabSequencesSelected()
{
    return _onTabSequencesSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabShotsSelected()
{
    return _onTabShotsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabAssetsSelected()
{
    return _onTabAssetsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabTasksSelected()
{
    return _onTabTasksSelected;
}

template<typename T>
void Views::ViewProjects::_addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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
void Views::ViewProjects::_addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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
void Views::ViewProjects::_assignTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
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
void Views::ViewProjects::_unAssignTagFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
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

//Main
void Views::ViewProjects::_mnuMainProjectsItemTriggered()
{
    if((_stkProperties->currentWidget() == _viewPropertiesShots) ||
            (_stkProperties->currentWidget() == _viewPropertiesTaskActivities))
        _mnuNavBarPropertiesDataItem->select();

    _mnuNavBarPropertiesSequencesItem->show();
    _mnuNavBarPropertiesShotsItem->hide();
    _mnuNavBarPropertiesAssetsItem->show();
    _mnuNavBarPropertiesTasksItem->show();
    _mnuNavBarPropertiesTaskActivitiesItem->hide();

    _viewPropertiesTags->setCreateOptionHidden(false);

    _stkMain->setCurrentWidget(_cntProjects);
    updateProjectsView();

    _onTabProjectsSelected();
}

void Views::ViewProjects::_mnuMainSequencesItemTriggered()
{
    if((_stkProperties->currentWidget() == _viewPropertiesSequences) ||
            (_stkProperties->currentWidget() == _viewPropertiesAssets) ||
            (_stkProperties->currentWidget() == _viewPropertiesTaskActivities))
    {
        _mnuNavBarPropertiesDataItem->select();
    }

    _mnuNavBarPropertiesSequencesItem->hide();
    _mnuNavBarPropertiesShotsItem->show();
    _mnuNavBarPropertiesAssetsItem->hide();
    _mnuNavBarPropertiesTasksItem->show();
    _mnuNavBarPropertiesTaskActivitiesItem->hide();

    _viewPropertiesTags->setCreateOptionHidden(true);

    _stkMain->setCurrentWidget(_cntSequences);
    updateSequencesView();

    _onTabSequencesSelected();
}

void Views::ViewProjects::_mnuMainShotsItemTriggered()
{
    if((_stkProperties->currentWidget() == _viewPropertiesSequences) ||
            (_stkProperties->currentWidget() == _viewPropertiesShots) ||
            (_stkProperties->currentWidget() == _viewPropertiesAssets) ||
            (_stkProperties->currentWidget() == _viewPropertiesTaskActivities))
    {
        _mnuNavBarPropertiesDataItem->select();
    }

    _mnuNavBarPropertiesSequencesItem->hide();
    _mnuNavBarPropertiesShotsItem->hide();
    _mnuNavBarPropertiesAssetsItem->hide();
    _mnuNavBarPropertiesTasksItem->show();
    _mnuNavBarPropertiesTaskActivitiesItem->hide();

    _viewPropertiesTags->setCreateOptionHidden(true);

    _stkMain->setCurrentWidget(_cntShots);
    updateShotsView();

    _onTabShotsSelected();
}

void Views::ViewProjects::_mnuMainAssetsItemTriggered()
{
    if((_stkProperties->currentWidget() == _viewPropertiesSequences) ||
            (_stkProperties->currentWidget() == _viewPropertiesShots) ||
            (_stkProperties->currentWidget() == _viewPropertiesAssets) ||
            (_stkProperties->currentWidget() == _viewPropertiesTaskActivities))
    {
        _mnuNavBarPropertiesDataItem->select();
    }

    _mnuNavBarPropertiesSequencesItem->hide();
    _mnuNavBarPropertiesShotsItem->hide();
    _mnuNavBarPropertiesAssetsItem->hide();
    _mnuNavBarPropertiesTasksItem->show();
    _mnuNavBarPropertiesTaskActivitiesItem->hide();

    _viewPropertiesTags->setCreateOptionHidden(true);

    _stkMain->setCurrentWidget(_cntAssets);
    updateAssetsView();

    _onTabAssetsSelected();
}

void Views::ViewProjects::_mnuMainTasksItemTriggered()
{
    if((_stkProperties->currentWidget() == _viewPropertiesSequences) ||
            (_stkProperties->currentWidget() == _viewPropertiesShots) ||
            (_stkProperties->currentWidget() == _viewPropertiesAssets) ||
            (_stkProperties->currentWidget() == _viewPropertiesTasks))
    {
        _mnuNavBarPropertiesDataItem->select();
    }

    _mnuNavBarPropertiesSequencesItem->hide();
    _mnuNavBarPropertiesShotsItem->hide();
    _mnuNavBarPropertiesAssetsItem->hide();
    _mnuNavBarPropertiesTasksItem->hide();
    _mnuNavBarPropertiesTaskActivitiesItem->show();

    _viewPropertiesTags->setCreateOptionHidden(true);

    _stkMain->setCurrentWidget(_cntTasks);
    updateTasksView();

    _onTabTasksSelected();
}

//Projects
void Views::ViewProjects::_btnProjectsCreateClicked()
{
    Views::DlgCreateAndEditProject *dlg = new Views::DlgCreateAndEditProject();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            if(!Database::DatabaseManager::instance().dboExists<Projects::Project>(dlg->projectName()))
            {

                Projects::Project *project = new Projects::Project(dlg->projectName());
                project->setStatus(dlg->status());
                project->setManager(dlg->manager());
                project->setStartDate(dlg->startDate());
                project->setEndDate(dlg->endDate());
                project->setDurationInFrames(dlg->duration());
                project->setFps(dlg->fps());
                project->setFrameWidth(dlg->frameWidth());
                project->setFrameHeight(dlg->frameHeight());
                project->setPriority(dlg->priority());
                project->setDescription(dlg->description());
                project->setActive(dlg->isActive());

                if(Database::DatabaseManager::instance().createDbo<Projects::Project>(project))
                {
                    Projects::ProjectsIO::createProjectDirectoryStructure(dlg->projectName());
                    updateProjectsView();

                    _logger->log(std::string("Created project ") + dlg->projectName(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete project;

                    _logger->log(std::string("Error creating project ") + dlg->projectName(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_btnProjectsRemoveClicked()
{

}

void Views::ViewProjects::_btnProjectsEditClicked()
{
    if(_qtvProjects->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditProject *dlg = new Views::DlgCreateAndEditProject(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto prjPtr : _qtvProjects->selectedItems())
            {
                if(dlg->editedStartDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setEndDate(dlg->endDate());

                if(dlg->editedDuration())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setDurationInFrames(dlg->duration());

                if(dlg->editedFps())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setFps(dlg->fps());

                if(dlg->editedFrameWidth())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setFrameWidth(dlg->frameWidth());

                if(dlg->editedFrameHeight())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setFrameHeight(dlg->frameHeight());

                if(dlg->editedPriority())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setStatus(dlg->status());

                if(dlg->editedManager())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setManager(dlg->manager());

                if(dlg->editedDescription())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(prjPtr)->setActive(dlg->isActive());
            }

            _qtvProjects->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_btnProjectsFilesClicked()
{
    if(_qtvProjects->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    std::string prjName = _qtvProjects->selectedItems().at(0)->name();

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getRelativeProjectDir(prjName) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_btnProjectsImportThumbnailsClicked()
{
    if(_qtvProjects->model()->rowCount() == 0)
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

                //match thumbnail by project name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                Wt::Dbo::ptr<Projects::Project> prjPtr;

                if(AppSettings::instance().isLoadInactiveData())
                    prjPtr = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("Project_Name = ?").bind(rawFileName);
                else
                    prjPtr = Database::DatabaseManager::instance().session()->find<Projects::Project>().where("Project_Name = ? AND Active = ?").bind(rawFileName).bind(true);

                if(prjPtr)//found a project that has the same name as the thumbnail ?
                {
                    std::string prjDir = Projects::ProjectsIO::getAbsoluteProjectDir(prjPtr->name());
                    std::string localFile = prjDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                    if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                        Ms::IO::removeFile(localFile);//delete it

                    if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                    {
                        prjPtr.modify()->setThumbnail(Projects::ProjectsIO::getRelativeProjectDir(prjPtr->name()) + Ms::IO::dirSeparator() +
                                                      "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table projects") + e.what(),
                             Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
        }

        _qtvProjects->updateView();

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_projectImported(Wt::Dbo::ptr<Projects::Project> project)
{
    Projects::ProjectsIO::createProjectDirectoryStructure(project->name());
}

void Views::ViewProjects::_createProjectsTableView()
{
    _qtvProjects = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::Project>(&Database::DatabaseManager::instance());
    _qtvProjects->setRowHeight(160);
    _qtvProjects->setDefaultFilterColumnIndex(1);
    _qtvProjects->setIgnoreNumFilterColumns(1);
    _qtvProjects->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    _qtvProjects->itemImported().connect(this, &Views::ViewProjects::_projectImported);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Add.png", "Create A New Project");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsCreateClicked);

        _qtvProjects->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvProjects->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Remove.png", "Remove Selected Project");
        //btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsRemoveClicked);
    }

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Edit.png", "Edit Selected Projects");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsEditClicked);
    }

    //requires "edit" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvProjects->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewProjects::_btnProjectsFilesClicked);
    }

    updateProjectsView();
}

//Sequences
void Views::ViewProjects::_createSequenceRequested()
{
    if(_qtvProjects->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one project.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditSequence *dlg = new Views::DlgCreateAndEditSequence();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::Project> prjPtr = _qtvProjects->selectedItems().at(0);

            Projects::ProjectSequenceId id;
            id.name = (dlg->sequenceName());
            id.project = prjPtr;

            if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectSequence>(id))
            {
                Projects::ProjectSequence *sequence = new Projects::ProjectSequence(dlg->sequenceName());
                sequence->setProject(prjPtr);
                sequence->setStatus(dlg->status());
                sequence->setStartDate(dlg->startDate());
                sequence->setEndDate(dlg->endDate());
                sequence->setDurationInFrames(dlg->duration());
                sequence->setFps(dlg->fps());
                sequence->setFrameWidth(dlg->frameWidth());
                sequence->setFrameHeight(dlg->frameHeight());
                sequence->setPriority(dlg->priority());
                sequence->setDescription(dlg->description());
                sequence->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectSequence>(sequence);
                if(seqPtr.get())
                {
                    Projects::ProjectsIO::createSequenceDirectoryStructure(prjPtr->name(), seqPtr->name());
                    _updatePropertiesSequencesView();

                    _logger->log(std::string("Created sequence for project ") + prjPtr->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete sequence;

                    _logger->log(std::string("Error creating sequence for project ") + prjPtr->name(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_sequenceImported(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    Projects::ProjectsIO::createSequenceDirectoryStructure(sequence->projectName(), sequence->name());
}

void Views::ViewProjects::_createSequencesTableView()
{
    _viewSequences = new Views::ViewSequences();
    _viewSequences->setCreateOptionHidden(true);
    _viewSequences->qtvSequences()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    _viewSequences->qtvSequences()->itemImported().connect(this, &Views::ViewProjects::_sequenceImported);
}

//Shots
void Views::ViewProjects::_createShotRequested()
{
    if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one sequence.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditShot *dlg = new Views::DlgCreateAndEditShot();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = _viewSequences->qtvSequences()->selectedItems().at(0);

            Projects::ProjectShotId id;
            id.name = (dlg->shotName());
            id.sequence = seqPtr;

            if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectShot>(id))
            {
                Projects::ProjectShot *shot = new Projects::ProjectShot(dlg->shotName());
                shot->setSequence(seqPtr);
                shot->setStatus(dlg->status());
                shot->setStartDate(dlg->startDate());
                shot->setEndDate(dlg->endDate());
                shot->setDurationInFrames(dlg->duration());
                shot->setFps(dlg->fps());
                shot->setFrameWidth(dlg->frameWidth());
                shot->setFrameHeight(dlg->frameHeight());
                shot->setPriority(dlg->priority());
                shot->setDescription(dlg->description());
                shot->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectShot>(shot);
                if(shotPtr.get())
                {
                    Projects::ProjectsIO::createShotDirectoryStructure(seqPtr->projectName(), seqPtr->name(), shotPtr->name());
                    _updatePropertiesShotsView();

                    _logger->log(std::string("Created shot for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete shot;

                    _logger->log(std::string("error creating shot for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_shotImported(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    Projects::ProjectsIO::createShotDirectoryStructure(shot->projectName(), shot->sequenceName(), shot->name());
}

void Views::ViewProjects::_createShotsTableView()
{
    _viewShots = new Views::ViewShots();
    _viewShots->setCreateOptionHidden(true);
    _viewShots->qtvShots()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    _viewShots->qtvShots()->itemImported().connect(this, &Views::ViewProjects::_shotImported);
}

//Assets
void Views::ViewProjects::_createAssetRequested()
{
    if(_qtvProjects->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one aroject.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditAsset *dlg = new Views::DlgCreateAndEditAsset();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::Project> prjPtr = _qtvProjects->selectedItems().at(0);

            Projects::ProjectAssetId id;
            id.name = (dlg->assetName());
            id.project = prjPtr;

            if(!Database::DatabaseManager::instance().dboExists<Projects::ProjectAsset>(id))
            {
                Projects::ProjectAsset *asset = new Projects::ProjectAsset(dlg->assetName());
                asset->setProject(prjPtr);
                asset->setStatus(dlg->status());
                asset->setType(dlg->assetType());
                asset->setStartDate(dlg->startDate());
                asset->setEndDate(dlg->endDate());
                asset->setPriority(dlg->priority());
                asset->setDescription(dlg->description());
                asset->setActive(dlg->isActive());

                Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectAsset>(asset);
                if(assetPtr.get())
                {
                    Projects::ProjectsIO::createAssetDirectoryStructure(prjPtr->name(), assetPtr->name());
                    _updatePropertiesAssetsView();

                    _logger->log(std::string("Created asset ") + dlg->assetName(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete asset;

                    _logger->log(std::string("error creating asset ") + dlg->assetName(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                _logger->log(std::string("Object alredy exist"), Ms::Log::LogMessageType::Warning);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_assetImported(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    Projects::ProjectsIO::createAssetDirectoryStructure(asset->projectName(), asset->name());
}

void Views::ViewProjects::_createAssetsTableView()
{
    _viewAssets = new Views::ViewAssets();
    _viewAssets->setCreateOptionHidden(true);
    _viewAssets->qtvAssets()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    _viewAssets->qtvAssets()->itemImported().connect(this, &Views::ViewProjects::_assetImported);
}

//Tasks
void Views::ViewProjects::_createTasksRequested()
{
    if((_stkMain->currentWidget() == _cntProjects && _qtvProjects->table()->selectedIndexes().size() > 0) ||
            (_stkMain->currentWidget() == _cntSequences && _viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0) ||
            (_stkMain->currentWidget() == _cntShots && _viewShots->qtvShots()->table()->selectedIndexes().size() > 0) ||
            (_stkMain->currentWidget() == _cntAssets && _viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0))
    {
        Views::DlgCreateAndEditTask *dlg = new Views::DlgCreateAndEditTask();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                if(_stkMain->currentWidget() == _cntProjects)
                {
                    for(auto &prjPtr : _qtvProjects->selectedItems())
                    {
                        Projects::ProjectTask *task = new Projects::ProjectTask();
                        task->setUser(dlg->user());
                        task->setStatus(dlg->status());
                        task->setType(dlg->type());
                        task->setStartDate(dlg->startDate());
                        task->setEndDate(dlg->endDate());
                        task->setPriority(dlg->priority());
                        task->setDescription(dlg->description());
                        task->setActive(dlg->isActive());
                        task->setProject(prjPtr);

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createProjectTaskDirectoryStructure(prjPtr->name(), taskPtr.id());
                            _updatePropertiesTasksView();

                            _logger->log(std::string("Created task for project ") + prjPtr->name(), Ms::Log::LogMessageType::Info);
                        }
                        else
                        {
                            delete task;

                            _logger->log(std::string("Error creating task for project") + prjPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }
                else if(_stkMain->currentWidget() == _cntSequences)
                {
                    for(auto &seqPtr : _viewSequences->qtvSequences()->selectedItems())
                    {
                        Projects::ProjectTask *task = new Projects::ProjectTask();
                        task->setUser(dlg->user());
                        task->setStatus(dlg->status());
                        task->setType(dlg->type());
                        task->setStartDate(dlg->startDate());
                        task->setEndDate(dlg->endDate());
                        task->setPriority(dlg->priority());
                        task->setDescription(dlg->description());
                        task->setActive(dlg->isActive());
                        task->setSequence(seqPtr);

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createSequenceTaskDirectoryStructure(seqPtr->projectName(), seqPtr->name(), taskPtr.id());
                            _updatePropertiesTasksView();

                            _logger->log(std::string("Created task for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Info);
                        }
                        else
                        {
                            delete task;

                            _logger->log(std::string("Error creating task for sequence") + seqPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }
                else if(_stkMain->currentWidget() == _cntShots)
                {
                    for(auto &shotPtr : _viewShots->qtvShots()->selectedItems())
                    {
                        Projects::ProjectTask *task = new Projects::ProjectTask();
                        task->setUser(dlg->user());
                        task->setStatus(dlg->status());
                        task->setType(dlg->type());
                        task->setStartDate(dlg->startDate());
                        task->setEndDate(dlg->endDate());
                        task->setPriority(dlg->priority());
                        task->setDescription(dlg->description());
                        task->setActive(dlg->isActive());
                        task->setShot(shotPtr);

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createShotTaskDirectoryStructure(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name(), taskPtr.id());
                            _updatePropertiesTasksView();

                            _logger->log(std::string("Created task for shot ") + shotPtr->name(), Ms::Log::LogMessageType::Info);
                        }
                        else
                        {
                            delete task;

                            _logger->log(std::string("Error creating task for shot") + shotPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }
                else if(_stkMain->currentWidget() == _cntAssets)
                {
                    //add tasks for selected assets
                    for(auto &assetPtr : _viewAssets->qtvAssets()->selectedItems())
                    {
                        Projects::ProjectTask *task = new Projects::ProjectTask();
                        task->setUser(dlg->user());
                        task->setAsset(assetPtr);
                        task->setStatus(dlg->status());
                        task->setType(dlg->type());
                        task->setStartDate(dlg->startDate());
                        task->setEndDate(dlg->endDate());
                        task->setPriority(dlg->priority());
                        task->setDescription(dlg->description());
                        task->setActive(dlg->isActive());
                        task->setAsset(assetPtr);

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createAssetTaskDirectoryStructure(assetPtr->projectName(), assetPtr->projectName(), taskPtr.id());
                            _updatePropertiesTasksView();

                            _logger->log(std::string("Created task for asset ") + assetPtr->name(), Ms::Log::LogMessageType::Info, Log::LogMessageContext::ServerAndClient);
                        }
                        else
                        {
                            delete task;

                            _logger->log(std::string("error creating task for asset ") + assetPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }

                    updateTasksView();
                }
            }

            delete dlg;
        }));

        dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
    }
    else
    {
        _logger->log(std::string("Please select at least one item"), Ms::Log::LogMessageType::Warning);
    }
}

void Views::ViewProjects::_createTasksForTemplateRequested()
{
    if((_stkMain->currentWidget() == _cntProjects && _qtvProjects->table()->selectedIndexes().size() > 0) ||
            (_stkMain->currentWidget() == _cntSequences && _viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0) ||
            (_stkMain->currentWidget() == _cntShots && _viewShots->qtvShots()->table()->selectedIndexes().size() > 0) ||
            (_stkMain->currentWidget() == _cntAssets && _viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0))
    {
        Views::DlgSelectTaskTemplate *dlg = new Views::DlgSelectTaskTemplate();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                Wt::Dbo::ptr<Projects::ProjectTaskTemplate> templatePtr = dlg->taskTemplate();

                if(_stkMain->currentWidget() == _cntProjects ||
                        _stkMain->currentWidget() == _cntSequences ||
                        _stkMain->currentWidget() == _cntShots ||
                        _stkMain->currentWidget() == _cntAssets)
                {
                    if(_stkMain->currentWidget() == _cntProjects)
                    {
                        for(auto &prjPtr : _qtvProjects->selectedItems())
                        {
                            if(templatePtr->createTasksForProject(prjPtr))
                                _logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                            else
                                _logger->log(std::string("error creating tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                    else if(_stkMain->currentWidget() == _cntSequences)
                    {
                        for(auto &seqPtr : _viewSequences->qtvSequences()->selectedItems())
                        {
                            if(templatePtr->createTasksForProjectSequence(seqPtr))
                                _logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                            else
                                _logger->log(std::string("error creating tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                    else if(_stkMain->currentWidget() == _cntShots)
                    {
                        for(auto &shotPtr : _viewShots->qtvShots()->selectedItems())
                        {
                            if(templatePtr->createTasksForProjectShot(shotPtr))
                                _logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                            else
                                _logger->log(std::string("error creating tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                    else if(_stkMain->currentWidget() == _cntAssets)
                    {
                        //add tasks for selected assets
                        for(auto &assetPtr : _viewAssets->qtvAssets()->selectedItems())
                        {
                            if(templatePtr->createTasksForProjectAsset(assetPtr))
                                _logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                            else
                                _logger->log(std::string("error creating tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }

                    _updatePropertiesTasksView();
                }
            }

            delete dlg;
        }));

        dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
    }
    else
    {
        _logger->log(std::string("Please select at least one item"), Ms::Log::LogMessageType::Warning);
    }
}

void Views::ViewProjects::_taskImported(Wt::Dbo::ptr<Projects::ProjectTask> task)
{
    if(task->project().get())
        Projects::ProjectsIO::createProjectTaskDirectoryStructure(task->project()->name(), task.id());

    if(task->sequence().get())
        Projects::ProjectsIO::createSequenceTaskDirectoryStructure(task->sequence()->projectName(), task->sequence()->name(), task.id());

    if(task->shot().get())
        Projects::ProjectsIO::createShotTaskDirectoryStructure(task->shot()->sequence()->projectName(), task->shot()->sequence()->name(), task->shot()->name(), task.id());

    if(task->asset().get())
        Projects::ProjectsIO::createAssetTaskDirectoryStructure(task->asset()->projectName(), task->asset()->name(), task.id());
}

void Views::ViewProjects::_createTasksTableView()
{
    _viewTasks = new Views::ViewTasks();
    _viewTasks->setCreateOptionHidden(true);
    _viewTasks->qtvTasks()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    _viewTasks->qtvTasks()->itemImported().connect(this, &Views::ViewProjects::_taskImported);
}

//Properties
void Views::ViewProjects::_mnuNavBarPropertiesDataItemTriggered()
{
    _updatePropertiesDataView();
    _stkProperties->setCurrentWidget(_viewPropertiesData);
}

void Views::ViewProjects::_mnuNavBarPropertiesTagsItemTriggered()
{
    _updatePropertiesTagsView();
    _updatePropertiesAssignedTagsView();
    _stkProperties->setCurrentWidget(_viewPropertiesTags);
}

void Views::ViewProjects::_mnuNavBarPropertiesNotesItemTriggered()
{
    _updatePropertiesNotesView();
    _stkProperties->setCurrentWidget(_viewPropertiesNotes);
}

void Views::ViewProjects::_mnuNavBarPropertiesSequencesItemTriggered()
{
    _viewPropertiesSequences->updateView(_qtvProjects->selectedItems());
    _stkProperties->setCurrentWidget(_viewPropertiesSequences);
}

void Views::ViewProjects::_mnuNavBarPropertiesShotsItemTriggered()
{
    _viewPropertiesShots->updateView(_viewSequences->qtvSequences()->selectedItems());
    _stkProperties->setCurrentWidget(_viewPropertiesShots);
}

void Views::ViewProjects::_mnuNavBarPropertiesAssetsItemTriggered()
{
    _viewPropertiesAssets->updateView(_qtvProjects->selectedItems());
    _stkProperties->setCurrentWidget(_viewPropertiesAssets);
}

void Views::ViewProjects::_mnuNavBarPropertiesTasksItemTriggered()
{
    _updatePropertiesTasksView();
    _stkProperties->setCurrentWidget(_viewPropertiesTasks);
}

void Views::ViewProjects::_mnuNavBarPropertiesTaskActivitiesItemTriggered()
{
    _updatePropertiesTaskActivitiesView();
    _stkProperties->setCurrentWidget(_viewPropertiesTaskActivities);
}

void Views::ViewProjects::_addDataRequested()
{
    if(_stkMain->currentWidget() == _cntProjects)
        _addDataToDbo<Projects::Project>(_qtvProjects->selectedItems());
    else if(_stkMain->currentWidget() == _cntSequences)
        _addDataToDbo<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems());
    else if(_stkMain->currentWidget() == _cntShots)
        _addDataToDbo<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems());
    else if(_stkMain->currentWidget() == _cntAssets)
        _addDataToDbo<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems());
    else if(_stkMain->currentWidget() == _cntTasks)
        _addDataToDbo<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems());
}

void Views::ViewProjects::_removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec)
{

}

void Views::ViewProjects::_createProjectTagRequested()
{
    bool createTag = false;
    bool customProjectTag = false;

    if(_qtvProjects->table()->selectedIndexes().size() > 0)
    {
        if(_qtvProjects->table()->selectedIndexes().size() != 1)
            _logger->log("Please select only one project.", Ms::Log::LogMessageType::Warning);
        else
        {
            customProjectTag = true;
            createTag = true;
        }
    }
    else
        createTag = true;

    if(!createTag)
        return;

    Views::Dialogs::DlgCreateTag *dlg = new Views::Dialogs::DlgCreateTag();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Database::Tag *tag = new Database::Tag(dlg->tagName(), dlg->tagContent());
            if(customProjectTag)
                tag->setType("Local");
            else
                tag->setType("Project");

            tag->setActive(dlg->isActive());

            Wt::Dbo::ptr<Database::Tag> tagPtr = Database::DatabaseManager::instance().createDbo<Database::Tag>(tag);

            if(tagPtr.get())
            {
                if(customProjectTag)
                    Database::DatabaseManager::instance().modifyDbo<Projects::Project>(_qtvProjects->selectedItems().at(0))->addTag(tagPtr);

                _updatePropertiesTagsView();
            }
            else
                delete tag;
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(tagVec.size() == 0)
        return;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Projects::Project>(_qtvProjects->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
            _assignTagToDbo<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems(), tagVec);
    }
}

void Views::ViewProjects::_unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(tagVec.size() == 0)
        return;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Projects::Project>(_qtvProjects->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems(), tagVec);
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
            _unAssignTagFromDbo<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems(), tagVec);
    }
}

void Views::ViewProjects::_filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    std::string strFilterQuery = "";

    if(tagVec.size() == 0)
        return;

    std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Database::Tag>(tagVec);

    if(_stkMain->currentWidget() == _cntProjects)
    {
        strFilterQuery = "Project_Name IN (SELECT pt.project_Project_Name FROM rel_project_assigned_tags pt WHERE tag_id IN (" + idValues.at(0) + "))";

        _qtvProjects->setCustomFilterString(strFilterQuery);
        _qtvProjects->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        strFilterQuery = "Sequence_Name IN (SELECT st.project_sequence_Sequence_Name FROM rel_project_sequence_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")) AND "
                "Sequence_Project_Project_Name IN (SELECT st.project_sequence_Sequence_Project_Project_Name FROM rel_project_sequence_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + "))";

        _viewSequences->qtvSequences()->setCustomFilterString(strFilterQuery);
        _viewSequences->qtvSequences()->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        strFilterQuery = "Shot_Name IN (SELECT st.project_shot_Shot_Name FROM rel_project_shot_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")) AND "
                "Shot_Sequence_Sequence_Name IN (SELECT st.project_shot_Shot_Sequence_Sequence_Name FROM rel_project_shot_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")) AND "
                "Shot_Sequence_Sequence_Project_Project_Name IN (SELECT st.project_shot_Shot_Sequence_Sequence_Project_Project_Name FROM rel_project_shot_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + "))";

        _viewShots->qtvShots()->setCustomFilterString(strFilterQuery);
        _viewShots->qtvShots()->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        strFilterQuery = "Asset_Name IN (SELECT at.project_asset_Asset_Name FROM rel_project_asset_assigned_tags at WHERE tag_id IN (" + idValues.at(0) + ")) AND "
                "Asset_Project_Project_Name IN (SELECT at.project_asset_Asset_Project_Project_Name FROM rel_project_asset_assigned_tags at WHERE tag_id IN (" + idValues.at(0) + "))";

        _viewAssets->qtvAssets()->setCustomFilterString(strFilterQuery);
        _viewAssets->qtvAssets()->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        strFilterQuery = "id IN (SELECT tt.project_task_id FROM rel_project_task_assigned_tags tt WHERE tag_id IN (" + idValues.at(0) + "))";

        _viewTasks->qtvTasks()->setCustomFilterString(strFilterQuery);
        _viewTasks->qtvTasks()->setCustomFilterActive(true);
    }
}

void Views::ViewProjects::_clearTagsFilterRequested()
{
    if(_stkMain->currentWidget() == _cntProjects)
    {
        _qtvProjects->setCustomFilterString("");
        _qtvProjects->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        _viewSequences->qtvSequences()->setCustomFilterString("");
        _viewSequences->qtvSequences()->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        _viewShots->qtvShots()->setCustomFilterString("");
        _viewShots->qtvShots()->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        _viewAssets->qtvAssets()->setCustomFilterString("");
        _viewAssets->qtvAssets()->setCustomFilterActive(true);
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        _viewTasks->qtvTasks()->setCustomFilterString("");
        _viewTasks->qtvTasks()->setCustomFilterActive(true);
    }
}

void Views::ViewProjects::_addNoteRequested()
{
    if(_stkMain->currentWidget() == _cntProjects)
        _addNoteToDbo<Projects::Project>(_qtvProjects->selectedItems());
    else if(_stkMain->currentWidget() == _cntSequences)
         _addNoteToDbo<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems());
    else if(_stkMain->currentWidget() == _cntShots)
        _addNoteToDbo<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems());
    else if(_stkMain->currentWidget() == _cntAssets)
        _addNoteToDbo<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems());
    else if(_stkMain->currentWidget() == _cntTasks)
        _addNoteToDbo<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems());
}

void Views::ViewProjects::_removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec)
{

}

void Views::ViewProjects::_createTaskActivityRequested()
{
    if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one task.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskActivity *dlg = new Views::DlgCreateAndEditTaskActivity();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = _viewTasks->qtvTasks()->selectedItems().at(0);

            Projects::ProjectTaskActivity *activity = new Projects::ProjectTaskActivity();
            activity->setTask(taskPtr);
            activity->setType(dlg->type());
            activity->setStatus(dlg->status());
            activity->setDescription(dlg->description());
            activity->setActive(dlg->isActive());

            Wt::Dbo::ptr<Projects::ProjectTaskActivity> activityPtr = Database::DatabaseManager::instance().createDbo<Projects::ProjectTaskActivity>(activity);
            if(activityPtr.get())
            {
                _updatePropertiesTaskActivitiesView();

                _logger->log(std::string("Created task activity ") + std::to_string(activityPtr.id()), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete activity;

                _logger->log(std::string("error creating task activity"), Ms::Log::LogMessageType::Error);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_createTaskActivitiesForTemplateRequested()
{
    if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one task.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgSelectActivityTemplate *dlg = new Views::DlgSelectActivityTemplate();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::ptr<Projects::ProjectActivityTemplate> templatePtr = dlg->activityTemplate();

            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = _viewTasks->qtvTasks()->selectedItems().at(0);

            if(templatePtr->createActivitiesForProjectTask(taskPtr))
            {
                _updatePropertiesTaskActivitiesView();

                _logger->log(std::string("Created task activities for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
            }
            else
            {
                _logger->log(std::string("error creating task activities for template ") + templatePtr->name(), Ms::Log::LogMessageType::Error);
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::_removeTaskActivitiesRequested(const std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> &activityVec)
{

}

void Views::ViewProjects::_createPropertiesView()
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
    _mnuNavBarPropertiesDataItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesDataItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesDataItem);

    _mnuNavBarPropertiesTagsItem = new Wt::WMenuItem("Tags");
    _mnuNavBarPropertiesTagsItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesTagsItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesTagsItem);

    _mnuNavBarPropertiesNotesItem = new Wt::WMenuItem("Notes");
    _mnuNavBarPropertiesNotesItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesNotesItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesNotesItem);

    _mnuNavBarPropertiesSequencesItem = new Wt::WMenuItem("Sequences");
    _mnuNavBarPropertiesSequencesItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesSequencesItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesSequencesItem);

    _mnuNavBarPropertiesShotsItem = new Wt::WMenuItem("Shots");
    _mnuNavBarPropertiesShotsItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesShotsItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesShotsItem);

    _mnuNavBarPropertiesAssetsItem = new Wt::WMenuItem("Assets");
    _mnuNavBarPropertiesAssetsItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesAssetsItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesAssetsItem);

    _mnuNavBarPropertiesTasksItem = new Wt::WMenuItem("Tasks");
    _mnuNavBarPropertiesTasksItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesTasksItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesTasksItem);

    _mnuNavBarPropertiesTaskActivitiesItem = new Wt::WMenuItem("Activities");
    _mnuNavBarPropertiesTaskActivitiesItem->triggered().connect(this, &Views::ViewProjects::_mnuNavBarPropertiesTaskActivitiesItemTriggered);
    _mnuNavBarProperties->addItem(_mnuNavBarPropertiesTaskActivitiesItem);

    _stkProperties = new Wt::WStackedWidget();

    _layCntPropertiesMain->addWidget(_stkProperties, 1);

    _viewPropertiesData = new Views::ViewDboData();
    _viewPropertiesData->addDataRequested().connect(this, &Views::ViewProjects::_addDataRequested);
    _viewPropertiesData->removeDataRequested().connect(this, &Views::ViewProjects::_removeDataRequested);

    _stkProperties->addWidget(_viewPropertiesData);

    _viewPropertiesTags = new Views::ViewTags();
    _viewPropertiesTags->createTagRequested().connect(this, &Views::ViewProjects::_createProjectTagRequested);
    _viewPropertiesTags->assignTagsRequested().connect(this, &Views::ViewProjects::_assignTagsRequested);
    _viewPropertiesTags->unassignTagsRequested().connect(this, &Views::ViewProjects::_unassignTagsRequested);
    _viewPropertiesTags->filterByTagsRequested().connect(this, &Views::ViewProjects::_filterByTagsRequested);
    _viewPropertiesTags->clearTagsFilterRequested().connect(this, &Views::ViewProjects::_clearTagsFilterRequested);

    _stkProperties->addWidget(_viewPropertiesTags);

    _viewPropertiesNotes = new Views::ViewNotes();
    _viewPropertiesNotes->addNoteRequested().connect(this, &Views::ViewProjects::_addNoteRequested);
    _viewPropertiesNotes->removeNotesRequested().connect(this, &Views::ViewProjects::_removeNotesRequested);

    _stkProperties->addWidget(_viewPropertiesNotes);

    _viewPropertiesSequences = new Views::ViewSequences();
    _viewPropertiesSequences->createSequenceRequested().connect(this, &Views::ViewProjects::_createSequenceRequested);
    _qtvProjects->table()->selectionChanged().connect(this, &Views::ViewProjects::updateSequencesView);

    _stkProperties->addWidget(_viewPropertiesSequences);

    _viewPropertiesShots = new Views::ViewShots();
    _viewPropertiesShots->createShotRequested().connect(this, &Views::ViewProjects::_createShotRequested);
    _viewSequences->qtvSequences()->table()->selectionChanged().connect(this, &Views::ViewProjects::updateShotsView);

    _stkProperties->addWidget(_viewPropertiesShots);

    _viewPropertiesAssets = new Views::ViewAssets();
    _viewPropertiesAssets->createAssetRequested().connect(this, &Views::ViewProjects::_createAssetRequested);
    _qtvProjects->table()->selectionChanged().connect(this, &Views::ViewProjects::updateAssetsView);

    _stkProperties->addWidget(_viewPropertiesAssets);

    _viewPropertiesTasks = new Views::ViewTasks();
    _viewPropertiesTasks->createTaskRequested().connect(this, &Views::ViewProjects::_createTasksRequested);
    _viewPropertiesTasks->createTasksForTemplateRequested().connect(this, &Views::ViewProjects::_createTasksForTemplateRequested);

    _stkProperties->addWidget(_viewPropertiesTasks);

    _viewPropertiesTaskActivities = new Views::ViewTaskActivity();
    _viewPropertiesTaskActivities->createTaskActivityRequested().connect(this, &Views::ViewProjects::_createTaskActivityRequested);
    _viewPropertiesTaskActivities->createTaskActivitiesForTemplateRequested().connect(this, &Views::ViewProjects::_createTaskActivitiesForTemplateRequested);
    _viewPropertiesTaskActivities->removeTaskActivitiesRequested().connect(this, &Views::ViewProjects::_removeTaskActivitiesRequested);

    _stkProperties->addWidget(_viewPropertiesTaskActivities);

    _mnuNavBarPropertiesDataItem->select();//default selected item

    _propertiesPanel->addPropertiesView(this->id(), _cntPropertiesMain);
}

void Views::ViewProjects::_updatePropertiesDataView()
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

    const Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvPropertiesData = _viewPropertiesData->qtvData();

    _qtvPropertiesData->clearColumns();

    //add columns
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Database::DatabaseManager::instance().session()->find<Database::DboData>();

    bool update = false;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems());

            query.where("project_sequence_Sequence_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems());

            query.where("Project_Shot_Shot_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems());

            query.where("Project_Asset_Asset_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Project_Task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
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
        _qtvPropertiesData->addBaseColumns(flags, editRank);

    _qtvPropertiesData->updateView();
}

void Views::ViewProjects::_updatePropertiesTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
            return;

        try
        {
            bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
            Wt::WFlags<Wt::ItemFlag> flags;
            if(canEdit)
                flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
            else
                flags = Wt::ItemIsSelectable;

            int editRank = Auth::AuthManager::instance().currentUser()->editRank();

            const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesTags = _viewPropertiesTags->qtvTags();

            _qtvPropertiesTags->clearColumns();

            //add columns
            _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
            _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

            Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Database::DatabaseManager::instance().session()->find<Database::Tag>();

            if(_stkMain->currentWidget() == _cntProjects)
            {
                if(_qtvProjects->table()->selectedIndexes().size() > 0)
                {
                    std::string projectsSelectSql = "(pt.project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + "))";
                    query.where("(id IN (SELECT pt.tag_id FROM rel_project_tags pt WHERE " + projectsSelectSql + ") OR (Type IN ('Project', 'Global')))");
                }
                else
                    query.where("(Type IN ('Project', 'Global'))");
            }
            else
            {
                if(_qtvProjects->table()->selectedIndexes().size() > 0)
                {
                    std::string projectsSelectSql = "(pt.project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + "))";
                    query.where("(id IN (SELECT pt.tag_id FROM rel_project_tags pt WHERE " + projectsSelectSql + ") OR (Type = 'Global'))");
                }
                else
                    query.where("Type = 'Global'");
            }

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();

            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            query.where("View_Rank <= ?").bind(viewRank);

            _qtvPropertiesTags->setQuery(query);

            if(AppSettings::instance().isShowExtraColumns())
                _qtvPropertiesTags->addBaseColumns(flags, editRank);

            _qtvPropertiesTags->updateView();
        }
        catch(Wt::Dbo::Exception ex)
        {
            _logger->log(std::string("error occured while trying to update tags view ") + ex.what(), Ms::Log::LogMessageType::Error);
        }
}

void Views::ViewProjects::_updatePropertiesAssignedTagsView()
{
    if(!Database::DatabaseManager::instance().openTransaction())
        return;

    try
    {
        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesAssignedTags = _viewPropertiesTags->qtvAssignedTags();

        _qtvPropertiesAssignedTags->clearColumns();

        //add columns
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Database::DatabaseManager::instance().session()->find<Database::Tag>();

        bool update = false;

        if(_stkMain->currentWidget() == _cntProjects)
        {
            if(_qtvProjects->table()->selectedIndexes().size() > 0)
            {
                update = true;
                std::string projectsSelectSql = "(pt.project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + "))";
                query.where("(id IN (SELECT pt.tag_id FROM rel_project_assigned_tags pt WHERE " + projectsSelectSql + "))");
            }
        }
        else if(_stkMain->currentWidget() == _cntSequences)
        {
            if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems());

                std::string sequencesSelectSql = "(pt.project_sequence_Sequence_Name IN (" + idValues.at(0) + ") AND "
                        "pt.project_sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_sequence_assigned_tags pt WHERE " + sequencesSelectSql + "))");
            }
        }
        else if(_stkMain->currentWidget() == _cntShots)
        {
            if(_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems());

                std::string shotsSelectSql = "(pt.project_shot_Shot_Name IN (" + idValues.at(0) + ") AND "
                        "pt.project_shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ") AND "
                        "pt.project_shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_shot_assigned_tags pt WHERE " + shotsSelectSql + "))");
            }
        }
        else if(_stkMain->currentWidget() == _cntAssets)
        {
            if(_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems());

                std::string assetsSelectSql = "(pt.project_asset_Asset_Name IN (" + idValues.at(0) + ") AND "
                        "pt.project_asset_Asset_Project_Project_Name IN (" + idValues.at(1) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_asset_assigned_tags pt WHERE " + assetsSelectSql + "))");
            }
        }
        else if(_stkMain->currentWidget() == _cntTasks)
        {
            if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::string tasksSelectSql = "(pt.project_task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems()).at(0) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_task_assigned_tags pt WHERE " + tasksSelectSql + "))");
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
            _qtvPropertiesAssignedTags->addBaseColumns(flags, editRank);

        _qtvPropertiesAssignedTags->updateView();
    }
    catch(Wt::Dbo::Exception ex)
    {
        _logger->log(std::string("error occured while trying to update assigned tags view ") + ex.what(), Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewProjects::_updatePropertiesNotesView()
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

    const Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvPropertiesNotes = _viewPropertiesNotes->qtvNotes();

    _qtvPropertiesNotes->clearColumns();

    //add columns
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Database::DatabaseManager::instance().session()->find<Database::Note>();

    bool update = false;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("Project_Project_Name IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(_qtvProjects->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(_viewSequences->qtvSequences()->selectedItems());

            query.where("project_sequence_Sequence_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectShot>(_viewShots->qtvShots()->selectedItems());

            query.where("Project_Shot_Shot_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectAsset>(_viewAssets->qtvAssets()->selectedItems());

            query.where("Project_Asset_Asset_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(_stkMain->currentWidget() == _cntTasks)
    {
        if(_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Project_Task_id IN (" + Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectTask>(_viewTasks->qtvTasks()->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
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
        _qtvPropertiesNotes->addBaseColumns(flags, editRank);

    _qtvPropertiesNotes->updateView();
}

void Views::ViewProjects::_updatePropertiesSequencesView()
{
    _viewPropertiesSequences->updateView(_qtvProjects->selectedItems());
}

void Views::ViewProjects::_updatePropertiesShotsView()
{
    _viewPropertiesShots->updateView(_viewSequences->qtvSequences()->selectedItems());
}

void Views::ViewProjects::_updatePropertiesAssetsView()
{
    _viewPropertiesAssets->updateView(_qtvProjects->selectedItems());
}

void Views::ViewProjects::_updatePropertiesTasksView()
{
    std::vector<Wt::Dbo::ptr<Projects::Project>> prjVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> seqVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> shotVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> assetVec;

    if(_stkMain->currentWidget() == _cntProjects)
    {
        if(_qtvProjects->table()->selectedIndexes().size() > 0)
            prjVec = _qtvProjects->selectedItems();
    }
    else if(_stkMain->currentWidget() == _cntSequences)
    {
        if(_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
            seqVec = _viewSequences->qtvSequences()->selectedItems();
    }
    else if(_stkMain->currentWidget() == _cntShots)
    {
        if(_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
            shotVec =  _viewShots->qtvShots()->selectedItems();
    }
    else if(_stkMain->currentWidget() == _cntAssets)
    {
        if(_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
            assetVec = _viewAssets->qtvAssets()->selectedItems();
    }

    _viewPropertiesTasks->updateView(prjVec, seqVec, shotVec, assetVec);
}

void Views::ViewProjects::_updatePropertiesTaskActivitiesView()
{
    _viewPropertiesTaskActivities->updateView(_viewTasks->qtvTasks()->selectedItems());
}

void Views::ViewProjects::_prepareView()
{
    /*******************--Project--********************/
    setTitle("<b><i>Projects</i></b>");

    Wt::WVBoxLayout *_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    _layMain->setContentsMargins(14,14,14,14);

    _navBarMain= new Wt::WNavigationBar();

    _cntNavBarMain= new Wt::WContainerWidget();
    _cntNavBarMain->addWidget(_navBarMain);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarMain);

    _mnuMain = new Wt::WMenu(Wt::Horizontal);
    _navBarMain->addMenu(_mnuMain);

    _mnuMainProjectsItem = new Wt::WMenuItem("Projects");
    _mnuMainProjectsItem->triggered().connect(this, &Views::ViewProjects::_mnuMainProjectsItemTriggered);
    _mnuMain->addItem(_mnuMainProjectsItem);

    _mnuMainSequencesItem = new Wt::WMenuItem("Sequences");
    _mnuMainSequencesItem->triggered().connect(this, &Views::ViewProjects::_mnuMainSequencesItemTriggered);
    _mnuMain->addItem(_mnuMainSequencesItem);

    _mnuMainShotsItem = new Wt::WMenuItem("Shots");
    _mnuMainShotsItem->triggered().connect(this, &Views::ViewProjects::_mnuMainShotsItemTriggered);
    _mnuMain->addItem(_mnuMainShotsItem);

    _mnuMainAssetsItem = new Wt::WMenuItem("Assets");
    _mnuMainAssetsItem->triggered().connect(this, &Views::ViewProjects::_mnuMainAssetsItemTriggered);
    _mnuMain->addItem(_mnuMainAssetsItem);

    _mnuMainTasksItem = new Wt::WMenuItem("Tasks");
    _mnuMainTasksItem->triggered().connect(this, &Views::ViewProjects::_mnuMainTasksItemTriggered);
    _mnuMain->addItem(_mnuMainTasksItem);

    _stkMain = new Wt::WStackedWidget();
    _stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    _layMain->addWidget(_stkMain, 1);

    //Projects//////////////////////////////////////
    _layProjects = new Wt::WVBoxLayout();
    _layProjects->setContentsMargins(0,0,0,0);
    _layProjects->setSpacing(0);

    _cntProjects = new Wt::WContainerWidget();
    _cntProjects->setLayout(_layProjects);

    //add our projects view to the project view
    _stkMain->addWidget(_cntProjects);

    _createProjectsTableView();

    _layProjects->addWidget(_qtvProjects, 1);

    //Sequences//////////////////////////////////////
    _laySequences = new Wt::WVBoxLayout();
    _laySequences->setContentsMargins(0,0,0,0);
    _laySequences->setSpacing(0);

    _cntSequences = new Wt::WContainerWidget();
    _cntSequences->setLayout(_laySequences);

    //add our sequences view to the project view
    _stkMain->addWidget(_cntSequences);

    _createSequencesTableView();

    _laySequences->addWidget(_viewSequences, 1);

    //Shots///////////////////////////
    _layShots = new Wt::WVBoxLayout();
    _layShots->setContentsMargins(0,0,0,0);
    _layShots->setSpacing(0);

    _cntShots = new Wt::WContainerWidget();
    _cntShots->setLayout(_layShots);

    //add our shots view to the project view
    _stkMain->addWidget(_cntShots);

    _createShotsTableView();

    _layShots->addWidget(_viewShots, 1);

    //Assets//////////////////////////////////////
    _layAssets = new Wt::WVBoxLayout();
    _layAssets->setContentsMargins(0,0,0,0);
    _layAssets->setSpacing(0);

    _cntAssets = new Wt::WContainerWidget();
    _cntAssets->setLayout(_layAssets);

    //add our Assets view to the project view
    _stkMain->addWidget(_cntAssets);

    _createAssetsTableView();

    _layAssets->addWidget(_viewAssets, 1);

    //Tasks///////////////////////////
    _layTasks = new Wt::WVBoxLayout();
    _layTasks->setContentsMargins(0,0,0,0);
    _layTasks->setSpacing(0);

    _cntTasks = new Wt::WContainerWidget();
    _cntTasks->setLayout(_layTasks);

    //add our tasks view to the project view
    _stkMain->addWidget(_cntTasks);

    _createTasksTableView();

    _layTasks->addWidget(_viewTasks, 1);

    //Properties View
    _createPropertiesView();

    //updateViews
    updateSequencesView();
    updateShotsView();
    updateAssetsView();
    updateTasksView();
}
