/*
 *
 * File: mainwindow_dck_project.cpp
 * Description: this file contains methods for Main Window's Project dock
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * TAB: New
 * BUTTON: ADD
 * Description: Adds a new project
 *
 * */
void MainWindow::on_btn_projAdd_clicked() {

    dbconn.ins_in_project(ui->ln_projName->text(),
                          langaux.at(ui->cmb_projOrigLang->currentIndex()),
                          langaux.at(ui->cmb_projTargLang->currentIndex()));

    this->fill_projects_widgets();
    ui->ln_projName->clear();

    if (ui->lst_langActives->count() > 0) {
        ui->tab_projects->setCurrentIndex(0);
    }
}

/*
 * Note: set btn_projAdd object as Autodefault into UI Design
 *  to activate 'click' event when a user press Enter over this object.
 *  That's why we don't need next method:
 *
 * void MainWindow::on_btn_projAdd_pressed() {
 *     this->on_btn_projAdd_clicked();
 * }
 *
 * */


/*
 * TAB: Actives
 * BUTTON: Remove
 * Description: Moves selected project to Trash
 *
 * */
void MainWindow::on_btn_projRemove_clicked() {
    QString project;
    QList <QListWidgetItem*> items;
    unsigned int row;

    items = ui->lst_projActives->selectedItems();
    if (items.count() < 1) { // At least 1 item must be selected
        return;
    }

    row = ui->lst_projActives->currentRow();
    project = ui->lst_projActives->currentItem()->text();

    qDebug() << "Project Active to Trash: " << project;

    dbconn.tb_project_2_trash(projaux.at(row));


    this->fill_projects_widgets();
    this->fill_projTrash_widgets();

    ui->statusBar->showMessage("Project to trash: "+ project, 5000);
    if (ui->lst_projActives->count() > 0) {
        ui->tab_projects->setCurrentIndex(0);
    }
}

/*
 * TAB: Trash
 * BUTTON: Activate
 * Description: Actives selected project from Trash
 *
 * */
void MainWindow::on_btn_projActivate_trash_clicked() {
    QString project;
    unsigned int row;

    if (ui->lst_projTrash->selectedItems().count() < 1) { // At least 1 item must be selected
        return;
    }

    row = ui->lst_projTrash->currentRow();

    dbconn.tb_project_recover_from_trash(projTrashList.at(row).id,
                                         projTrashList.at(row).olangID,
                                         projTrashList.at(row).tlangID);
    this->fill_projects_widgets();
    this->fill_projTrash_widgets();
    this->fill_languages_widgets();
    this->fill_langTrash_widgets();

    ui->statusBar->showMessage("Project recovered from trash: "+ project, 5000);

    if (ui->lst_projActives->count() > 0) {
        ui->tab_projects->setCurrentIndex(0);
    }
}

/*
 * TAB: Trash
 * BUTTON: RTemove
 * Description: Removes definitely selected project from Trash
 *
 * */
void MainWindow::on_btn_projRemove_trash_clicked() {
    QString project;
    unsigned int row;

    if (ui->lst_projTrash->selectedItems().count() < 1) { // At least 1 item must be selected
        return;
    }

    row = ui->lst_projTrash->currentRow();
    dbconn.tb_project_remove_from_trash(projTrashList.at(row).id);
    this->fill_projTrash_widgets();

    if (ui->lst_projActives->count() > 0) {
        ui->tab_projects->setCurrentIndex(0);
    }
}

