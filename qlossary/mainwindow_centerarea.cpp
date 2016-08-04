/*
 *
 * File: mainwindow_centerarea.cpp
 * Description: this file contains methods for Main Window's center area's widgets
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * COMBOBOX: Project
 * Description: Shows selected project's terms in central area's list
 *
 * */

void MainWindow::on_cmb_mainfrm_projectslst_currentIndexChanged(int index) {

    /*
     * Note: this hack is to avoid combobox->clear() generated a call to
     *      on_cmb_mainfrm_projectslst_currentIndexChanged() with an
     *      index of -1, and we must check this casuistic to avoid program
     *      crash (segmentation fault).
     * */
    if (index < 0) {
        return;
    }

    if (ui->ln_search->text() != "") {
        this->fill_mainproject_searchterm(projaux.at(index),
                                          ui->ln_search->text());
    } else {
        this->fill_mainproject_term_lst(projaux.at(index));
    }
}

void MainWindow::on_lst_terms_clicked(const QModelIndex &index) {

    QString body;
    unsigned int row;


    //row = ui->lst_terms->currentRow();
    row = index.row();
    body = "<h2>" + termsList.at(row).tterm +"</h2>";
    if (termsList.at(row).ctxt != "" ) {
        body += "<hr/><p>"+termsList.at(row).ctxt+"</p>";
    }

    body += "<hr/><p><i>Project</i>: <strong>"+termsList.at(row).projName+"</strong></p>";

    ui->txtdt_term_desc->setHtml(body);
}

/*
 * LineEdit: Search
 * Description: looks for a term in DB
 *
 * */
void MainWindow::on_ln_search_returnPressed() {

    if (ui->ln_search->text() != "") {
        this->fill_mainproject_searchterm(
                    projaux.at(ui->cmb_mainfrm_projectslst->currentIndex()),
                    ui->ln_search->text());
    } else {
        this->fill_mainproject_term_lst(projaux.at(ui->cmb_mainfrm_projectslst->currentIndex()));
    }


}
