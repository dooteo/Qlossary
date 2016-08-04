/*
 *
 * File: sqlite_tb_project.cpp
 * Description: this file contains methods to handle "project" table
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "sqlite.h"

/*
 * Inserts a new project
 *
 * */
unsigned int Sqlite::ins_in_project(QString name, QString origlangID, QString targlangID) {

    QSqlQuery sqlquery(dbconn);
    status = 0;

    sqlquery.prepare("INSERT INTO project (name, origlangID, targlangID) "
                        "VALUES (?, ?, ?);");
    sqlquery.addBindValue(name);
    sqlquery.addBindValue(origlangID);
    sqlquery.addBindValue(targlangID);

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    qDebug() << "Insert row into project: ok.";

    return (status);
}



/*
 * Get's a project whole data
 *
 * */
QStringList Sqlite::getProject(QString projID) {

    QStringList resultSet;
    QSqlQuery sqlquery(dbconn);

    sqlquery.prepare(" SELECT t1.id, t1.name, t2.id AS olangID, t2.name AS olang, t2.isocode AS oisocode, "
                     " t3.id AS tlangID, t3.name AS tlang, t3.isocode AS tisocode "
                     " FROM project AS t1, language AS t2, language AS t3 "
                     " WHERE (t1.id = ?) AND (t2.id = t1.origlangID) AND (t3.id = t1.targlangID)");

    sqlquery.bindValue(0, projID);
    if (! sqlquery.exec() ){
        qDebug() << "[Sqlite::getProject]";
        qDebug() << sqlquery.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqlquery.next()) {
        resultSet.append(sqlquery.value(0).toString()); // project ID
        resultSet.append(sqlquery.value(1).toString()); // project name
        resultSet.append(sqlquery.value(2).toString()); // original lang ID
        resultSet.append(sqlquery.value(3).toString()); // original language name
        resultSet.append(sqlquery.value(4).toString()); // original isocode
        resultSet.append(sqlquery.value(5).toString()); // target language ID
        resultSet.append(sqlquery.value(6).toString()); // target language name
        resultSet.append(sqlquery.value(7).toString()); // target isocode
    }

    return (resultSet);
}

/*
 * Get's all rows from _project_ table
 *
 * */
QStringList Sqlite::getAllProjects() {
    QStringList resultSet;
    QSqlQuery sqlquery(dbconn);

    sqlquery.prepare("SELECT id, name, origlangID, tarlangID FROM project ORDER BY name ASC;");
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqlquery.next()) {
        resultSet.append(sqlquery.value(0).toString()); // ID
        resultSet.append(sqlquery.value(1).toString()); // name
        resultSet.append(sqlquery.value(2).toString()); // origlangID
        resultSet.append(sqlquery.value(3).toString()); // targetlangID
    }

    return (resultSet);
}


/*
 * Get's all projects whole data
 *
 * */
QStringList Sqlite::getAllProjects4List() {

    QStringList resultSet;
    QSqlQuery sqlquery(dbconn);

    sqlquery.prepare("SELECT t1.id, t1.name, t2.name, t2.isocode, t3.name, t3.isocode "
                     "FROM project AS t1, language AS t2, language AS t3 "
                     "WHERE (t2.id = t1.origlangID) AND (t3.id = t1.targlangID) "
                     "ORDER BY t1.name ASC;");
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqlquery.next()) {
        resultSet.append(sqlquery.value(0).toString()); // project ID
        resultSet.append(sqlquery.value(1).toString()); // project name
        resultSet.append(sqlquery.value(2).toString()); // original language name
        resultSet.append(sqlquery.value(3).toString()); // original isocode
        resultSet.append(sqlquery.value(4).toString()); // target language name
        resultSet.append(sqlquery.value(5).toString()); // target isocode
    }

    return (resultSet);
}


/*
 * Get's all projects data from Trash DB
 *
 * */
QStringList Sqlite::getAllProjectsTrash4List() {

    QStringList resultSet;
    QSqlQuery sqltrash(dbtrash);

    sqltrash.prepare("SELECT t1.id, t1.name, t1.origlangID, t1.olangname, t1.oisocode, "
                     "t1.targlangID, t1.tlangname, t1.tisocode "
                     "FROM project AS t1 "
                     "ORDER BY t1.name ASC;");
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqltrash.next()) {
        resultSet.append(sqltrash.value(0).toString()); // project ID
        resultSet.append(sqltrash.value(1).toString()); // project name
        resultSet.append(sqltrash.value(2).toString()); // original language ID
        resultSet.append(sqltrash.value(3).toString()); // original language name
        resultSet.append(sqltrash.value(4).toString()); // original lang isocode
        resultSet.append(sqltrash.value(5).toString()); // target language ID
        resultSet.append(sqltrash.value(6).toString()); // target language name
        resultSet.append(sqltrash.value(7).toString()); // target language isocode
    }

    return (resultSet);
}


/*
 * Moves a project to Trash
 *
 * */
unsigned int Sqlite::tb_project_2_trash(QString projID){
    QSqlQuery sqlquery(dbconn);
    QSqlQuery sqltrash(dbtrash);
    QStringList resultSet;

    status = 0;

    resultSet = this->getProject(projID);

    if ( (resultSet.count() % 8) != 0) {
        return(1);
    }

    /*
    *  Note: You can also run whole string type query if you want to,
    *       in case you do no want to use QSqlQuery::prepare() for whatever reason You've got!
    *       Don't forget to use quotes (') for inner strings!
    *
    *     sqltrash.prepare("INSERT INTO project (id, name, origlangID, olangname, oisocode, "
    *               "targlangID, tlangname, tisocode ) "
    *               "VALUES ("+projID+",'"+resultSet.at(1)+"', "+resultSet.at(2)+
    *               ", '"+resultSet.at(3)+"', '"+resultSet.at(4)+"', "+resultSet.at(5)+
    *               ", '"+resultSet.at(6)+"', '"+resultSet.at(7)+"')");
    **/

    sqltrash.prepare("INSERT OR IGNORE INTO project (id, name, origlangID, olangname, oisocode, "
                    "targlangID, tlangname, tisocode ) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    sqltrash.bindValue(0, projID.toUInt()); // ID
    sqltrash.bindValue(1, resultSet.at(1)); // project name
    sqltrash.bindValue(2, resultSet.at(2)); // original language ID
    sqltrash.bindValue(3, resultSet.at(3)); // original language name
    sqltrash.bindValue(4, resultSet.at(4)); // original lang isocode
    sqltrash.bindValue(5, resultSet.at(5)); // target language ID
    sqltrash.bindValue(6, resultSet.at(6)); // target language name
    sqltrash.bindValue(7, resultSet.at(7)); // target language isocode

    /*
     * Note: if You want to see which Query was sent to Sqlite engine,
     *      use next method: executedQuery();
     *      It's very helpful to debug SQL queries!
     *
     *  qDebug() << sqltrash.executedQuery();
     *
     * */

    if (! sqltrash.exec() ){
        qDebug() << "[Sqlite::tb_project_2_trash]";
        qDebug() << sqltrash.lastError();
        return (1);
    }

    status = this->tb_term_2_trash(projID);
    if (status != 0) {
        return (status);
    }

    sqlquery.prepare("DELETE FROM project WHERE (id = ?)");
    sqlquery.bindValue(0, projID.toUInt()); // ID
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    qDebug() << "a Project moved to Trash";
    return (status);
}

unsigned int Sqlite::tb_project_recover_from_trash(QString projID, QString olangID, QString tlangID){
    QSqlQuery sqlquery(dbconn);
    QSqlQuery sqltrash(dbtrash);
    status = 0;

    sqltrash.prepare("SELECT * FROM project WHERE (id = ?)");

    sqltrash.bindValue(0, projID.toUInt());
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }

    /* Note: Seems Sqlite3 does not support QSqlQuery::size() method,
     *      it returns -1 value in tests I made.
     * */
    sqltrash.next();
    /*
     * [0] = ID, [1] = proj name, [2] = olangID, [3] = olang name, [4] = olang isocode,
     *                      [5] = tlangID, [6] = tlang name, [7] = tlang isocode
     * */


    // Check original language is registered into Glossary DB
    sqlquery.prepare("SELECT id FROM language WHERE (id = ?)");
    sqlquery.bindValue(0, olangID);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    if (sqlquery.size() < 1) { // There is not such language in Glossary DB
        this->ins_whole_in_language(sqltrash.value(2).toString(),
                                    sqltrash.value(3).toString(),
                                    sqltrash.value(4).toString());
        this->tb_language_remove_from_trash(sqltrash.value(2).toString());
    }

    // Check target language is registered into Glossary DB
    sqlquery.prepare("SELECT id FROM language WHERE (id = ?)");
    sqlquery.bindValue(0, tlangID);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    if (sqlquery.size() < 1) { // There is not such language in Glossary DB
        this->ins_whole_in_language(sqltrash.value(5).toString(),
                                    sqltrash.value(6).toString(),
                                    sqltrash.value(7).toString());
        this->tb_language_remove_from_trash(sqltrash.value(5).toString());
    }

    // So once we've got orig and target languages in DB, we can insert project...

    sqlquery.prepare("INSERT OR IGNORE INTO project (id, name, origlangID, targlangID) "\
                     "VALUES(?, ?, ?, ?)");

    sqlquery.bindValue(0, sqltrash.value(0).toInt()); // ID
    sqlquery.bindValue(1, sqltrash.value(1).toString()); // project name
    sqlquery.bindValue(2, sqltrash.value(2).toString()); // original language ID
    sqlquery.bindValue(3, sqltrash.value(5).toString()); // target language ID

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    this->tb_term_recover_from_trash(projID);

    sqltrash.prepare("DELETE FROM project WHERE (id = ?)");
    sqltrash.bindValue(0, projID.toUInt());
    if ( ! sqltrash.exec()) {
        qDebug() << sqltrash.lastError();
        return(1);
    }
    /*status = this->tb_term_2_trash(sqltrash.value(0).toString());
    if (status != 0) {
        return (status);
    }
*/

    return (0);
}

unsigned int Sqlite::tb_project_remove_from_trash(QString projID) {
    QSqlQuery sqltrash(dbtrash);

    sqltrash.prepare("DELETE FROM project WHERE (id = ?)");
    sqltrash.bindValue(0, projID); // ID
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }

    return (0);
}
