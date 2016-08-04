/*
 *
 * File: newlanguage.cpp
 * Description: this file contains constructors and methods for NewLanguage dialog
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "newlanguage.h"
#include "ui_newlanguage.h"

NewLanguage::NewLanguage(QWidget *parent) : QDialog(parent), ui(new Ui::NewLanguage) {

    ui->setupUi(this);
}

NewLanguage::~NewLanguage() {
    delete ui;
}

void NewLanguage::on_btn_cancel_clicked() {
    this->reject();
}

void NewLanguage::on_btn_save_clicked() {
    this->accept();
}

QStringList NewLanguage::getData() {
    QStringList language;

    language.append(ui->langName->text()); // add at [0] position
    language.append(ui->isocode->text()); // add at [1] position

    return (language);
}
