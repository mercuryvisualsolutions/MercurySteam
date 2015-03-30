#include "viewprojects.h"

#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Projects/projectsio.h"
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
    m_logger = Session::SessionManager::instance().logger();
    m_propertiesPanel = Session::SessionManager::instance().propertiesPanel();

    prepareView();

    m_mnuMain->select(m_mnuMainProjectsItem);

    updateProjectsView();
    m_stkMain->setCurrentWidget(m_cntProjects);
}

void Views::ViewProjects::updateView()
{
    if(m_mnuMain->currentItem() == m_mnuMainProjectsItem)
        updateProjectsView();
    else if(m_mnuMain->currentItem() == m_mnuMainSequencesItem)
        updateSequencesView();
    else if(m_mnuMain->currentItem() == m_mnuMainShotsItem)
        updateShotsView();
    else if(m_mnuMain->currentItem() == m_mnuMainAssetsItem)
        updateAssetsView();
    else if(m_mnuMain->currentItem() == m_mnuMainTasksItem)
        updateTasksView();
}

void Views::ViewProjects::updateProjectsView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::Project>> query;

        int viewRank = Session::SessionManager::instance().user()->viewRank();

        if(AppSettings::instance().isLoadInactiveData())
            query = Session::SessionManager::instance().dboSession().find<Projects::Project>().where("View_Rank <= ?").bind(viewRank);
        else
            query = Session::SessionManager::instance().dboSession().find<Projects::Project>().where("View_Rank <= ? AND Active = ?").bind(viewRank).bind(true);

        m_qtvProjects->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvProjects->clearColumns();

        //add columns
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Widgets::Delegates::WorkStatusQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Manager_Name", "Manager", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Users::User>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Users::User>() :
         Session::SessionManager::instance().dboSession().find<Users::User>().where("Active = ?").bind(true),
         "Name", editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        m_qtvProjects->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvProjects->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvProjects->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating table view" << std::endl;
    }
}

void Views::ViewProjects::updateSequencesView()
{
    m_viewSequences->updateView(m_qtvProjects->selectedItems());
}

void Views::ViewProjects::updateShotsView()
{
    m_viewShots->updateView(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0 ?
                               m_viewSequences->qtvSequences()->selectedItems() : m_viewSequences->qtvSequences()->items());
}

void Views::ViewProjects::updateAssetsView()
{
    m_viewAssets->updateView(m_qtvProjects->selectedItems());
}

void Views::ViewProjects::updateTasksView()
{
    std::vector<Wt::Dbo::ptr<Projects::Project>> prjVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> seqVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> shotVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> assetVec;

    if(m_qtvProjects->table()->selectedIndexes().size() > 0)
    {
        prjVec = m_qtvProjects->selectedItems();

        seqVec = m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0 ?
                    m_viewSequences->qtvSequences()->selectedItems() : m_viewSequences->qtvSequences()->items();

        shotVec = m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0 ?
                    m_viewShots->qtvShots()->selectedItems() : m_viewShots->qtvShots()->items();

        assetVec = m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0 ?
                    m_viewAssets->qtvAssets()->selectedItems() : m_viewAssets->qtvAssets()->items();
    }

    m_viewTasks->updateView(prjVec, seqVec, shotVec, assetVec);
}

void Views::ViewProjects::updatePropertiesView()
{
    if(m_stkProperties->currentWidget() == m_viewPropertiesData)
    {
        updatePropertiesDataView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesTags)
    {
        updatePropertiesTagsView();
        updatePropertiesAssignedTagsView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesNotes)
    {
        updatePropertiesNotesView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesSequences)
    {
        updatePropertiesSequencesView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesShots)
    {
        updatePropertiesShotsView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesAssets)
    {
        updatePropertiesAssetsView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesTasks)
    {
        updatePropertiesTasksView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesTaskActivities)
    {
        updatePropertiesTaskActivitiesView();
    }
    else if(m_stkProperties->currentWidget() == m_viewPropertiesStatistics)
    {
        updatePropertiesStatisticsView();
    }
}

bool Views::ViewProjects::isProjectsViewShown()
{
    return m_mnuMain->currentItem() == m_mnuMainProjectsItem;
}

bool Views::ViewProjects::isSequencesViewShown()
{
    return m_mnuMain->currentItem() == m_mnuMainSequencesItem;
}

bool Views::ViewProjects::isShotsViewShown()
{
    return m_mnuMain->currentItem() == m_mnuMainShotsItem;
}

bool Views::ViewProjects::isAssetsViewShown()
{
    return m_mnuMain->currentItem() == m_mnuMainAssetsItem;
}

bool Views::ViewProjects::isTasksViewShown()
{
    return m_mnuMain->currentItem() == m_mnuMainTasksItem;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::Project> *Views::ViewProjects::projectsQueryTableView() const
{
    return m_qtvProjects;
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *Views::ViewProjects::sequencesQueryTableView() const
{
    return m_viewSequences->qtvSequences();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *Views::ViewProjects::shotsQueryTableView() const
{
    return m_viewShots->qtvShots();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *Views::ViewProjects::assetsQueryTableView() const
{
    return m_viewAssets->qtvAssets();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *Views::ViewProjects::tasksQueryTableView() const
{
    return m_viewTasks->qtvTasks();
}

void Views::ViewProjects::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    m_btnCreateProject->setHidden(!hasCreateProjectsPriv);
    m_btnImportProjectsThumbnails->setHidden(!hasEditPriv);
    m_btnEditProjects->setHidden(!hasEditPriv);

    m_qtvProjects->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    m_btnProjectsFiles->setHidden(!showTaskFilesButton);

    m_viewSequences->adjustUIPrivileges(user);
    m_viewShots->adjustUIPrivileges(user);
    m_viewAssets->adjustUIPrivileges(user);
    m_viewTasks->adjustUIPrivileges(user);
    m_viewPropertiesData->adjustUIPrivileges(user);
    m_viewPropertiesTags->adjustUIPrivileges(user);
    m_viewPropertiesNotes->adjustUIPrivileges(user);
    m_viewPropertiesSequences->adjustUIPrivileges(user);
    m_viewPropertiesShots->adjustUIPrivileges(user);
    m_viewPropertiesAssets->adjustUIPrivileges(user);
    m_viewPropertiesTasks->adjustUIPrivileges(user);

    //disable creation in main views
    m_viewSequences->setCreateOptionHidden(true);
    m_viewSequences->qtvSequences()->setImportCSVFeatureEnabled(false);
    m_viewShots->setCreateOptionHidden(true);
    m_viewShots->qtvShots()->setImportCSVFeatureEnabled(false);
    m_viewAssets->setCreateOptionHidden(true);
    m_viewAssets->qtvAssets()->setImportCSVFeatureEnabled(false);
    m_viewTasks->setCreateOptionHidden(true);
    m_viewTasks->setCreateForTemplateOptionHidden(true);
    m_viewTasks->qtvTasks()->setImportCSVFeatureEnabled(false);
}

Wt::Signal<> &Views::ViewProjects::onTabProjectsSelected()
{
    return m_onTabProjectsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabSequencesSelected()
{
    return m_onTabSequencesSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabShotsSelected()
{
    return m_onTabShotsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabAssetsSelected()
{
    return m_onTabAssetsSelected;
}

Wt::Signal<> &Views::ViewProjects::onTabTasksSelected()
{
    return m_onTabTasksSelected;
}

template<typename T>
void Views::ViewProjects::addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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
void Views::ViewProjects::addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec)
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

//Main
void Views::ViewProjects::mnuMainProjectsItemTriggered()
{
    if((m_stkProperties->currentWidget() == m_viewPropertiesShots) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesTaskActivities))
        m_mnuNavBarPropertiesDataItem->select();

    m_mnuNavBarPropertiesSequencesItem->show();
    m_mnuNavBarPropertiesShotsItem->hide();
    m_mnuNavBarPropertiesAssetsItem->show();
    m_mnuNavBarPropertiesTasksItem->show();
    m_mnuNavBarPropertiesTaskActivitiesItem->hide();

    m_viewPropertiesTags->setCreateOptionHidden(false);

    m_stkMain->setCurrentWidget(m_cntProjects);
    updateProjectsView();

    m_onTabProjectsSelected();
}

void Views::ViewProjects::mnuMainSequencesItemTriggered()
{
    if((m_stkProperties->currentWidget() == m_viewPropertiesSequences) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesAssets) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesTaskActivities))
    {
        m_mnuNavBarPropertiesDataItem->select();
    }

    m_mnuNavBarPropertiesSequencesItem->hide();
    m_mnuNavBarPropertiesShotsItem->show();
    m_mnuNavBarPropertiesAssetsItem->hide();
    m_mnuNavBarPropertiesTasksItem->show();
    m_mnuNavBarPropertiesTaskActivitiesItem->hide();

    m_viewPropertiesTags->setCreateOptionHidden(true);

    m_stkMain->setCurrentWidget(m_cntSequences);
    updateSequencesView();

    m_onTabSequencesSelected();
}

void Views::ViewProjects::mnuMainShotsItemTriggered()
{
    if((m_stkProperties->currentWidget() == m_viewPropertiesSequences) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesShots) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesAssets) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesTaskActivities))
    {
        m_mnuNavBarPropertiesDataItem->select();
    }

    m_mnuNavBarPropertiesSequencesItem->hide();
    m_mnuNavBarPropertiesShotsItem->hide();
    m_mnuNavBarPropertiesAssetsItem->hide();
    m_mnuNavBarPropertiesTasksItem->show();
    m_mnuNavBarPropertiesTaskActivitiesItem->hide();

    m_viewPropertiesTags->setCreateOptionHidden(true);

    m_stkMain->setCurrentWidget(m_cntShots);
    updateShotsView();

    m_onTabShotsSelected();
}

void Views::ViewProjects::mnuMainAssetsItemTriggered()
{
    if((m_stkProperties->currentWidget() == m_viewPropertiesSequences) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesShots) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesAssets) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesTaskActivities))
    {
        m_mnuNavBarPropertiesDataItem->select();
    }

    m_mnuNavBarPropertiesSequencesItem->hide();
    m_mnuNavBarPropertiesShotsItem->hide();
    m_mnuNavBarPropertiesAssetsItem->hide();
    m_mnuNavBarPropertiesTasksItem->show();
    m_mnuNavBarPropertiesTaskActivitiesItem->hide();

    m_viewPropertiesTags->setCreateOptionHidden(true);

    m_stkMain->setCurrentWidget(m_cntAssets);
    updateAssetsView();

    m_onTabAssetsSelected();
}

void Views::ViewProjects::mnuMainTasksItemTriggered()
{
    if((m_stkProperties->currentWidget() == m_viewPropertiesSequences) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesShots) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesAssets) ||
            (m_stkProperties->currentWidget() == m_viewPropertiesTasks))
    {
        m_mnuNavBarPropertiesDataItem->select();
    }

    m_mnuNavBarPropertiesSequencesItem->hide();
    m_mnuNavBarPropertiesShotsItem->hide();
    m_mnuNavBarPropertiesAssetsItem->hide();
    m_mnuNavBarPropertiesTasksItem->hide();
    m_mnuNavBarPropertiesTaskActivitiesItem->show();

    m_viewPropertiesTags->setCreateOptionHidden(true);

    m_stkMain->setCurrentWidget(m_cntTasks);
    updateTasksView();

    m_onTabTasksSelected();
}

//Projects
void Views::ViewProjects::btnProjectsCreateClicked()
{
    Views::DlgCreateAndEditProject *dlg = new Views::DlgCreateAndEditProject();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::Project>(dlg->projectName()))
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

                if(Session::SessionManager::instance().dboSession().createDbo<Projects::Project>(project))
                {
                    Projects::ProjectsIO::createProjectDirectoryStructure(dlg->projectName());
                    updateProjectsView();

                    m_logger->log(std::string("Created project ") + dlg->projectName(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete project;

                    m_logger->log(std::string("Error creating project ") + dlg->projectName(), Ms::Log::LogMessageType::Error);
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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::btnProjectsRemoveClicked()
{

}

void Views::ViewProjects::btnProjectsEditClicked()
{
    if(m_qtvProjects->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditProject *dlg = new Views::DlgCreateAndEditProject(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto prjPtr : m_qtvProjects->selectedItems())
            {
                if(dlg->editedStartDate())
                {
                    sendTaskNotification(prjPtr, "Start Date", Wt::asString(prjPtr->startDate()).toUTF8(), Wt::asString(dlg->startDate()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setStartDate(dlg->startDate());
                }

                if(dlg->editedEndDate())
                {
                    sendTaskNotification(prjPtr, "End Date", Wt::asString(prjPtr->endDate()).toUTF8(), Wt::asString(dlg->endDate()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setEndDate(dlg->endDate());
                }

                if(dlg->editedDuration())
                {
                    sendTaskNotification(prjPtr, "Duration", Wt::asString(prjPtr->durationInFrames()).toUTF8(), Wt::asString(dlg->duration()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setDurationInFrames(dlg->duration());
                }

                if(dlg->editedFps())
                {
                    sendTaskNotification(prjPtr, "FPS", Wt::asString(prjPtr->fps()).toUTF8(), Wt::asString(dlg->fps()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setFps(dlg->fps());
                }

                if(dlg->editedFrameWidth())
                {
                    sendTaskNotification(prjPtr, "Frame Width", Wt::asString(prjPtr->frameWidth()).toUTF8(), Wt::asString(dlg->frameWidth()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setFrameWidth(dlg->frameWidth());
                }

                if(dlg->editedFrameHeight())
                {
                    sendTaskNotification(prjPtr, "Frame Height", Wt::asString(prjPtr->frameHeight()).toUTF8(), Wt::asString(dlg->frameHeight()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setFrameHeight(dlg->frameHeight());
                }

                if(dlg->editedPriority())
                {
                    sendTaskNotification(prjPtr, "Priority", Wt::asString(prjPtr->priority()).toUTF8(), Wt::asString(dlg->priority()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setPriority(dlg->priority());
                }

                if(dlg->editedStatus())
                {
                    sendTaskNotification(prjPtr, "Status", prjPtr->status()->status(), dlg->status()->status());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setStatus(dlg->status());
                }
                if(dlg->editedManager())
                {
                    sendTaskNotification(prjPtr, "Manager", prjPtr->manager()->name(), dlg->manager()->name());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setManager(dlg->manager());
                }

                if(dlg->editedDescription())
                {
                    sendTaskNotification(prjPtr, "Description", prjPtr->description(), dlg->description());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setDescription(dlg->description());
                }

                if(dlg->editedActive())
                {
                    sendTaskNotification(prjPtr, "Active", Wt::asString(prjPtr->active()).toUTF8(), Wt::asString(dlg->isActive()).toUTF8());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(prjPtr)->setActive(dlg->isActive());
                }
            }

            transaction.commit();

            m_qtvProjects->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::btnProjectsFilesClicked()
{
    if(m_qtvProjects->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string prjName = m_qtvProjects->selectedItems().at(0)->name();

    DlgFilesManager *dlgFiles = new DlgFilesManager(Projects::ProjectsIO::getRelativeProjectDir(prjName) + Ms::IO::dirSeparator() + "files");
    dlgFiles->finished().connect(std::bind([=]()
    {
        delete dlgFiles;
    }));

    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    dlgFiles->setViewDisabled(!hasViewFilesPriv);
    dlgFiles->setCreateDisabled(!hasCreateRepoPriv);
    dlgFiles->setCheckInDisabled(!hasCheckInPriv);
    dlgFiles->setCheckOutDisabled(!hasCheckOutPriv);

    transaction.commit();

    dlgFiles->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::btnProjectsImportThumbnailsClicked()
{
    if(m_qtvProjects->model()->rowCount() == 0)
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

                    //match thumbnail by project name

                    Wt::Dbo::ptr<Projects::Project> prjPtr;

                    if(AppSettings::instance().isLoadInactiveData())
                        prjPtr = Session::SessionManager::instance().dboSession().find<Projects::Project>().where("Project_Name = ?").bind(rawFileName);
                    else
                        prjPtr = Session::SessionManager::instance().dboSession().find<Projects::Project>().where("Project_Name = ? AND Active = ?").bind(rawFileName).bind(true);

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
                    m_logger->log(std::string("Error occured while trying to import thumbnails to table projects") + e.what(),
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

            m_qtvProjects->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::projectImported(Wt::Dbo::ptr<Projects::Project> project)
{
    Projects::ProjectsIO::createProjectDirectoryStructure(project->name());
}

void Views::ViewProjects::projectDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role)
{
    //get project
    Wt::Dbo::ptr<Projects::Project> prjPtr = m_qtvProjects->itemForModelIndex(index);

    std::string headerName = Wt::asString(index.model()->headerData(index.column())).toUTF8();
    std::string orgValue = Wt::asString(index.data(role)).toUTF8();
    std::string newValue = Wt::asString(value).toUTF8();

    if(orgValue == newValue)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    std::string message = "Project \"" + prjPtr->name() + "\" \"" + headerName + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            //notify all users of tasks in current project of the change
            for(auto iter = prjPtr->tasks().begin(); iter != prjPtr->tasks().end(); ++iter)
            {
                (*iter)->user().modify()->addNotification(notificationPtr);
            }
        }
        else
        {
            delete notification;

            m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        delete notification;

        m_logger->log(ex.what(), Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        delete notification;

        m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewProjects::sendTaskNotification(Wt::Dbo::ptr<Projects::ProjectTask> taskPtr, const std::string &message)
{
    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            taskPtr->user().modify()->addNotification(notificationPtr);
        }
        else
        {
            delete notification;

            m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        delete notification;

        m_logger->log(ex.what(), Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        delete notification;

        m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewProjects::createProjectsTableView()
{
    m_qtvProjects = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::Project>(Session::SessionManager::instance().dboSession());
    m_qtvProjects->setRowHeight(160);
    m_qtvProjects->setDefaultFilterColumnIndex(1);
    m_qtvProjects->setIgnoreNumFilterColumns(1);
    m_qtvProjects->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    m_qtvProjects->itemImported().connect(this, &Views::ViewProjects::projectImported);

    Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::Project>> *model = const_cast<Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<Projects::Project>>*>(m_qtvProjects->model());

    model->dataAboutToBeChanged().connect(this, &Views::ViewProjects::projectDataAboutToBeChanged);

    m_btnCreateProject = m_qtvProjects->createToolButton("", "icons/Add.png", "Create A New Project");
    m_btnCreateProject->clicked().connect(this, &Views::ViewProjects::btnProjectsCreateClicked);

    //Wt::WPushButton *btn = m_qtvProjects->createToolButton("", "icons/Remove.png", "Remove Selected Project");
    //btn->clicked().connect(this, &Views::ViewProjects::btnProjectsRemoveClicked);

    m_btnEditProjects = m_qtvProjects->createToolButton("", "icons/Edit.png", "Edit Selected Projects");
    m_btnEditProjects->clicked().connect(this, &Views::ViewProjects::btnProjectsEditClicked);

    m_btnImportProjectsThumbnails = m_qtvProjects->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    m_btnImportProjectsThumbnails->clicked().connect(this, &Views::ViewProjects::btnProjectsImportThumbnailsClicked);

    m_btnProjectsFiles = m_qtvProjects->createToolButton("", "icons/Files.png", "Files Manager");
    m_btnProjectsFiles->clicked().connect(this, &Views::ViewProjects::btnProjectsFilesClicked);

    updateProjectsView();
}

void Views::ViewProjects::sendTaskNotification(Wt::Dbo::ptr<Projects::Project> prjPtr, const std::string &property, const std::string &orgValue, const std::string &newValue)
{
    if(orgValue == newValue)
        return;

    std::string message = "Project \"" + prjPtr->name() + "\" \"" + property + "\" has changed from \"" + orgValue + "\" to \"" + newValue + "\"";

    Database::Notification *notification = new Database::Notification(message);

    try
    {
        Wt::Dbo::ptr<Database::Notification> notificationPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Notification>(notification);

        if(notificationPtr.get())
        {
            //notify all users of tasks in current project of the change
            for(auto iter = prjPtr->tasks().begin(); iter != prjPtr->tasks().end(); ++iter)
            {
                (*iter)->user().modify()->addNotification(notificationPtr);
            }
        }
        else
        {
            delete notification;

            m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
        }
    }
    catch(Wt::Dbo::Exception ex)
    {
        delete notification;

        m_logger->log(ex.what(), Ms::Log::LogMessageType::Error);
    }
    catch(...)
    {
        delete notification;

        m_logger->log("Error occured while trying to create new notification", Ms::Log::LogMessageType::Error);
    }
}

//Sequences
void Views::ViewProjects::createSequenceRequested()
{
    if(m_qtvProjects->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one project.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditSequence *dlg = new Views::DlgCreateAndEditSequence();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::Project> prjPtr = m_qtvProjects->selectedItems().at(0);

            Projects::ProjectSequenceId id;
            id.name = (dlg->sequenceName());
            id.project = prjPtr;

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectSequence>(id))
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

                Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectSequence>(sequence);
                if(seqPtr.get())
                {
                    Projects::ProjectsIO::createSequenceDirectoryStructure(prjPtr->name(), seqPtr->name());
                    updatePropertiesSequencesView();

                    m_logger->log(std::string("Created sequence for project ") + prjPtr->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete sequence;

                    m_logger->log(std::string("Error creating sequence for project ") + prjPtr->name(), Ms::Log::LogMessageType::Error);
                }
            }
            else
            {
                m_logger->log(std::string("Object already exist"), Ms::Log::LogMessageType::Warning);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::sequenceImported(Wt::Dbo::ptr<Projects::ProjectSequence> sequence)
{
    Projects::ProjectsIO::createSequenceDirectoryStructure(sequence->projectName(), sequence->name());
}

void Views::ViewProjects::createSequencesTableView()
{
    m_viewSequences = new Views::ViewSequences();
    m_viewSequences->setCreateOptionHidden(true);
    m_viewSequences->qtvSequences()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    m_viewSequences->qtvSequences()->itemImported().connect(this, &Views::ViewProjects::sequenceImported);
}

//Shots
void Views::ViewProjects::createShotRequested()
{
    if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one sequence.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditShot *dlg = new Views::DlgCreateAndEditShot();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectSequence> seqPtr = m_viewSequences->qtvSequences()->selectedItems().at(0);

            Projects::ProjectShotId id;
            id.name = (dlg->shotName());
            id.sequence = seqPtr;

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectShot>(id))
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

                Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectShot>(shot);
                if(shotPtr.get())
                {
                    Projects::ProjectsIO::createShotDirectoryStructure(seqPtr->projectName(), seqPtr->name(), shotPtr->name());
                    updatePropertiesShotsView();

                    m_logger->log(std::string("Created shot for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete shot;

                    m_logger->log(std::string("error creating shot for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Error);
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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::shotImported(Wt::Dbo::ptr<Projects::ProjectShot> shot)
{
    Projects::ProjectsIO::createShotDirectoryStructure(shot->projectName(), shot->sequenceName(), shot->name());
}

void Views::ViewProjects::createShotsTableView()
{
    m_viewShots = new Views::ViewShots();
    m_viewShots->setCreateOptionHidden(true);
    m_viewShots->qtvShots()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    m_viewShots->qtvShots()->itemImported().connect(this, &Views::ViewProjects::shotImported);
}

//Assets
void Views::ViewProjects::createAssetRequested()
{
    if(m_qtvProjects->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one aroject.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditAsset *dlg = new Views::DlgCreateAndEditAsset();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::Project> prjPtr = m_qtvProjects->selectedItems().at(0);

            Projects::ProjectAssetId id;
            id.name = (dlg->assetName());
            id.project = prjPtr;

            if(!Session::SessionManager::instance().dboSession().dboExists<Projects::ProjectAsset>(id))
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

                Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectAsset>(asset);
                if(assetPtr.get())
                {
                    Projects::ProjectsIO::createAssetDirectoryStructure(prjPtr->name(), assetPtr->name());
                    updatePropertiesAssetsView();

                    m_logger->log(std::string("Created asset ") + dlg->assetName(), Ms::Log::LogMessageType::Info);
                }
                else
                {
                    delete asset;

                    m_logger->log(std::string("error creating asset ") + dlg->assetName(), Ms::Log::LogMessageType::Error);
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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::assetImported(Wt::Dbo::ptr<Projects::ProjectAsset> asset)
{
    Projects::ProjectsIO::createAssetDirectoryStructure(asset->projectName(), asset->name());
}

void Views::ViewProjects::createAssetsTableView()
{
    m_viewAssets = new Views::ViewAssets();
    m_viewAssets->setCreateOptionHidden(true);
    m_viewAssets->qtvAssets()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    m_viewAssets->qtvAssets()->itemImported().connect(this, &Views::ViewProjects::assetImported);
}

//Tasks
void Views::ViewProjects::createTasksRequested()
{
    if((m_stkMain->currentWidget() == m_cntProjects && m_qtvProjects->table()->selectedIndexes().size() > 0) ||
            (m_stkMain->currentWidget() == m_cntSequences && m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0) ||
            (m_stkMain->currentWidget() == m_cntShots && m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0) ||
            (m_stkMain->currentWidget() == m_cntAssets && m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0))
    {
        Views::DlgCreateAndEditTask *dlg = new Views::DlgCreateAndEditTask();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

                if(m_stkMain->currentWidget() == m_cntProjects)
                {
                    for(auto &prjPtr : m_qtvProjects->selectedItems())
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
                        task->setThumbnail(prjPtr->thumbnail());

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createProjectTaskDirectoryStructure(prjPtr->name(), taskPtr.id());
                            updatePropertiesTasksView();

                            m_logger->log(std::string("Created task for project ") + prjPtr->name(), Ms::Log::LogMessageType::Info);

                            // send notification to task user
                            std::string message = "A new task with Id \"" + std::to_string(taskPtr.id()) + "\" has been assigned to you.";
                            sendTaskNotification(taskPtr, message);
                        }
                        else
                        {
                            delete task;

                            m_logger->log(std::string("Error creating task for project") + prjPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }
                else if(m_stkMain->currentWidget() == m_cntSequences)
                {
                    for(auto &seqPtr : m_viewSequences->qtvSequences()->selectedItems())
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

                        if(dlg->thumbnail() == "Project")
                            task->setThumbnail(seqPtr->project()->thumbnail());
                        else
                            task->setThumbnail(seqPtr->thumbnail());

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createSequenceTaskDirectoryStructure(seqPtr->projectName(), seqPtr->name(), taskPtr.id());
                            updatePropertiesTasksView();

                            m_logger->log(std::string("Created task for sequence ") + seqPtr->name(), Ms::Log::LogMessageType::Info);

                            // send notification to task user
                            std::string message = "A new task with Id \"" + std::to_string(taskPtr.id()) + "\" has been assigned to you.";
                            sendTaskNotification(taskPtr, message);
                        }
                        else
                        {
                            delete task;

                            m_logger->log(std::string("Error creating task for sequence") + seqPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }
                else if(m_stkMain->currentWidget() == m_cntShots)
                {
                    for(auto &shotPtr : m_viewShots->qtvShots()->selectedItems())
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

                        if(dlg->thumbnail() == "Project")
                            task->setThumbnail(shotPtr->sequence()->project()->thumbnail());
                        else if(dlg->thumbnail() == "Sequence")
                            task->setThumbnail(shotPtr->id().sequence->thumbnail());
                        else
                            task->setThumbnail(shotPtr->thumbnail());

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createShotTaskDirectoryStructure(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name(), taskPtr.id());
                            updatePropertiesTasksView();

                            m_logger->log(std::string("Created task for shot ") + shotPtr->name(), Ms::Log::LogMessageType::Info);

                            // send notification to task user
                            std::string message = "A new task with Id \"" + std::to_string(taskPtr.id()) + "\" has been assigned to you.";
                            sendTaskNotification(taskPtr, message);
                        }
                        else
                        {
                            delete task;

                            m_logger->log(std::string("Error creating task for shot") + shotPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }
                else if(m_stkMain->currentWidget() == m_cntAssets)
                {
                    for(auto &assetPtr : m_viewAssets->qtvAssets()->selectedItems())
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

                        if(dlg->thumbnail() == "Project")
                            task->setThumbnail(assetPtr->project()->thumbnail());
                        else
                            task->setThumbnail(assetPtr->thumbnail());

                        Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                        if(taskPtr.get())
                        {
                            Projects::ProjectsIO::createAssetTaskDirectoryStructure(assetPtr->projectName(), assetPtr->projectName(), taskPtr.id());
                            updatePropertiesTasksView();

                            m_logger->log(std::string("Created task for asset ") + assetPtr->name(), Ms::Log::LogMessageType::Info, Log::LogMessageContext::ServerAndClient);

                            // send notification to task user
                            std::string message = "A new task with Id \"" + std::to_string(taskPtr.id()) + "\" has been assigned to you.";
                            sendTaskNotification(taskPtr, message);
                        }
                        else
                        {
                            delete task;

                            m_logger->log(std::string("error creating task for asset ") + assetPtr->name(), Ms::Log::LogMessageType::Error);
                        }
                    }
                }

                transaction.commit();

                updateTasksView();
            }

            delete dlg;
        }));

        dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
    }
    else
    {
        m_logger->log(std::string("Please select at least one item"), Ms::Log::LogMessageType::Warning);
    }
}

void Views::ViewProjects::createTasksForTemplateRequested()
{
    if((m_stkMain->currentWidget() == m_cntProjects && m_qtvProjects->table()->selectedIndexes().size() > 0) ||
            (m_stkMain->currentWidget() == m_cntSequences && m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0) ||
            (m_stkMain->currentWidget() == m_cntShots && m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0) ||
            (m_stkMain->currentWidget() == m_cntAssets && m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0))
    {
        Views::DlgSelectTaskTemplate *dlg = new Views::DlgSelectTaskTemplate();
        dlg->finished().connect(std::bind([=]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

                Wt::Dbo::ptr<Projects::ProjectTaskTemplate> templatePtr = dlg->taskTemplate();

                if(m_stkMain->currentWidget() == m_cntProjects ||
                        m_stkMain->currentWidget() == m_cntSequences ||
                        m_stkMain->currentWidget() == m_cntShots ||
                        m_stkMain->currentWidget() == m_cntAssets)
                {
                    if(m_stkMain->currentWidget() == m_cntProjects)
                    {
                        for(auto &prjPtr : m_qtvProjects->selectedItems())
                        {
                            for(auto iter = templatePtr->items().begin(); iter != templatePtr->items().end(); ++iter)
                            {
                                if(!(*iter)->active())
                                    continue;

                                Projects::ProjectTask *task = new Projects::ProjectTask();
                                task->setProject(prjPtr);
                                task->setStatus((*iter)->status());
                                task->setType((*iter)->type());
                                task->setDescription((*iter)->description());

                                Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                            }
                        }

                        m_logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                    }
                    else if(m_stkMain->currentWidget() == m_cntSequences)
                    {
                        for(auto &seqPtr : m_viewSequences->qtvSequences()->selectedItems())
                        {
                            for(auto iter = templatePtr->items().begin(); iter != templatePtr->items().end(); ++iter)
                            {
                                if(!(*iter)->active())
                                    continue;

                                Projects::ProjectTask *task = new Projects::ProjectTask();
                                task->setSequence(seqPtr);
                                task->setStatus((*iter)->status());
                                task->setType((*iter)->type());
                                task->setDescription((*iter)->description());

                                Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                            }
                        }

                        m_logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                    }
                    else if(m_stkMain->currentWidget() == m_cntShots)
                    {
                        for(auto &shotPtr : m_viewShots->qtvShots()->selectedItems())
                        {
                            for(auto iter = templatePtr->items().begin(); iter != templatePtr->items().end(); ++iter)
                            {
                                if(!(*iter)->active())
                                    continue;

                                Projects::ProjectTask *task = new Projects::ProjectTask();
                                task->setShot(shotPtr);
                                task->setStatus((*iter)->status());
                                task->setType((*iter)->type());
                                task->setDescription((*iter)->description());

                                Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                            }

                            m_logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                        }
                    }
                    else if(m_stkMain->currentWidget() == m_cntAssets)
                    {
                        for(auto &assetPtr : m_viewAssets->qtvAssets()->selectedItems())
                        {
                            for(auto iter = templatePtr->items().begin(); iter != templatePtr->items().end(); ++iter)
                            {
                                if(!(*iter)->active())
                                    continue;

                                Projects::ProjectTask *task = new Projects::ProjectTask();
                                task->setAsset(assetPtr);
                                task->setStatus((*iter)->status());
                                task->setType((*iter)->type());
                                task->setDescription((*iter)->description());

                                Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTask>(task);
                            }

                            m_logger->log(std::string("Created tasks for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
                        }
                    }

                    updatePropertiesTasksView();
                }

                transaction.commit();
            }

            delete dlg;
        }));

        dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
    }
    else
    {
        m_logger->log(std::string("Please select at least one item"), Ms::Log::LogMessageType::Warning);
    }
}

void Views::ViewProjects::taskImported(Wt::Dbo::ptr<Projects::ProjectTask> task)
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

void Views::ViewProjects::createTasksTableView()
{
    m_viewTasks = new Views::ViewTasks();
    m_viewTasks->setCreateOptionHidden(true);
    m_viewTasks->setCreateForTemplateOptionHidden(true);
    m_viewTasks->qtvTasks()->tableSelectionChanged().connect(this, &Views::ViewProjects::updatePropertiesView);
    m_viewTasks->qtvTasks()->itemImported().connect(this, &Views::ViewProjects::taskImported);
}

//Properties
void Views::ViewProjects::mnuNavBarPropertiesDataItemTriggered()
{
    updatePropertiesDataView();
    m_stkProperties->setCurrentWidget(m_viewPropertiesData);
}

void Views::ViewProjects::mnuNavBarPropertiesTagsItemTriggered()
{
    updatePropertiesTagsView();
    updatePropertiesAssignedTagsView();
    m_stkProperties->setCurrentWidget(m_viewPropertiesTags);
}

void Views::ViewProjects::mnuNavBarPropertiesNotesItemTriggered()
{
    updatePropertiesNotesView();
    m_stkProperties->setCurrentWidget(m_viewPropertiesNotes);
}

void Views::ViewProjects::mnuNavBarPropertiesSequencesItemTriggered()
{
    m_viewPropertiesSequences->updateView(m_qtvProjects->selectedItems());
    m_stkProperties->setCurrentWidget(m_viewPropertiesSequences);
}

void Views::ViewProjects::mnuNavBarPropertiesShotsItemTriggered()
{
    m_viewPropertiesShots->updateView(m_viewSequences->qtvSequences()->selectedItems());
    m_stkProperties->setCurrentWidget(m_viewPropertiesShots);
}

void Views::ViewProjects::mnuNavBarPropertiesAssetsItemTriggered()
{
    m_viewPropertiesAssets->updateView(m_qtvProjects->selectedItems());
    m_stkProperties->setCurrentWidget(m_viewPropertiesAssets);
}

void Views::ViewProjects::mnuNavBarPropertiesTasksItemTriggered()
{
    updatePropertiesTasksView();
    m_stkProperties->setCurrentWidget(m_viewPropertiesTasks);
}

void Views::ViewProjects::mnuNavBarPropertiesTaskActivitiesItemTriggered()
{
    updatePropertiesTaskActivitiesView();
    m_stkProperties->setCurrentWidget(m_viewPropertiesTaskActivities);
}

void Views::ViewProjects::mnuNavBarPropertiesStatisticsItemTriggered()
{
    updatePropertiesStatisticsView();
    m_stkProperties->setCurrentWidget(m_viewPropertiesStatistics);
}

void Views::ViewProjects::addDataRequested()
{
    if(m_stkMain->currentWidget() == m_cntProjects)
        addDataToDbo<Projects::Project>(m_qtvProjects->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntSequences)
        addDataToDbo<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntShots)
        addDataToDbo<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntAssets)
        addDataToDbo<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntTasks)
        addDataToDbo<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems());
}

void Views::ViewProjects::removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec)
{

}

void Views::ViewProjects::createProjectTagRequested()
{
    bool createTag = false;
    bool customProjectTag = false;

    if(m_qtvProjects->table()->selectedIndexes().size() > 0)
    {
        if(m_qtvProjects->table()->selectedIndexes().size() != 1)
            m_logger->log("Please select only one project.", Ms::Log::LogMessageType::Warning);
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
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Database::Tag *tag = new Database::Tag(dlg->tagName(), dlg->tagContent());
            if(customProjectTag)
                tag->setType("Local");
            else
                tag->setType("Project");

            tag->setActive(dlg->isActive());

            Wt::Dbo::ptr<Database::Tag> tagPtr = Session::SessionManager::instance().dboSession().createDbo<Database::Tag>(tag);

            if(tagPtr.get())
            {
                if(customProjectTag)
                {
                    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::Project>(m_qtvProjects->selectedItems().at(0))->addTag(tagPtr);

                    transaction.commit();
                }

                updatePropertiesTagsView();
            }
            else
                delete tag;

            transaction.commit();
       }

       delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(tagVec.size() == 0)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        if(m_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Projects::Project>(m_qtvProjects->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        if(m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        if(m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().assignTagsToDbo<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }

    transaction.commit();
}

void Views::ViewProjects::unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec)
{
    if(tagVec.size() == 0)
        return;

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        if(m_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Projects::Project>(m_qtvProjects->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        if(m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        if(m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
        {
            Session::SessionManager::instance().dboSession().unassignTagsFromDbo<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems(), tagVec);

            updatePropertiesAssignedTagsView();
        }
    }

    transaction.commit();
}

void Views::ViewProjects::filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec, bool exactSelection, bool inverse)
{
    std::string strFilterQuery = "";

    if(tagVec.size() == 0)
        return;

    std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Database::Tag>(tagVec);

    std::string inverseIn = inverse ? "NOT " : "";
    std::string andOr = inverse ? "OR" : "AND";

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        std::string matchBy = exactSelection ? " GROUP BY pt.project_Project_Name HAVING COUNT(DISTINCT pt.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Project_Name " + inverseIn + "IN (SELECT pt.project_Project_Name FROM rel_project_assigned_tags pt WHERE tag_id IN (" + idValues.at(0) + ")" + matchBy + ")";

        m_qtvProjects->setCustomFilterString(strFilterQuery);
        m_qtvProjects->setCustomFilterActive(true);
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        std::string projectMatchBy = exactSelection ? " GROUP BY st.project_sequence_Sequence_Project_Project_Name HAVING COUNT(DISTINCT st.tag_id) = " + std::to_string(tagVec.size()) : "";
        std::string projectSequenceMatchBy = exactSelection ? " GROUP BY st.project_sequence_Sequence_Name HAVING COUNT(DISTINCT st.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Sequence_Name " + inverseIn + "IN (SELECT st.project_sequence_Sequence_Name FROM rel_project_sequence_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")" + projectSequenceMatchBy + ") " + andOr +
                " Sequence_Project_Project_Name " + inverseIn + "IN (SELECT st.project_sequence_Sequence_Project_Project_Name FROM rel_project_sequence_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")" + projectMatchBy + ")";

        m_viewSequences->qtvSequences()->setCustomFilterString(strFilterQuery);
        m_viewSequences->qtvSequences()->setCustomFilterActive(true);
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        std::string projectMatchBy = exactSelection ? " GROUP BY st.project_shot_Shot_Sequence_Sequence_Project_Project_Name HAVING COUNT(DISTINCT st.tag_id) = " + std::to_string(tagVec.size()) : "";
        std::string projectSequenceMatchBy = exactSelection ? " GROUP BY st.project_shot_Shot_Sequence_Sequence_Name HAVING COUNT(DISTINCT st.tag_id) = " + std::to_string(tagVec.size()) : "";
        std::string projectShotMatchBy = exactSelection ? " GROUP BY st.project_shot_Shot_Name HAVING COUNT(DISTINCT st.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Shot_Name " + inverseIn + "IN (SELECT st.project_shot_Shot_Name FROM rel_project_shot_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")" + projectShotMatchBy + ") " + andOr +
                " Shot_Sequence_Sequence_Name " + inverseIn + "IN (SELECT st.project_shot_Shot_Sequence_Sequence_Name FROM rel_project_shot_assigned_tags st WHERE tag_id IN(" + idValues.at(0) + ")" + projectSequenceMatchBy + ") " + andOr +
                " Shot_Sequence_Sequence_Project_Project_Name " + inverseIn + "IN (SELECT st.project_shot_Shot_Sequence_Sequence_Project_Project_Name FROM rel_project_shot_assigned_tags st WHERE tag_id IN (" + idValues.at(0) + ")" + projectMatchBy + ")";

        m_viewShots->qtvShots()->setCustomFilterString(strFilterQuery);
        m_viewShots->qtvShots()->setCustomFilterActive(true);
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        std::string projectMatchBy = exactSelection ? " GROUP BY at.project_asset_Asset_Project_Project_Name HAVING COUNT(DISTINCT at.tag_id) = " + std::to_string(tagVec.size()) : "";
        std::string projectAssetMatchBy = exactSelection ? " GROUP BY at.project_asset_Asset_Name HAVING COUNT(DISTINCT at.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "Asset_Name " + inverseIn + "IN (SELECT at.project_asset_Asset_Name FROM rel_project_asset_assigned_tags at WHERE tag_id IN (" + idValues.at(0) + ")" + projectAssetMatchBy + ") " + andOr +
                "Asset_Project_Project_Name " + inverseIn + "IN (SELECT at.project_asset_Asset_Project_Project_Name FROM rel_project_asset_assigned_tags at WHERE tag_id IN (" + idValues.at(0) + ")" + projectMatchBy + ")";

        m_viewAssets->qtvAssets()->setCustomFilterString(strFilterQuery);
        m_viewAssets->qtvAssets()->setCustomFilterActive(true);
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        std::string taskMatchBy = exactSelection ? " GROUP BY tt.project_task_id HAVING COUNT(DISTINCT tt.tag_id) = " + std::to_string(tagVec.size()) : "";

        strFilterQuery = "id " + inverseIn + "IN (SELECT tt.project_task_id FROM rel_project_task_assigned_tags tt WHERE tag_id IN (" + idValues.at(0) + ")" + taskMatchBy + ")";

        m_viewTasks->qtvTasks()->setCustomFilterString(strFilterQuery);
        m_viewTasks->qtvTasks()->setCustomFilterActive(true);
    }
}

void Views::ViewProjects::clearTagsFilterRequested()
{
    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        m_qtvProjects->setCustomFilterString("");
        m_qtvProjects->setCustomFilterActive(false);
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        m_viewSequences->qtvSequences()->setCustomFilterString("");
        m_viewSequences->qtvSequences()->setCustomFilterActive(false);
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        m_viewShots->qtvShots()->setCustomFilterString("");
        m_viewShots->qtvShots()->setCustomFilterActive(false);
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        m_viewAssets->qtvAssets()->setCustomFilterString("");
        m_viewAssets->qtvAssets()->setCustomFilterActive(false);
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        m_viewTasks->qtvTasks()->setCustomFilterString("");
        m_viewTasks->qtvTasks()->setCustomFilterActive(false);
    }
}

void Views::ViewProjects::addNoteRequested()
{
    if(m_stkMain->currentWidget() == m_cntProjects)
        addNoteToDbo<Projects::Project>(m_qtvProjects->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntSequences)
         addNoteToDbo<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntShots)
        addNoteToDbo<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntAssets)
        addNoteToDbo<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems());
    else if(m_stkMain->currentWidget() == m_cntTasks)
        addNoteToDbo<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems());
}

void Views::ViewProjects::removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec)
{

}

void Views::ViewProjects::createTaskActivityRequested()
{
    if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one task.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditTaskActivity *dlg = new Views::DlgCreateAndEditTaskActivity();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = m_viewTasks->qtvTasks()->selectedItems().at(0);

            Projects::ProjectTaskActivity *activity = new Projects::ProjectTaskActivity();
            activity->setTask(taskPtr);
            activity->setType(dlg->type());
            activity->setStatus(dlg->status());
            activity->setDescription(dlg->description());
            activity->setActive(dlg->isActive());

            Wt::Dbo::ptr<Projects::ProjectTaskActivity> activityPtr = Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskActivity>(activity);
            if(activityPtr.get())
            {
                updatePropertiesTaskActivitiesView();

                m_logger->log(std::string("Created task activity ") + std::to_string(activityPtr.id()), Ms::Log::LogMessageType::Info);
            }
            else
            {
                delete activity;

                m_logger->log(std::string("error creating task activity"), Ms::Log::LogMessageType::Error);
            }

            transaction.commit();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::createTaskActivitiesForTemplateRequested()
{
    if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one task.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgSelectActivityTemplate *dlg = new Views::DlgSelectActivityTemplate();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            Wt::Dbo::ptr<Projects::ProjectActivityTemplate> templatePtr = dlg->activityTemplate();

            Wt::Dbo::ptr<Projects::ProjectTask> taskPtr = m_viewTasks->qtvTasks()->selectedItems().at(0);

            for(auto iter = templatePtr->items().begin(); iter != templatePtr->items().end(); ++iter)
            {
                if(!(*iter)->active())
                    continue;

                Projects::ProjectTaskActivity *activity = new Projects::ProjectTaskActivity();
                activity->setTask(taskPtr);
                activity->setStatus((*iter)->status());
                activity->setType((*iter)->type());
                activity->setHours((*iter)->hours());
                activity->setDescription((*iter)->description());

                Session::SessionManager::instance().dboSession().createDbo<Projects::ProjectTaskActivity>(activity);
            }

            transaction.commit();

            updatePropertiesTaskActivitiesView();

            m_logger->log(std::string("Created task activities for template ") + templatePtr->name(), Ms::Log::LogMessageType::Info);
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewProjects::removeTaskActivitiesRequested(const std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> &activityVec)
{

}

void Views::ViewProjects::createPropertiesView()
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
    m_mnuNavBarPropertiesDataItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesDataItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesDataItem);

    m_mnuNavBarPropertiesTagsItem = new Wt::WMenuItem("Tags");
    m_mnuNavBarPropertiesTagsItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesTagsItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesTagsItem);

    m_mnuNavBarPropertiesNotesItem = new Wt::WMenuItem("Notes");
    m_mnuNavBarPropertiesNotesItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesNotesItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesNotesItem);

    m_mnuNavBarPropertiesSequencesItem = new Wt::WMenuItem("Sequences");
    m_mnuNavBarPropertiesSequencesItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesSequencesItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesSequencesItem);

    m_mnuNavBarPropertiesShotsItem = new Wt::WMenuItem("Shots");
    m_mnuNavBarPropertiesShotsItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesShotsItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesShotsItem);

    m_mnuNavBarPropertiesAssetsItem = new Wt::WMenuItem("Assets");
    m_mnuNavBarPropertiesAssetsItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesAssetsItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesAssetsItem);

    m_mnuNavBarPropertiesTasksItem = new Wt::WMenuItem("Tasks");
    m_mnuNavBarPropertiesTasksItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesTasksItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesTasksItem);

    m_mnuNavBarPropertiesTaskActivitiesItem = new Wt::WMenuItem("Activities");
    m_mnuNavBarPropertiesTaskActivitiesItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesTaskActivitiesItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesTaskActivitiesItem);

    m_mnuNavBarPropertiesStatisticsItem = new Wt::WMenuItem("Statistics");
    m_mnuNavBarPropertiesStatisticsItem->triggered().connect(this, &Views::ViewProjects::mnuNavBarPropertiesStatisticsItemTriggered);
    m_mnuNavBarProperties->addItem(m_mnuNavBarPropertiesStatisticsItem);

    m_stkProperties = new Wt::WStackedWidget();

    m_layCntPropertiesMain->addWidget(m_stkProperties, 1);

    //Data
    m_viewPropertiesData = new Views::ViewDboData();
    m_viewPropertiesData->addDataRequested().connect(this, &Views::ViewProjects::addDataRequested);
    m_viewPropertiesData->removeDataRequested().connect(this, &Views::ViewProjects::removeDataRequested);

    m_stkProperties->addWidget(m_viewPropertiesData);

    //Tags
    m_viewPropertiesTags = new Views::ViewTags();
    m_viewPropertiesTags->createTagRequested().connect(this, &Views::ViewProjects::createProjectTagRequested);
    m_viewPropertiesTags->assignTagsRequested().connect(this, &Views::ViewProjects::assignTagsRequested);
    m_viewPropertiesTags->unassignTagsRequested().connect(this, &Views::ViewProjects::unassignTagsRequested);
    m_viewPropertiesTags->filterByTagsRequested().connect(this, &Views::ViewProjects::filterByTagsRequested);
    m_viewPropertiesTags->clearTagsFilterRequested().connect(this, &Views::ViewProjects::clearTagsFilterRequested);

    m_stkProperties->addWidget(m_viewPropertiesTags);

    //Notes
    m_viewPropertiesNotes = new Views::ViewNotes();
    m_viewPropertiesNotes->addNoteRequested().connect(this, &Views::ViewProjects::addNoteRequested);
    m_viewPropertiesNotes->removeNotesRequested().connect(this, &Views::ViewProjects::removeNotesRequested);

    m_stkProperties->addWidget(m_viewPropertiesNotes);

    //Sequences
    m_viewPropertiesSequences = new Views::ViewSequences();
    m_viewPropertiesSequences->qtvSequences()->setRowHeight(25);
    m_viewPropertiesSequences->createSequenceRequested().connect(this, &Views::ViewProjects::createSequenceRequested);
    m_qtvProjects->table()->selectionChanged().connect(this, &Views::ViewProjects::updateSequencesView);

    m_stkProperties->addWidget(m_viewPropertiesSequences);

    //Shots
    m_viewPropertiesShots = new Views::ViewShots();
    m_viewPropertiesShots->qtvShots()->setRowHeight(25);
    m_viewPropertiesShots->createShotRequested().connect(this, &Views::ViewProjects::createShotRequested);
    m_viewSequences->qtvSequences()->table()->selectionChanged().connect(this, &Views::ViewProjects::updateShotsView);

    m_stkProperties->addWidget(m_viewPropertiesShots);

    //Assets
    m_viewPropertiesAssets = new Views::ViewAssets();
    m_viewPropertiesAssets->qtvAssets()->setRowHeight(25);
    m_viewPropertiesAssets->createAssetRequested().connect(this, &Views::ViewProjects::createAssetRequested);
    m_qtvProjects->table()->selectionChanged().connect(this, &Views::ViewProjects::updateAssetsView);

    m_stkProperties->addWidget(m_viewPropertiesAssets);

    //Tasks
    m_viewPropertiesTasks = new Views::ViewTasks();
    m_viewPropertiesTasks->createTaskRequested().connect(this, &Views::ViewProjects::createTasksRequested);
    m_viewPropertiesTasks->createTasksForTemplateRequested().connect(this, &Views::ViewProjects::createTasksForTemplateRequested);

    m_stkProperties->addWidget(m_viewPropertiesTasks);

    //TaskActivities
    m_viewPropertiesTaskActivities = new Views::ViewTaskActivity();
    m_viewPropertiesTaskActivities->createTaskActivityRequested().connect(this, &Views::ViewProjects::createTaskActivityRequested);
    m_viewPropertiesTaskActivities->createTaskActivitiesForTemplateRequested().connect(this, &Views::ViewProjects::createTaskActivitiesForTemplateRequested);
    m_viewPropertiesTaskActivities->removeTaskActivitiesRequested().connect(this, &Views::ViewProjects::removeTaskActivitiesRequested);

    m_stkProperties->addWidget(m_viewPropertiesTaskActivities);

    //Statistics
    m_viewPropertiesStatistics = new Ms::Widgets::MTableViewWidget();
    m_viewPropertiesStatistics->setImportCSVFeatureEnabled(false);
    m_stkProperties->addWidget(m_viewPropertiesStatistics);


    m_mnuNavBarPropertiesDataItem->select();//default selected item

    m_propertiesPanel->addPropertiesView(this->id(), m_cntPropertiesMain);
}

void Views::ViewProjects::updatePropertiesDataView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvPropertiesData = m_viewPropertiesData->qtvData();

    _qtvPropertiesData->clearColumns();

    //add columns
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOKey", "Key", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("DBOValue", "Value", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::DboData>> query = Session::SessionManager::instance().dboSession().find<Database::DboData>();

    bool update = false;

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        if(m_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("Project_Project_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(m_qtvProjects->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems());

            query.where("project_sequence_Sequence_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        if(m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems());

            query.where("Project_Shot_Shot_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        if(m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems());

            query.where("Project_Asset_Asset_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Project_Task_id IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesData->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
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

void Views::ViewProjects::updatePropertiesTagsView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesTags = m_viewPropertiesTags->qtvTags();

        _qtvPropertiesTags->clearColumns();

        //add columns
        _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Session::SessionManager::instance().dboSession().find<Database::Tag>();

        if(m_stkMain->currentWidget() == m_cntProjects)
        {
            if(m_qtvProjects->table()->selectedIndexes().size() > 0)
            {
                std::string projectsSelectSql = "(pt.project_Project_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(m_qtvProjects->selectedItems()).at(0) + "))";
                query.where("(id IN (SELECT pt.tag_id FROM rel_project_tags pt WHERE " + projectsSelectSql + ") OR (Type IN ('Project', 'Global')))");
            }
            else
                query.where("(Type IN ('Project', 'Global'))");
        }
        else
        {
            if(m_qtvProjects->table()->selectedIndexes().size() > 0)
            {
                std::string projectsSelectSql = "(pt.project_Project_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(m_qtvProjects->selectedItems()).at(0) + "))";
                query.where("(id IN (SELECT pt.tag_id FROM rel_project_tags pt WHERE " + projectsSelectSql + ") OR (Type = 'Global'))");
            }
            else
                query.where("Type = 'Global'");
        }

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
    catch(Wt::Dbo::Exception ex)
    {
        m_logger->log(std::string("error occured while trying to update tags view ") + ex.what(), Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewProjects::updatePropertiesAssignedTagsView()
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesAssignedTags = m_viewPropertiesTags->qtvAssignedTags();

        _qtvPropertiesAssignedTags->clearColumns();

        //add columns
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesAssignedTags->addColumn(Ms::Widgets::MQueryTableViewColumn("Type", "Type", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

        Wt::Dbo::Query<Wt::Dbo::ptr<Database::Tag>> query = Session::SessionManager::instance().dboSession().find<Database::Tag>();

        bool update = false;

        if(m_stkMain->currentWidget() == m_cntProjects)
        {
            if(m_qtvProjects->table()->selectedIndexes().size() > 0)
            {
                update = true;
                std::string projectsSelectSql = "(pt.project_Project_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(m_qtvProjects->selectedItems()).at(0) + "))";
                query.where("(id IN (SELECT pt.tag_id FROM rel_project_assigned_tags pt WHERE " + projectsSelectSql + "))");
            }
        }
        else if(m_stkMain->currentWidget() == m_cntSequences)
        {
            if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems());

                std::string sequencesSelectSql = "(pt.project_sequence_Sequence_Name IN (" + idValues.at(0) + ") AND "
                        "pt.project_sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_sequence_assigned_tags pt WHERE " + sequencesSelectSql + "))");
            }
        }
        else if(m_stkMain->currentWidget() == m_cntShots)
        {
            if(m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems());

                std::string shotsSelectSql = "(pt.project_shot_Shot_Name IN (" + idValues.at(0) + ") AND "
                        "pt.project_shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ") AND "
                        "pt.project_shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_shot_assigned_tags pt WHERE " + shotsSelectSql + "))");
            }
        }
        else if(m_stkMain->currentWidget() == m_cntAssets)
        {
            if(m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems());

                std::string assetsSelectSql = "(pt.project_asset_Asset_Name IN (" + idValues.at(0) + ") AND "
                        "pt.project_asset_Asset_Project_Project_Name IN (" + idValues.at(1) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_asset_assigned_tags pt WHERE " + assetsSelectSql + "))");
            }
        }
        else if(m_stkMain->currentWidget() == m_cntTasks)
        {
            if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
            {
                update = true;

                std::string tasksSelectSql = "(pt.project_task_id IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems()).at(0) + "))";

                query.where("(id IN (SELECT pt.tag_id FROM rel_project_task_assigned_tags pt WHERE " + tasksSelectSql + "))");
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
    catch(Wt::Dbo::Exception ex)
    {
        m_logger->log(std::string("error occured while trying to update assigned tags view ") + ex.what(), Ms::Log::LogMessageType::Error);
    }
}

void Views::ViewProjects::updatePropertiesNotesView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
    Wt::WFlags<Wt::ItemFlag> flags;
    if(canEdit)
        flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
    else
        flags = Wt::ItemIsSelectable;

    int editRank = Session::SessionManager::instance().user()->editRank();

    Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvPropertiesNotes = m_viewPropertiesNotes->qtvNotes();

    _qtvPropertiesNotes->clearColumns();

    //add columns
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("id", "Id", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Content", "Content", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));

    Wt::Dbo::Query<Wt::Dbo::ptr<Database::Note>> query = Session::SessionManager::instance().dboSession().find<Database::Note>();

    bool update = false;

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        if(m_qtvProjects->table()->selectedIndexes().size() > 0)
        {
            update = true;
            query.where("Project_Project_Name IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(m_qtvProjects->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectSequence>(m_viewSequences->qtvSequences()->selectedItems());

            query.where("project_sequence_Sequence_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        if(m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectShot>(m_viewShots->qtvShots()->selectedItems());

            query.where("Project_Shot_Shot_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Name IN (" + idValues.at(1) + ")"
                        " AND Project_Shot_Shot_Sequence_Sequence_Project_Project_Name IN (" + idValues.at(2) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Name", "Shot Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Shot_Shot_Sequence_Sequence_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        if(m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            std::vector<std::string> idValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectAsset>(m_viewAssets->qtvAssets()->selectedItems());

            query.where("Project_Asset_Asset_Name IN (" + idValues.at(0) + ")"
                        " AND Project_Asset_Asset_Project_Project_Name IN (" + idValues.at(1) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Name", "Asset Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Asset_Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        if(m_viewTasks->qtvTasks()->table()->selectedIndexes().size() > 0)
        {
            update = true;

            query.where("Project_Task_id IN (" + Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectTask>(m_viewTasks->qtvTasks()->selectedItems()).at(0) + ")");
        }

        _qtvPropertiesNotes->addColumn(Ms::Widgets::MQueryTableViewColumn("Project_Task_id", "Task ID", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
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

void Views::ViewProjects::updatePropertiesSequencesView()
{
    m_viewPropertiesSequences->updateView(m_qtvProjects->selectedItems());
}

void Views::ViewProjects::updatePropertiesShotsView()
{
    m_viewPropertiesShots->updateView(m_viewSequences->qtvSequences()->selectedItems());
}

void Views::ViewProjects::updatePropertiesAssetsView()
{
    m_viewPropertiesAssets->updateView(m_qtvProjects->selectedItems());
}

void Views::ViewProjects::updatePropertiesTasksView()
{
    std::vector<Wt::Dbo::ptr<Projects::Project>> prjVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> seqVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> shotVec;
    std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> assetVec;

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        if(m_qtvProjects->table()->selectedIndexes().size() > 0)
            prjVec = m_qtvProjects->selectedItems();
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        if(m_viewSequences->qtvSequences()->table()->selectedIndexes().size() > 0)
            seqVec = m_viewSequences->qtvSequences()->selectedItems();
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        if(m_viewShots->qtvShots()->table()->selectedIndexes().size() > 0)
            shotVec =  m_viewShots->qtvShots()->selectedItems();
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        if(m_viewAssets->qtvAssets()->table()->selectedIndexes().size() > 0)
            assetVec = m_viewAssets->qtvAssets()->selectedItems();
    }

    m_viewPropertiesTasks->updateView(prjVec, seqVec, shotVec, assetVec);
}

void Views::ViewProjects::updatePropertiesTaskActivitiesView()
{
    m_viewPropertiesTaskActivities->updateView(m_viewTasks->qtvTasks()->selectedItems());
}

void Views::ViewProjects::updatePropertiesStatisticsView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    m_viewPropertiesStatistics->clear();

    if(m_stkMain->currentWidget() == m_cntProjects)
    {
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Project Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Progress"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Sequences"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Sequences"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Shots"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Shots"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Assets"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Assets"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Activities"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Activities"));      

        for(Wt::Dbo::ptr<Projects::Project> &project : m_qtvProjects->selectedItems())
        {
            std::vector<Wt::WStandardItem*> items;

            items.push_back(new Wt::WStandardItem(project->name()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->progress()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->totalHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->doneHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->totalSequences()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->doneSequences()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->totalShots()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->doneShots()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->totalAssets()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->doneAssets()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->totalTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->doneTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->totalActivities()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(project->doneActivities()).c_str()));

            m_viewPropertiesStatistics->model()->appendRow(items);
        }
    }
    else if(m_stkMain->currentWidget() == m_cntSequences)
    {
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Project Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Sequence Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Progress"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Shots"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Shots"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Activities"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Activities"));

        for(Wt::Dbo::ptr<Projects::ProjectSequence> &seq : m_viewSequences->qtvSequences()->selectedItems())
        {
            std::vector<Wt::WStandardItem*> items;

            items.push_back(new Wt::WStandardItem(seq->project()->name()));
            items.push_back(new Wt::WStandardItem(seq->name()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->progress()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->totalHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->doneHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->totalShots()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->doneShots()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->totalTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->doneTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->totalActivities()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(seq->doneActivities()).c_str()));

            m_viewPropertiesStatistics->model()->appendRow(items);
        }
    }
    else if(m_stkMain->currentWidget() == m_cntShots)
    {
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Project Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Sequence Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Shot Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Progress"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Activities"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Activities"));

        for(Wt::Dbo::ptr<Projects::ProjectShot> &shot : m_viewShots->qtvShots()->selectedItems())
        {
            std::vector<Wt::WStandardItem*> items;

            items.push_back(new Wt::WStandardItem(shot->project()->name()));
            items.push_back(new Wt::WStandardItem(shot->sequence()->name()));
            items.push_back(new Wt::WStandardItem(shot->name()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->progress()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->totalHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->doneHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->totalTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->doneTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->totalActivities()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(shot->doneActivities()).c_str()));

            m_viewPropertiesStatistics->model()->appendRow(items);
        }
    }
    else if(m_stkMain->currentWidget() == m_cntAssets)
    {
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Project Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Asset Name"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Progress"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Finished Tasks"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Activities"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Finished Activities"));

        for(Wt::Dbo::ptr<Projects::ProjectAsset> &asset : m_viewAssets->qtvAssets()->selectedItems())
        {
            std::vector<Wt::WStandardItem*> items;

            items.push_back(new Wt::WStandardItem(asset->project()->name()));
            items.push_back(new Wt::WStandardItem(asset->name()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->progress()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->totalHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->doneHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->totalTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->doneTasks()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->totalActivities()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(asset->doneActivities()).c_str()));

            m_viewPropertiesStatistics->model()->appendRow(items);
        }
    }
    else if(m_stkMain->currentWidget() == m_cntTasks)
    {
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Task Id"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Progress"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Done Hours"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Total Activities"));
        m_viewPropertiesStatistics->addColumn(Ms::Core::MTableViewColumn("Finished Activities"));

        for(Wt::Dbo::ptr<Projects::ProjectTask> &task : m_viewTasks->qtvTasks()->selectedItems())
        {
            std::vector<Wt::WStandardItem*> items;

            items.push_back(new Wt::WStandardItem(std::to_string(task.id())));
            items.push_back(new Wt::WStandardItem(std::to_string(task->progress()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(task->totalHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(task->doneHours()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(task->totalActivities()).c_str()));
            items.push_back(new Wt::WStandardItem(std::to_string(task->doneActivities()).c_str()));

            m_viewPropertiesStatistics->model()->appendRow(items);
        }
    }

    transaction.commit();
}

void Views::ViewProjects::prepareView()
{
    /*******************--Project--********************/
    setTitle("<b><i>Projects</i></b>");

    Wt::WVBoxLayout *m_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    m_layMain->setContentsMargins(14,0,14,14);

    m_navBarMain= new Wt::WNavigationBar();

    m_cntNavBarMain= new Wt::WContainerWidget();
    m_cntNavBarMain->addWidget(m_navBarMain);

    //add our navigation bar to the view
    m_layMain->addWidget(m_cntNavBarMain);

    m_mnuMain = new Wt::WMenu(Wt::Horizontal);
    m_navBarMain->addMenu(m_mnuMain);

    m_mnuMainProjectsItem = new Wt::WMenuItem("Projects");
    m_mnuMainProjectsItem->triggered().connect(this, &Views::ViewProjects::mnuMainProjectsItemTriggered);
    m_mnuMain->addItem(m_mnuMainProjectsItem);

    m_mnuMainSequencesItem = new Wt::WMenuItem("Sequences");
    m_mnuMainSequencesItem->triggered().connect(this, &Views::ViewProjects::mnuMainSequencesItemTriggered);
    m_mnuMain->addItem(m_mnuMainSequencesItem);

    m_mnuMainShotsItem = new Wt::WMenuItem("Shots");
    m_mnuMainShotsItem->triggered().connect(this, &Views::ViewProjects::mnuMainShotsItemTriggered);
    m_mnuMain->addItem(m_mnuMainShotsItem);

    m_mnuMainAssetsItem = new Wt::WMenuItem("Assets");
    m_mnuMainAssetsItem->triggered().connect(this, &Views::ViewProjects::mnuMainAssetsItemTriggered);
    m_mnuMain->addItem(m_mnuMainAssetsItem);

    m_mnuMainTasksItem = new Wt::WMenuItem("Tasks");
    m_mnuMainTasksItem->triggered().connect(this, &Views::ViewProjects::mnuMainTasksItemTriggered);
    m_mnuMain->addItem(m_mnuMainTasksItem);

    m_stkMain = new Wt::WStackedWidget();
    m_stkMain->setTransitionAnimation(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut), true);

    m_layMain->addWidget(m_stkMain, 1);

    //Projects//////////////////////////////////////
    m_layProjects = new Wt::WVBoxLayout();
    m_layProjects->setContentsMargins(0,0,0,0);
    m_layProjects->setSpacing(0);

    m_cntProjects = new Wt::WContainerWidget();
    m_cntProjects->setLayout(m_layProjects);

    //add our projects view to the project view
    m_stkMain->addWidget(m_cntProjects);

    createProjectsTableView();

    m_layProjects->addWidget(m_qtvProjects, 1);

    //Sequences//////////////////////////////////////
    m_laySequences = new Wt::WVBoxLayout();
    m_laySequences->setContentsMargins(0,0,0,0);
    m_laySequences->setSpacing(0);

    m_cntSequences = new Wt::WContainerWidget();
    m_cntSequences->setLayout(m_laySequences);

    //add our sequences view to the project view
    m_stkMain->addWidget(m_cntSequences);

    createSequencesTableView();

    m_laySequences->addWidget(m_viewSequences, 1);

    //Shots///////////////////////////
    m_layShots = new Wt::WVBoxLayout();
    m_layShots->setContentsMargins(0,0,0,0);
    m_layShots->setSpacing(0);

    m_cntShots = new Wt::WContainerWidget();
    m_cntShots->setLayout(m_layShots);

    //add our shots view to the project view
    m_stkMain->addWidget(m_cntShots);

    createShotsTableView();

    m_layShots->addWidget(m_viewShots, 1);

    //Assets//////////////////////////////////////
    m_layAssets = new Wt::WVBoxLayout();
    m_layAssets->setContentsMargins(0,0,0,0);
    m_layAssets->setSpacing(0);

    m_cntAssets = new Wt::WContainerWidget();
    m_cntAssets->setLayout(m_layAssets);

    //add our Assets view to the project view
    m_stkMain->addWidget(m_cntAssets);

    createAssetsTableView();

    m_layAssets->addWidget(m_viewAssets, 1);

    //Tasks///////////////////////////
    m_layTasks = new Wt::WVBoxLayout();
    m_layTasks->setContentsMargins(0,0,0,0);
    m_layTasks->setSpacing(0);

    m_cntTasks = new Wt::WContainerWidget();
    m_cntTasks->setLayout(m_layTasks);

    //add our tasks view to the project view
    m_stkMain->addWidget(m_cntTasks);

    createTasksTableView();

    m_layTasks->addWidget(m_viewTasks, 1);

    //Properties View
    createPropertiesView();

    //updateViews
    updateSequencesView();
    updateShotsView();
    updateAssetsView();
    updateTasksView();
}
