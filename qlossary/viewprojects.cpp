/*
 *
 * File: viewprojects.cpp
 * Description: this file contains constructors and methods for ViewProjects dialog
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "viewprojects.h"
#include "ui_viewprojects.h"
#include <QDebug>

ViewProjects::ViewProjects(QWidget *parent) : QDialog(parent), ui(new Ui::ViewProjects) {
    ui->setupUi(this);
    connect(ui->lst_projects, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                                     this, SLOT(selected_project(QListWidgetItem*)) );
    connect(ui->lst_projects, SIGNAL(itemClicked(QListWidgetItem*)),
                                     this, SLOT(selected_project(QListWidgetItem*)) );
}

ViewProjects::~ViewProjects() {
    delete ui;
}

void ViewProjects::on_btn_close_clicked() {
    this->close();
}

void ViewProjects::on_btn_select_clicked() {
    this->accept();
}


void ViewProjects::setData(QStringList projs) {
#define COLUMNS 4

    unsigned int i, rows;

    /* projs list should be like:
     * [0] == project ID, [1] == project name, [3] = original language, [4] = target language
     * [5] == project ID, [6] == project name, [7] = original language, [8] = target language
     * (...)
     * */

    rows = projs.count();
    if (rows % COLUMNS != 0) {
        return;
    }
    projectsID.clear();
    for (i = 0; i < rows; i = i + COLUMNS) {
        qDebug() << projs.at(i+1) + "  ["+projs.at(i+2)+" => "+projs.at(i+3)+"]";
        projectsID.append( QString(projs.at(i)).toDouble() );
        ui->lst_projects->addItem(projs.at(i+1) + "  ["+projs.at(i+2)+" > "+projs.at(i+3)+"]");
    }
}

void ViewProjects::selected_project(QListWidgetItem *item) {
    unsigned int row; // To use as index!

    row = ui->lst_projects->currentRow();
    cur_project.first = projectsID[row];
    cur_project.second = item->text();
}

QPair<double, QString> ViewProjects::get_project() {
    return (cur_project);
}


