#ifndef DBSCHEMAATTRIBUTES_H
#define DBSCHEMAATTRIBUTES_H

#include <iostream>
#include <vector>

namespace Database
{
    namespace
    {
    //due to some mysql 6.5 restrictions:
    //-Can't alter many tables at once in mysql, so we use vector instead of a simple std::string
    //-mysql 5.6 prevents modification to columns with foreign keys without dropping the foreign keys first, then readding them in
    //another ALTER TABLE statement
        std::vector<std::string> sqlAttributesCommands =
                {"ALTER TABLE project\n"
                "CHANGE COLUMN Thumbnail Thumbnail VARCHAR(255) NOT NULL DEFAULT 'pics/NoPreviewBig.png',\n"
                "CHANGE COLUMN Date_Created Date_Created DATETIME NOT NULL DEFAULT '2014-01-01 12:00:00',\n"
                "CHANGE COLUMN Start_Date Start_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN End_Date End_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN Duration_In_Frames Duration_In_Frames INT(11) NOT NULL DEFAULT '100',\n"
                "CHANGE COLUMN FPS FPS FLOAT NOT NULL DEFAULT '25.0',\n"
                "CHANGE COLUMN Frame_Width Frame_Width INT(11) NOT NULL DEFAULT '1920',\n"
                "CHANGE COLUMN Frame_Height Frame_Height INT(11) NOT NULL DEFAULT '1080',\n"
                "CHANGE COLUMN Description Description TEXT NOT NULL,\n"
                "CHANGE COLUMN Progress Progress INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1',\n"
                 //mysql 5.6 prevents modification to columns with foreign keys without dropping them first
                "DROP FOREIGN KEY fk_project_Project_Manager,\n"
                "DROP FOREIGN KEY fk_project_Current,\n"
                "CHANGE COLUMN Current_Status Current_Status VARCHAR(255) NOT NULL DEFAULT 'In Progress',\n"
                "CHANGE COLUMN Project_Manager_Name Project_Manager_Name VARCHAR(255) NOT NULL DEFAULT 'admin';\n",
                 //add the foreign keys again
                "ALTER TABLE project\n"
                "ADD CONSTRAINT fk_project_Project_Manager FOREIGN KEY (Project_Manager_Name) REFERENCES user(Name),\n"
                "ADD CONSTRAINT fk_project_Current FOREIGN KEY (Current_Status) REFERENCES project_work_status(Status);",

                "ALTER TABLE project_sequence\n"
                "CHANGE COLUMN Thumbnail Thumbnail VARCHAR(255) NOT NULL DEFAULT 'pics/NoPreviewBig.png',\n"
                "CHANGE COLUMN Date_Created Date_Created DATETIME NOT NULL DEFAULT '2014-01-01 12:00:00',\n"
                "CHANGE COLUMN Start_Date Start_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN End_Date End_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN Duration_In_Frames Duration_In_Frames INT(11) NOT NULL DEFAULT '100',\n"
                "CHANGE COLUMN FPS FPS FLOAT NOT NULL DEFAULT '25.0',\n"
                "CHANGE COLUMN Frame_Width Frame_Width INT(11) NOT NULL DEFAULT '1920',\n"
                "CHANGE COLUMN Frame_Height Frame_Height INT(11) NOT NULL DEFAULT '1080',\n"
                "CHANGE COLUMN Description Description TEXT NOT NULL,\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1',\n"
                "DROP FOREIGN KEY fk_project_sequence_Current,\n"
                "CHANGE COLUMN Current_Status Current_Status VARCHAR(255) NOT NULL DEFAULT 'In Progress';",

                "ALTER TABLE project_sequence\n"
                "ADD CONSTRAINT fk_project_sequence_Current FOREIGN KEY (Current_Status) REFERENCES project_work_status(Status);",

                "ALTER TABLE project_shot\n"
                "CHANGE COLUMN Thumbnail Thumbnail VARCHAR(255) NOT NULL DEFAULT 'pics/NoPreviewBig.png',\n"
                "CHANGE COLUMN Date_Created Date_Created DATETIME NOT NULL DEFAULT '2014-01-01 12:00:00',\n"
                "CHANGE COLUMN Start_Date Start_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN End_Date End_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN Duration_In_Frames Duration_In_Frames INT(11) NOT NULL DEFAULT '100',\n"
                "CHANGE COLUMN FPS FPS FLOAT NOT NULL DEFAULT '25.0',\n"
                "CHANGE COLUMN Frame_Width Frame_Width INT(11) NOT NULL DEFAULT '1920',\n"
                "CHANGE COLUMN Frame_Height Frame_Height INT(11) NOT NULL DEFAULT '1080',\n"
                "CHANGE COLUMN Description Description TEXT NOT NULL,\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1',\n"
                "DROP FOREIGN KEY fk_project_shot_Current,\n"
                "CHANGE COLUMN Current_Status Current_Status VARCHAR(255) NOT NULL DEFAULT 'In Progress';",

                "ALTER TABLE project_shot\n"
                "ADD CONSTRAINT fk_project_shot_Current FOREIGN KEY (Current_Status) REFERENCES project_work_status(Status);",

                "ALTER TABLE project_asset\n"
                "CHANGE COLUMN Thumbnail Thumbnail VARCHAR(255) NOT NULL DEFAULT 'pics/NoPreviewBig.png',\n"
                "CHANGE COLUMN Date_Created Date_Created DATETIME NOT NULL DEFAULT '2014-01-01 12:00:00',\n"
                "CHANGE COLUMN Start_Date Start_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN End_Date End_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN Description Description TEXT NOT NULL,\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1',\n"
                "CHANGE COLUMN Current_Status Current_Status VARCHAR(255) NOT NULL DEFAULT 'In Progress',\n"
                "DROP FOREIGN KEY fk_project_asset_Current,\n"
                "DROP FOREIGN KEY fk_project_asset_Asset,\n"
                "CHANGE COLUMN Asset_Type Asset_Type VARCHAR(255) NOT NULL DEFAULT '3D-Model';",

                "ALTER TABLE project_asset\n"
                "ADD CONSTRAINT fk_project_asset_Current FOREIGN KEY (Current_Status) REFERENCES project_work_status(Status),\n"
                "ADD CONSTRAINT fk_project_asset_Asset FOREIGN KEY (Asset_Type) REFERENCES project_asset_type(Type);",

                "ALTER TABLE project_task\n"
                "CHANGE COLUMN Date_Created Date_Created DATETIME NOT NULL DEFAULT '2014-01-01 12:00:00',\n"
                "CHANGE COLUMN Start_Date Start_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN End_Date End_Date DATE NOT NULL DEFAULT '2014-01-01',\n"
                "CHANGE COLUMN Description Description TEXT NOT NULL,\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1',\n"
                "DROP FOREIGN KEY fk_project_task_Current,\n"
                "DROP FOREIGN KEY fk_project_task_Task,\n"
                "CHANGE COLUMN Current_Status Current_Status VARCHAR(255) NOT NULL DEFAULT 'In Progress',\n"
                "CHANGE COLUMN Task_Type Task_Type VARCHAR(255) NOT NULL DEFAULT 'Animation';",

                "ALTER TABLE project_task\n"
                "ADD CONSTRAINT fk_project_task_Current FOREIGN KEY (Current_Status) REFERENCES project_work_status(Status),\n"
                "ADD CONSTRAINT fk_project_task_Task FOREIGN KEY (Task_Type) REFERENCES project_task_type(Type);",

                "ALTER TABLE dbodata\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE `group`\n"
                "CHANGE COLUMN Rank Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE privilege\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE project_asset_type\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE project_task_type\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE project_work_status\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE project_work_status_type\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE project_work_status_type\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE user\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';",

                "ALTER TABLE user_title\n"
                "CHANGE COLUMN View_Rank View_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Edit_Rank Edit_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Remove_Rank Remove_Rank INT(11) NOT NULL DEFAULT '0',\n"
                "CHANGE COLUMN Active Active TINYINT(1) NOT NULL DEFAULT '1';"
                };
    }
}

#endif // DBSCHEMAATTRIBUTES_H
