/*
 *
 * File: newproject.cpp
 * Description: this file contains constructors and methods for NewProject dialog
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "newproject.h"
#include "ui_newproject.h"

extern Sqlite dbconn;

NewProject::NewProject(QWidget *parent) : QDialog(parent), ui(new Ui::NewProject) {

    ui->setupUi(this);
    this->setWindowTitle("New Project");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

NewProject::~NewProject() {
    delete ui;
}

void NewProject::on_btn_Cancel_clicked() {
    this->reject();
}
void NewProject::on_btn_addNewProject_clicked() {
    this->accept();
}

void NewProject::setData(QStringList langs) {
    int i;
    /* langs list is like:
     * [0] == id, [1] = language name, [2] = isocode
     * [3] == id, [4] = language name, [5] = isocode
     * (...)
     * */
    for (i = 0; i < langs.count(); i = i + 3) {
        langaux.append(langs.at(i)); // Language DB id
        ui->cmbOrigLang->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]"); // Lang name + isocode
        ui->cmbTargetLang->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]");
    }
}

QStringList NewProject::getData() {
    QStringList project;

    project.append(ui->lnProjectName->text());
    project.append(langaux.at(ui->cmbOrigLang->currentIndex())); // DB id for Original Language
    project.append(langaux.at(ui->cmbTargetLang->currentIndex())); // DB id for Target Language

    return (project);
}
