/*
 *
 * File: viewlanguages.cpp
 * Description: this file contains constructors and methods for ViewLanguages dialog
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */
#include "viewlanguages.h"
#include "ui_viewlanguages.h"

ViewLanguages::ViewLanguages(QWidget *parent) : QDialog(parent), ui(new Ui::ViewLanguages) {
    ui->setupUi(this);
}

ViewLanguages::~ViewLanguages() {
    delete ui;
}

void ViewLanguages::setData(QStringList langs) {
#define COLUMNS 3

    int i, rows;
    /* langs list is like:
     * [0] == id, [1] = language name, [2] = isocode
     * [3] == id, [4] = language name, [5] = isocode
     * (...)
     * */
    rows = langs.count();
    if (rows % COLUMNS != 0) {
        return;
    }

    for (i = 0; i < rows; i = i + COLUMNS) {
        ui->lst_languages->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]");
    }

}

void ViewLanguages::on_btn_close_clicked() {
    this->close();
}
