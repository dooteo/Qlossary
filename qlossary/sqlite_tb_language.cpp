/*
 *
 * File: sqlite_tb_language.cpp
 * Description: this file contains methods to handle _language_ table
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "sqlite.h"

/*
 * Inserts a new row into Language table
 * */
unsigned int Sqlite::ins_in_language(QString name, QString isocode) {

    QSqlQuery sqlquery(dbconn);
    status = 0;

    sqlquery.prepare("INSERT OR IGNORE INTO language (name, isocode) "
                        "VALUES (?, ?);");
    sqlquery.addBindValue(name);
    sqlquery.addBindValue(isocode);

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    qDebug() << "Insert row into language: ok.";

    return (status);
}
unsigned int Sqlite::ins_whole_in_language(QString langID, QString name, QString isocode) {

    QSqlQuery sqlquery(dbconn);
    status = 0;

    sqlquery.prepare("INSERT OR IGNORE INTO language (id, name, isocode) "
                        "VALUES (?, ?, ?);");
    sqlquery.addBindValue(langID);
    sqlquery.addBindValue(name);
    sqlquery.addBindValue(isocode);

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    qDebug() << "Insert row into language: ok.";

    return (status);
}


/*
 * Get all languages data from main glossary DB
 *
 * */
QStringList Sqlite::getAllLanguages() {
    QStringList resultSet;
    QSqlQuery sqlquery(dbconn);

    sqlquery.prepare("SELECT id, name, isocode FROM language ORDER BY name ASC;");
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqlquery.next()) {
        resultSet.append(sqlquery.value(0).toString()); // ID
        resultSet.append(sqlquery.value(1).toString()); // name
        resultSet.append(sqlquery.value(2).toString()); // isocode
    }

    return (resultSet);
}
/*
 * Get all languages data from Trash DB
 *
 * */
QStringList Sqlite::getAllTrashLanguages() {
    QStringList resultSet;
    QSqlQuery sqltrash(dbtrash);

    sqltrash.prepare("SELECT id, name, isocode FROM language ORDER BY name ASC;");
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqltrash.next()) {
        resultSet.append(sqltrash.value(0).toString()); // ID
        resultSet.append(sqltrash.value(1).toString()); // name
        resultSet.append(sqltrash.value(2).toString()); // isocode
    }

    return (resultSet);
}

/*
 * Moves a language (a row) to Trash DB
 *
 * */
unsigned int Sqlite::tb_language_2_trash(QString langID) {
    QSqlQuery sqlquery(dbconn);
    QSqlQuery sqltrash(dbtrash);
    QStringList aux;


    sqlquery.prepare("SELECT * FROM language WHERE (id = ?)");
    sqlquery.bindValue(0, langID.toUInt());
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    sqltrash.prepare("INSERT INTO language VALUES(?, ?, ?)");

    sqlquery.next(); // Gert first row from dbconn
    sqltrash.bindValue(0, sqlquery.value(0).toInt()); // ID
    sqltrash.bindValue(1, sqlquery.value(1).toString()); // language name
    sqltrash.bindValue(2, sqlquery.value(2).toString()); // language isocode
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }

    sqlquery.prepare("SELECT id FROM project WHERE (origlangID = ?) OR (targlangID = ?)");
    sqlquery.bindValue(0, langID.toUInt());
    sqlquery.bindValue(1, langID.toUInt());
    if (! sqlquery.exec()) {
        qDebug() << sqlquery.lastError();
        return (1);
    }

    while (sqlquery.next()) {
        qDebug() << "Proj ID: "<< sqlquery.value(0);
        status = this->tb_project_2_trash(sqlquery.value(0).toString());
        if (status > 0) {
            qDebug() << "Something wrong at project to trash";
            return (status);
        }
    }

    sqlquery.prepare("DELETE FROM language WHERE (id = ?)");
    sqlquery.bindValue(0, langID);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }
    qDebug() << "A language moved to Trash";
    return (status);
}

/*
 * Moves a Language (a row) from Trash to Glossary DB
 *
 * */

unsigned int Sqlite::tb_language_recover_from_trash (QString langID) {
    QSqlQuery sqlquery(dbconn);
    QSqlQuery sqltrash(dbtrash);
    QStringList aux;

    sqltrash.prepare("SELECT * FROM language WHERE (id = ?)");
    sqltrash.bindValue(0, langID);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }

    sqlquery.prepare("INSERT INTO language VALUES(?, ?, ?)");

    sqltrash.next(); // Gert first row from dbconn
    sqlquery.bindValue(0, sqltrash.value(0).toInt()); // ID
    sqlquery.bindValue(1, sqltrash.value(1).toString()); // language name
    sqlquery.bindValue(2, sqltrash.value(2).toString()); // language isocode
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    sqltrash.prepare("DELETE FROM language WHERE (id = ?)");
    sqltrash.bindValue(0, langID);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }
    return (status);
}

/*
 * Removes a Language (a row) from Trash DB
 *
 * */

unsigned int Sqlite::tb_language_remove_from_trash(QString langID) {
    QSqlQuery sqltrash(dbtrash);

    sqltrash.prepare("DELETE FROM language WHERE (id = ?)");
    sqltrash.bindValue(0, langID); // ID
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }

    return (0);
}
