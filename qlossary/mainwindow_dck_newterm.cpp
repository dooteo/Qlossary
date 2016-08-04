/*
 *
 * File: mainwindow_dck_newterm.cpp
 * Description: this file contains methods for Main Window's 'New Term' dock
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_btn_clearTerm_clicked(){
    ui->ln_new_origterm->clear();
    ui->ln_new_targterm->clear();
    ui->txt_new_termContext->clear();

    ui->cmb_new_termType->setCurrentIndex(0);
}



void MainWindow::on_btn_addTerm_clicked() {

    if ( ui->ln_new_origterm->text().trimmed().isEmpty()) {
        return;
    }
    if (ui->ln_new_targterm->text().trimmed().isEmpty()) {
        return;
    }
    dbconn.ins_in_term(projaux.at(ui->cmb_newtermprojects->currentIndex()),
                       typeIDs.at(ui->cmb_new_termType->currentIndex()),
                       ui->ln_new_origterm->text().trimmed(),
                       ui->ln_new_targterm->text().trimmed(),
                       ui->txt_new_termContext->toPlainText());

    qDebug() << "New Term added: " << ui->ln_new_origterm->text();

    this->on_btn_clearTerm_clicked();
    ui->cmb_mainfrm_projectslst->setCurrentIndex(ui->cmb_newtermprojects->currentIndex());


    this->fill_mainproject_term_lst(projaux.at(ui->cmb_newtermprojects->currentIndex()));
}
