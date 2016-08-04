/*
 *
 * File: aboutqlossary.cpp
 * Description: this file contains constructors and methods for AboutQlossary dialog
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "aboutqlossary.h"
#include "ui_aboutqlossary.h"

AboutQlossary::AboutQlossary(QWidget *parent) : QDialog(parent), ui(new Ui::AboutQlossary) {
    ui->setupUi(this);
    this->setWindowTitle("About Qlossary");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

AboutQlossary::~AboutQlossary() {
    delete ui;
}

void AboutQlossary::on_btn_close_about_dlg_clicked() {
    this->close();
}
