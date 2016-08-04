/*
 *
 * File: mainwindow_dck_language.cpp
 * Description: this file contains methods for Main Window's Language dock
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * TAB: New
 * BUTTON: ADD
 * Description: Adds new language
 *
 * */
void MainWindow::on_btn_langAdd_clicked() {

    if ( (ui->ln_langName->text() == "") || (ui->ln_langIsocode->text() == "") ){
        return;
    }

    dbconn.ins_in_language(ui->ln_langName->text(), ui->ln_langIsocode->text());

    this->fill_languages_widgets();
    this->fill_langTrash_widgets();
    ui->statusBar->showMessage("Language added: "+ ui->ln_langName->text() +
                               " ["+ ui->ln_langIsocode->text()+"]", 5000);
    // ---- Clear input lines ----
    ui->ln_langName->clear();
    ui->ln_langIsocode->clear();
    ui->tab_languages->setCurrentIndex(0);
}
/*
 * TAB: Actives
 * BUTTON: Remove
 * Description: Moves selected language to Trash
 *
 * */
void MainWindow::on_btn_langRemove_actvs_clicked() {
    QString language;
    QList <QListWidgetItem*> items;
    unsigned int row;

    items = ui->lst_langActives->selectedItems();
    if (items.count() < 1) { // At least 1 item must be selected
        return;
    }

    row = ui->lst_langActives->currentRow();
    language = ui->lst_langActives->currentItem()->text();

    dbconn.tb_language_2_trash(langaux.at(row));
    this->fill_languages_widgets();
    this->fill_langTrash_widgets();
    this->fill_projects_widgets();
    this->fill_projTrash_widgets();

    ui->statusBar->showMessage("Language to trash: "+ language, 5000);
    if (ui->lst_langActives->count() > 0) {
        ui->tab_languages->setCurrentIndex(0);
    }
}
/*
 * TAB: Trash
 * BUTTON: Activate
 * Description: Recovers selected language from Trash to include it into Activate langs
 *
 * */
void MainWindow::on_btn_langActivate_trash_clicked() {
    QString language;
    unsigned int row;

    if (ui->lst_langTrash->selectedItems().count() < 1) { // At least 1 item must be selected
        return;
    }

    row = ui->lst_langTrash->currentRow();
    language = ui->lst_langTrash->currentItem()->text();

    dbconn.tb_language_recover_from_trash(langTrashAux.at(row));

    this->fill_languages_widgets();
    this->fill_langTrash_widgets();

    ui->statusBar->showMessage("Language recovered from trash: "+ language, 5000);
    ui->tab_languages->setCurrentIndex(0);
}
/*
 * TAB: Trash
 * BUTTON: REMOVE
 * Description: Remove selected language from Trash
 *
 * */
void MainWindow::on_btn_langRemove_trash_clicked() {
    QString language;
    QList <QListWidgetItem*> items;
    unsigned int row;

    items = ui->lst_langTrash->selectedItems();
    if (items.count() < 1) { // At least 1 item must be selected
        return;
    }

    row = ui->lst_langTrash->currentRow();

    language = ui->lst_langTrash->currentItem()->text();
    dbconn.tb_language_remove_from_trash(langTrashAux.at(row));

    this->fill_languages_widgets();
    this->fill_langTrash_widgets();
    ui->statusBar->showMessage("Language removed from trash: "+ language, 5000);

    if (ui->lst_langActives->count() > 0) {
        ui->tab_languages->setCurrentIndex(0);
    }
}
