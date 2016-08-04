/*
 *
 * File: sqlite.h
 * Description: this file contains headers for Sqlite DB
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef SQLITE_H
#define SQLITE_H


/*
 * Includes For SQLite
 *
 */
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QString>
#include <QStandardPaths>
#include <QDir>

#include <QDebug>

/*
 * ---- DEFINES FOR DB and TABLES ----
 * */

#define DB_EXTENSION ".sqlite"
#define DB_TRASH_EXTENSION "_trash.sqlite"

#define DB_CONNNAME "gloss"
#define DB_CONNNAME_TRASH "gloss_trash"

#define DB_ACTIVATE_FOREIGNKEYS "PRAGMA foreign_keys = ON"
#define CREATE_TB_LANGUAGE "CREATE TABLE IF NOT EXISTS language " \
                    "(id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, "\
                    "name VARCHAR(30) UNIQUE, "\
                    "isocode VARCHAR(10) UNIQUE)"

#define CREATE_TB_PROJECT "CREATE TABLE IF NOT EXISTS project " \
                    "(id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, "\
                    "name VARCHAR(30) UNIQUE NOT NULL, "\
                    "origlangID INTEGER NOT NULL, "\
                    "targlangID INTEGER NOT NULL, "\
                    "FOREIGN KEY(origlangID) REFERENCES language(id) ON DELETE CASCADE, "\
                    "FOREIGN KEY(targlangID) REFERENCES language(id) ON DELETE CASCADE)"

#define CREATE_TB_TYPE "CREATE TABLE IF NOT EXISTS type " \
                    "(id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, "\
                    "name VARCHAR(20) UNIQUE NOT NULL, "\
                    "shortname VARCHAR(7) UNIQUE NOT NULL, "\
                    "description VARCHAR(50) )"

#define CREATE_TB_TERM "CREATE TABLE IF NOT EXISTS term " \
                    "(id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, "\
                    "projectID INTEGER NOT NULL, "\
                    "typeID INTEGER NOT NULL, "\
                    "origterm VARCHAR(50) , "\
                    "targterm VARCHAR(50), "\
                    "context VARCHAR(250), "\
                    "FOREIGN KEY(projectID) REFERENCES project(id) ON DELETE CASCADE," \
                    "FOREIGN KEY(typeID) REFERENCES type(id))"

/*
 * Project table in Trash must have other fields, think about this:
 * You can move a project to Trash, but its related languages are not into Trash.
 * That's why we must record languages name and isocode too into Trash DB's
 * _project_ table.
 *
 * */
#define CREATE_TRASH_TB_PROJECT "CREATE TABLE IF NOT EXISTS project " \
                    "(id INTEGER UNIQUE PRIMARY KEY, "\
                    "name VARCHAR(30) UNIQUE, "\
                    "origlangID INTEGER NOT NULL, "\
                    "olangname VARCHAR(30), "\
                    "oisocode VARCHAR(10), "\
                    "targlangID INTEGER NOT NULL, "\
                    "tlangname VARCHAR(30), "\
                    "tisocode VARCHAR(10))"

#define CREATE_TRASH_TB_TERM "CREATE TABLE IF NOT EXISTS term " \
                    "(id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, "\
                    "projectID INTEGER NOT NULL, "\
                    "typeID INTEGER NOT NULL, "\
                    "origterm VARCHAR(50) , "\
                    "targterm VARCHAR(50), "\
                    "context VARCHAR(250), "\
                    "FOREIGN KEY(projectID) REFERENCES project(id) ON DELETE CASCADE)"


#define TB_CREATED 0
#define TB_EXISTS (TB_CREATED + 1)
#define TB_WITH_ERROR (TB_EXISTS + 1)


class Sqlite {
public:
    // ---- Constructors ----
    Sqlite();
    ~Sqlite();
    // ---- Methods ----
    void open(QString dbname, QString abspath);
    void activate_foreign_keys();
    unsigned int create_tables();
    unsigned int create_trash_tables();

    // ---- ---- Methods for MAIN DB (glossary) tables ---- ----
    // ---- Methods for _language_ table ----
    unsigned int ins_in_language(QString name, QString isocode);
    unsigned int ins_whole_in_language(QString langID, QString name, QString isocode);
    QStringList getAllLanguages();
    QStringList getAllTrashLanguages();

    // ---- Methods for _project_ table ----
    unsigned int ins_in_project(QString name, QString origlangID, QString targlangID);
    QStringList getProject(QString projID);
    QStringList getAllProjects();
    QStringList getAllProjects4List();
    QStringList getAllProjectsTrash4List();

    // ---- Methods for _type_ table ----
    QStringList getAllTypes();

    // ---- Methods for _term_ table ----
    unsigned int ins_in_term(QString projID, QString typeID,
                             QString origterm, QString targterm,
                             QString context);

    QStringList get_terms_by_proj(QString projID,
                                   unsigned int myLimit,
                                   unsigned int myOffset);

    QStringList searchTerm(QString projID, QString term);

    // ---- ---- Methods for TRASH DB tables ---- ----

    unsigned int tb_language_2_trash(QString langID);
    unsigned int tb_language_recover_from_trash (QString langID);
    unsigned int tb_language_remove_from_trash(QString langID);

    unsigned int tb_project_2_trash(QString langID);
    unsigned int tb_project_recover_from_trash(QString projID, QString olangID, QString tlangID);
    unsigned int tb_project_remove_from_trash(QString projID);

    unsigned int tb_term_2_trash(QString projID);
    unsigned int tb_term_recover_from_trash(QString projID);


private:
    unsigned int status;
    QSqlDatabase dbconn;
    QSqlDatabase dbtrash;
};

#endif // SQLITE_H
