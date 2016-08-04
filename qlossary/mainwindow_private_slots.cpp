/*
 *
 * File: mainwindow_private_slots.cpp
 * Description: this file contains private methods for Main Window's slots like triggers...
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_mn_Help_About_Qlossary_triggered() {

    AboutQlossary dlg_about;
    dlg_about.setModal(true);
    dlg_about.exec();

}

void MainWindow::on_mnProject_New_triggered() {
    QStringList project;
    NewProject dlg_newproject;

    project = dbconn.getAllLanguages();
    if (project.at(0) == "Error") {
        return;
    }

    dlg_newproject.setData(project);
    dlg_newproject.setModal(true);

    if ( QDialog::Accepted == dlg_newproject.exec() ) {
        /* ---- user accepts ----
        * dialog here is closed, but not destroyed
        * all it's data is in reach
        * so we can do anything with the data
        */
        project = dlg_newproject.getData();
        if ( (project.at(0) == "") || (project.at(1) == "") || (project.at(2) == "") ){
            return;
        }

        // [0] = Project name, [1] = Orig Lang ID, [2] = Target Lang ID
        dbconn.ins_in_project(project.at(0), project.at(1), project.at(2));
    }
}

void MainWindow::on_mnProject_NewLanguage_triggered() {
    QStringList language;
    NewLanguage dlg_newlanguage;

    dlg_newlanguage.setModal(true);
    if ( QDialog::Accepted == dlg_newlanguage.exec() ) {
        language = dlg_newlanguage.getData();
        if ( (language.at(0) == "") || (language.at(1) == "") ) {
            return;
        }
        // [0] = Language name, [1] = Lang Isocode
        dbconn.ins_in_language(language.at(0), language.at(1));
    }
}

void MainWindow::on_mnProject_ViewLanguages_triggered() {
    QStringList languages;
    ViewLanguages dlg_viewLanguages;

    languages = dbconn.getAllLanguages();

    dlg_viewLanguages.setData(languages);
    dlg_viewLanguages.setModal(true);
    dlg_viewLanguages.exec();

}

void MainWindow::on_mnProject_ViewProjects_triggered() {
    QStringList projects;
    QPair<double, QString> cur_project;
    ViewProjects dlg_viewprojects;

    projects = dbconn.getAllProjects4List();

    dlg_viewprojects.setData(projects);
    dlg_viewprojects.setModal(true);

    if ( QDialog::Accepted == dlg_viewprojects.exec() ) {
        cur_project = dlg_viewprojects.get_project();
        this->setWindowTitle(PROG_TITLE + cur_project.second);
        this->statusBar()->showMessage("Selected project: " + cur_project.second);
    }

}

void MainWindow::on_mnView_Languages_triggered() {
    ui->dck_languages->show();
}

void MainWindow::on_mnView_Projects_triggered() {
    ui->dck_projects->show();
}

void MainWindow::on_mnView_NewTerm_triggered() {
    ui->dck_newterm->show();
}


