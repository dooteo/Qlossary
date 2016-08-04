/*
 *
 * File: mainwindow.cpp
 * Description: this file contains constructors for Main Window
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"

// ---- ---- ---- Constructors ---- ---- ----

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    qDebug() << "Program started...\n";

    dbconn.open("glossary", NULL);

    this->fill_languages_widgets();
    this->fill_langTrash_widgets();
    this->fill_projects_widgets();
    this->fill_projTrash_widgets();
    this->fill_termTypes();

    qDebug() << "Filled term types" ;
    if (projaux.count() > 0) {
        this->fill_mainproject_term_lst(projaux.at(0));
    }
}

MainWindow::~MainWindow() {
    delete ui;
    qDebug() << "Program finished...\n";
}


/*
 * Note: check mainwindow_*.cpp files to find MainWindow's other methods.
 *
 * */
