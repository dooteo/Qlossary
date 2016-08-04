/*
 *
 * File: mainwindow_private_methods.cpp
 * Description: this file contains private methods for Main Window
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::fill_languages_widgets() {
    QStringList langs;
    int i, rows;

    langs = dbconn.getAllLanguages();

    langaux.clear();
    ui->cmb_projOrigLang->clear();
    ui->cmb_projTargLang->clear();
    ui->lst_langActives->clear();

    rows = langs.count();
    if (rows > 0) {
        ui->tab_languages->setTabEnabled(0, true);
    } else {
        ui->tab_languages->setTabEnabled(0, false);
    }
    /* langs list is like:
     * [0] == id, [1] = language name, [2] = isocode
     * [3] == id, [4] = language name, [5] = isocode
     * (...)
     * */
    for (i = 0; i < rows; i = i + 3) {
        langaux.append(langs.at(i)); // Language DB id
        ui->cmb_projOrigLang->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]"); // Lang name + isocode
        ui->cmb_projTargLang->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]");

        ui->lst_langActives->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]");
    }

}

void MainWindow::fill_langTrash_widgets() {
    QStringList langs;
    int i, rows;

    langs = dbconn.getAllTrashLanguages();

    langTrashAux.clear();
    ui->lst_langTrash->clear();


    /* langs list is like:
     * [0] == id, [1] = language name, [2] = isocode
     * [3] == id, [4] = language name, [5] = isocode
     * (...)
     * */
    rows = langs.count();
    // Trash has languages
    if (rows > 0) {
        ui->tab_languages->setTabEnabled(1, true);

    // Trash has no languages
    } else {
        ui->tab_languages->setTabEnabled(1, false);
        return;
    }

    for (i = 0; i < rows; i = i + 3) {
        langTrashAux.append(langs.at(i)); // Language Trash DB id
        ui->lst_langTrash->addItem(langs.at(i + 1) + " ["+langs.at(i + 2)+"]");
    }
}


void MainWindow::fill_projects_widgets() {
    QStringList projs;
    int i, rows;

    projs = dbconn.getAllProjects4List();

    projaux.clear();
    ui->lst_projActives->clear();
    ui->cmb_newtermprojects->clear();

    /*
     * Note: a combobox->clear() method emits on_subject_comboBox_currentIndexChanged()" signal
     *   with an index value of -1.
     * */
    ui->cmb_mainfrm_projectslst->clear();


    rows = projs.count();
    if (rows > 0) {
        ui->tab_projects->setTabEnabled(0, true);
    } else {
        ui->tab_projects->setTabEnabled(0, false);
    }

    /* projects list is like:
     * [0] == id, [1] = proj name, [2] = original lang name, [3] = orig isocode
     *                             [4] = target lang name [5] = target isocode
     * [6] == id, [7] = proj name, [8] = original lang name, [9] = orig isocode
     *                             [10] = target lang name, [11] = target isocode
     * (...)
     * */

    /* Note: we could not use next code to add _TOOLTIP_ to items, it doesn't work!
     *
     * for (i = 0; i < rows; i = i + 6) {
     *   projaux.append(projs.at(i));
     *   ui->lst_projActives->addItem(projs.at(i + 1));
     *   ui->lst_projActives->item(i)->setToolTip("Hi there just a tip!");
     * }
     *
     */

    for (i = 0; i < rows; i = i + 6) {

        projaux.append(projs.at(i));
        ui->cmb_newtermprojects->addItem(projs.at(i + 1));
        ui->cmb_mainfrm_projectslst->addItem(projs.at(i + 1));

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(projs.at(i + 1));
        item->setToolTip(projs.at(i + 1) + "\n  "+projs.at(i + 2)+" ("+projs.at(i + 3)+")\n  "+
                         projs.at(i + 4)+" ("+projs.at(i + 5)+")");
        ui->lst_projActives->addItem(item);

    }
}

void MainWindow::fill_projTrash_widgets() {
    QStringList projs;
    int i, rows;

    projs = dbconn.getAllProjectsTrash4List();

    projTrashList.clear();
    ui->lst_projTrash->clear();

    rows = projs.count();
    // Trash has projects
    if (rows > 0) {
        ui->tab_projects->setTabEnabled(1, true);

    // Trash has no projects
    } else {
        ui->tab_projects->setTabEnabled(1, false);
        return;
    }

    if (rows % 8 != 0) {
        return;
    }

    /* projects list is like:
     * [0] == id, [1] = proj name, [2] = original lang ID, [3] = orig lang name, [4] = orig isocode
     *            [5] = target lang ID [6] = target lang name [7] = target isocode
     * [8] == id, [9] = proj name, [10] = original lang ID, [11] = orig lang name, [12] = orig isocode
     *            [13] = target lang ID [14] = target lang name [15] = target isocode
     * (...)
     * */

    for (i = 0; i < rows; i = i + 8) {

        // Let's got to use ProjtrashStruct struct and a list to store them
        ProjtrashStruct projItem;
        projItem.id = projs.at(i);
        projItem.olangID = projs.at(i + 2);
        projItem.tlangID = projs.at(i + 5);

        projTrashList.append(projItem);

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(projs.at(i + 1));
        item->setToolTip(projs.at(i + 1) + "\n  "+projs.at(i + 3)+" ("+projs.at(i + 4)+")\n  "+
                         projs.at(i + 6)+" ("+projs.at(i + 7)+")");

        ui->lst_projTrash->addItem(item);

    }
}

void MainWindow::fill_termTypes() {
    QStringList types;
    int i, rows;

    types = dbconn.getAllTypes();

    typeIDs.clear();
    ui->cmb_new_termType->clear();

    rows = types.count();

    /* type list is like:
     * [0] == id, [1] = name, [2] = short name, [3] = description
     * [4] == id, [5] = name, [6] = short name, [7] = description
     * (...)
     * */

    for (i = 0; i < rows; i = i + 4) {
        typeIDs.append(types.at(i)); // Types ID
        ui->cmb_new_termType->addItem(types.at(i + 1));
    }

}

void MainWindow::fill_mainproject_term_lst(QString projID) {
    QStringList terms;
    int i, rows;

    termsList.clear();
    ui->lst_terms->clear();
    ui->txtdt_term_desc->clear();

    /*
     * Note: projID, limit, offset
     *     If limit = 0, no limit
     *
     * */
    terms = dbconn.get_terms_by_proj(projID, 0, 0);
    rows = terms.count();

    /* term row is as:
     * [0] = ID, [1] = oterm, [2] = tterm, [3] = ctxt,
     *                        [4] = typename, [5] = type shortname, [6] = projname,
     * [7] = ID, [8] = oterm, [8] = tterm, [9] = ctxt,
     *                       [10] = typename, [11] = type shortname, [12] = projname,
     *
     * (...)
     * */
    for (i = 0; i < rows; i = i + 7) {
        BasicTerm termItem;
        termItem.id = terms.at(i);
        termItem.oterm = terms.at(i + 1);
        termItem.tterm = terms.at(i + 2);
        termItem.ctxt = terms.at(i + 3);
        termItem.typeName = terms.at(i + 4);
        termItem.typeShortname = terms.at(i + 5);
        termItem.projName = terms.at(i + 6);

        termsList.append(termItem);
        ui->lst_terms->addItem(terms.at(i + 1));
    }
}

void MainWindow::fill_mainproject_searchterm(QString projID, QString term) {
    QStringList terms;
    QString body;
    int i, rows;

    termsList.clear();
    ui->lst_terms->clear();
    ui->txtdt_term_desc->clear();
    body.clear();

    terms = dbconn.searchTerm(projID, term);
    rows = terms.count();

    if (rows == 0) {
        return;
    }

    /* term row is as:
     * [0] = ID, [1] = oterm, [2] = tterm, [3] = ctxt,
     *                        [4] = typename, [5] = type shortname, [6] = projname,
     * [7] = ID, [8] = oterm, [8] = tterm, [9] = ctxt,
     *                       [10] = typename, [11] = type shortname, [12] = projname,
     *
     * (...)
     * */


    for (i = 0; i < rows; i = i + 7) {
        BasicTerm termItem;
        termItem.id = terms.at(i);
        termItem.oterm = terms.at(i + 1);
        termItem.tterm = terms.at(i + 2);
        termItem.ctxt = terms.at(i + 3);
        termItem.typeName = terms.at(i + 4);
        termItem.typeShortname = terms.at(i + 5);
        termItem.projName = terms.at(i + 6);

        termsList.append(termItem);
        ui->lst_terms->addItem(terms.at(i + 1));

    }

    ui->lst_terms->setCurrentRow(0);

    body = "<h2>" + termsList.at(0).tterm +"</h2>";
    if (termsList.at(0).ctxt != "" ) {
        body += "<hr/><p>"+termsList.at(0).ctxt+"</p>";
    }

    body += "<hr/><p><i>Project</i>: <strong>"+termsList.at(0).projName+"</strong></p>";
    ui->txtdt_term_desc->setHtml(body);

}
