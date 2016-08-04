/*
 *
 * File: mainwindow.h
 * Description: this file contains headers for program's Main Window.
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include <QDir>
#include <QStringList>
#include <QPair>

#include <QDebug>

/*
 * Includes For SQLite
 *
 */
/*
 * #include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
*/

#include "global_defines.h"
#include "aboutqlossary.h"
#include "newlanguage.h"
#include "newproject.h"
#include "sqlite.h"
#include "viewlanguages.h"
#include "viewprojects.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void add_new_projct();
    Sqlite dbconn;

private slots:
    // ---- ---- Methods for MainWindow Menus ---- ----
    void on_mn_Help_About_Qlossary_triggered();

    void on_mnProject_New_triggered();

    void on_mnProject_NewLanguage_triggered();

    void on_mnProject_ViewLanguages_triggered();

    void on_mnProject_ViewProjects_triggered();

    void on_mnView_Languages_triggered();

    void on_mnView_Projects_triggered();

    // ---- ---- Methods for DOCK_WIDGET == LANGUAGE ---- ----
    void on_btn_langAdd_clicked();
    void on_btn_langRemove_actvs_clicked();
    void on_btn_langActivate_trash_clicked();
    void on_btn_langRemove_trash_clicked();

    // ---- ---- Methods for DOCK_WIDGET == PROJECT ---- ----
    void on_btn_projAdd_clicked();
    void on_btn_projRemove_clicked();
    void on_btn_projActivate_trash_clicked();
    void on_btn_projRemove_trash_clicked();


    // ---- ---- Methods for DOCK_WIDGET == NEW TERM ---- ----
    void on_mnView_NewTerm_triggered();
    void on_btn_clearTerm_clicked();
    void on_btn_addTerm_clicked();


    void on_cmb_mainfrm_projectslst_currentIndexChanged(int index);
    void on_ln_search_returnPressed();
    void on_lst_terms_clicked(const QModelIndex &index);


   // void on_btn_projAdd_pressed();

private:
    /*
     * Note: use a struct to store Project IDs from Trash
     * */
    struct ProjtrashStruct {
        QString id;
        QString olangID;
        QString tlangID;
    };
    QList <ProjtrashStruct> projTrashList;

    struct BasicTerm {
      QString id;
      QString oterm;
      QString tterm;
      QString ctxt;
      QString projName;
      QString typeName;
      QString typeShortname;
    };
    QList <BasicTerm> termsList;

    Ui::MainWindow *ui;
    QString mainproject;
    QStringList langaux;
    QStringList langTrashAux;
    QStringList projaux;

    QStringList typeIDs;

    void fill_languages_widgets();
    void fill_langTrash_widgets();
    void move_language_2_trash(QString langID);

    void fill_projects_widgets();
    void fill_projTrash_widgets();

    void fill_termTypes();
    void fill_mainproject_term_lst(QString projID);
    void fill_mainproject_searchterm(QString projID, QString term);
};

#endif // MAINWINDOW_H
