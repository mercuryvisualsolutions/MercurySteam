#include "MFilesUploadDialog.h"

Ms::Widgets::Dialogs::MFilesUploadDialog::MFilesUploadDialog()
{
    _prepareView();
}

Ms::Widgets::Dialogs::MFilesUploadDialog::MFilesUploadDialog(bool selectMultipleFiles, bool keepFiles) :
    MFilesUploadDialog()
{
    _filesUpload->setMultiple(selectMultipleFiles);
    _keepFiles = keepFiles;
}

const std::vector<Wt::Http::UploadedFile> Ms::Widgets::Dialogs::MFilesUploadDialog::httpUploadedFiles() const
{
    return _filesUpload->uploadedFiles();
}

const std::string Ms::Widgets::Dialogs::MFilesUploadDialog::spoolFileName() const
{
    return _filesUpload->spoolFileName();
}

const std::string Ms::Widgets::Dialogs::MFilesUploadDialog::clientFileName() const
{
    return _filesUpload->clientFileName().toUTF8();
}

bool Ms::Widgets::Dialogs::MFilesUploadDialog::isMultibleSelectionEnabled()
{
    return _filesUpload->multiple();
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::setMultibleSelectionEnabled(bool enabled)
{
    _filesUpload->setMultiple(enabled);
}

bool Ms::Widgets::Dialogs::MFilesUploadDialog::keepFiles()
{
    return _keepFiles;
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::setKeepFiles(bool keepFiles)
{
    _keepFiles = keepFiles;
}

const std::map<std::string, std::string> Ms::Widgets::Dialogs::MFilesUploadDialog::uploadedFilesMap() const
{
    std::map<std::string,std::string> filesMap;
    for(const Wt::Http::UploadedFile &file : _filesUpload->uploadedFiles())
    {
        filesMap[file.spoolFileName()] = file.clientFileName();
        std::cout << "uploaded file:" << file.spoolFileName() << std::endl;
    }

    return filesMap;
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::_prepareView()
{
    this->setCaption("Upload Files");
    this->rejectWhenEscapePressed(true);

    _layContents = new Wt::WVBoxLayout();
    _layContents->setContentsMargins(0,0,0,0);
    _layContents->setSpacing(0);

    this->contents()->setLayout(_layContents);

    _prgsBarUpload = new Wt::WProgressBar();

    _filesUpload = new Wt::WFileUpload();
    _filesUpload->stealSpooledFile();
    _filesUpload->setFileTextSize(40);
    _filesUpload->setProgressBar(_prgsBarUpload);
    _filesUpload->uploaded().connect(this, &Dialogs::MFilesUploadDialog::_uploadSuccessfull);
    _filesUpload->fileTooLarge().connect(this, &Dialogs::MFilesUploadDialog::_fileTooLarge);

    _layContents->addWidget(_filesUpload);

    _lblStatus = new Wt::WLabel();
    _layContents->addWidget(_lblStatus);

    _btnUpload = new Wt::WPushButton("Upload", this->footer());
    _btnUpload->clicked().connect(this, &Dialogs::MFilesUploadDialog::_btnUploadClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::_btnUploadClicked()
{
    _btnUpload->setDisabled(false);
    _btnCancel->setDisabled(false);

    _filesUpload->upload();

    _lblStatus->setText("Uploading...");
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::_uploadSuccessfull()
{
    _lblStatus->setText("Upload successfull");
    if(_keepFiles)
    {
        for(const Wt::Http::UploadedFile &file : _filesUpload->uploadedFiles())
        {
            file.stealSpoolFile();
        }
    }

    if(_filesUpload->uploadedFiles().size() > 0)
        this->accept();
    else
        this->reject();
}

void Ms::Widgets::Dialogs::MFilesUploadDialog::_fileTooLarge()
{
    std::cerr << "One of the uploaded files is too large" << std::endl;
    _lblStatus->setText("One of the uploaded files is too large");

    _btnUpload->setDisabled(false);
    _btnCancel->setDisabled(false);
}
