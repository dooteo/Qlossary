/*
 *
 * File: sqlite_tb_term.cpp
 * Description: this file contains methods to handle "term" table
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "sqlite.h"

unsigned int Sqlite::ins_in_term(QString projID, QString typeID,
                                 QString origterm, QString targterm,
                                 QString context) {


    QSqlQuery sqlquery(dbconn);
    status = 0;



    sqlquery.prepare("INSERT INTO term (projectID, typeID, origterm, targterm, context) "
                     "VALUES (?, ?, ?, ?, ?)");
    /*
     * Note: other way to bind values is
     */
    sqlquery.bindValue(0, projID.toUInt());
    sqlquery.bindValue(1, typeID.toUInt());
    sqlquery.bindValue(2, origterm);
    sqlquery.bindValue(3, targterm);
    sqlquery.bindValue(4, context);

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    return (status);
}

unsigned int Sqlite::tb_term_2_trash(QString projID){
    QSqlQuery sqlquery(dbconn);
    QSqlQuery sqltrash(dbtrash);


    sqlquery.prepare("SELECT * FROM term WHERE (projectID = ?)");
    sqlquery.bindValue(0, projID.toUInt());
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    while (sqlquery.next()) {

        sqltrash.prepare("INSERT INTO term (id, projectID, typeID, origterm, targterm, context) "\
                         "VALUES(?, ?, ?, ?, ?, ?)");

        sqltrash.bindValue(0, sqlquery.value(0).toInt()); // ID
        sqltrash.bindValue(1, sqlquery.value(1).toString()); // project ID
        sqltrash.bindValue(2, sqlquery.value(2).toString()); // type ID
        sqltrash.bindValue(3, sqlquery.value(3).toString()); // original term
        sqltrash.bindValue(4, sqlquery.value(4).toString()); // target term

        if (sqlquery.value(5).toString().trimmed().isEmpty()) {
            sqltrash.bindValue(5, ""); // context
        } else {
            sqltrash.bindValue(5, sqlquery.value(5).toString()); // context
        }

        if (! sqltrash.exec() ){
            qDebug() << sqltrash.lastError();
            return (1);
        }
    }

    sqlquery.prepare("DELETE FROM term WHERE (projectID = ?)");
    sqlquery.bindValue(0, projID.toUInt());
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (1);
    }

    return (0);
}

unsigned int Sqlite::tb_term_recover_from_trash(QString projID){
    QSqlQuery sqlquery(dbconn);
    QSqlQuery sqltrash(dbtrash);

    sqltrash.prepare("SELECT * FROM term WHERE (projectID = ?)");
    sqltrash.bindValue(0, projID);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }

    while (sqltrash.next()) {
        sqlquery.prepare("INSERT INTO term (id, projectID, typeID, origterm, targterm, context) "\
                         "VALUES(?, ?, ?, ?, ?, ?)");

        sqlquery.bindValue(0, sqltrash.value(0).toInt()); // ID
        sqlquery.bindValue(1, sqltrash.value(1).toString()); // project ID
        sqlquery.bindValue(2, sqltrash.value(2).toString()); // type ID
        sqlquery.bindValue(3, sqltrash.value(3).toString()); // original term
        sqlquery.bindValue(4, sqltrash.value(4).toString()); // target term
        sqlquery.bindValue(5, sqltrash.value(5).toString()); // context

        if (! sqlquery.exec() ){
            qDebug() << sqlquery.lastError();
            return (1);
        }
    }

    sqltrash.prepare("DELETE FROM term WHERE (projectID = ?)");
    sqltrash.bindValue(0, projID);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (1);
    }
    return (0);
}

QStringList Sqlite::get_terms_by_proj (QString projID, unsigned int myLimit, unsigned int myOffset) {
    QSqlQuery sqlquery(dbconn);
    QStringList resultSet;
    QString auxquery;

    /*
     * Note: this time, we'll use :placeholders instead of ?
     * to populate prepared statements
     *
     * */

    if (myLimit > 0) {
        auxquery = " LIMIT ? OFFSET ? ";
    } else {
        auxquery = "";
    }

    if ( projID != "" ) {
        sqlquery.prepare("SELECT t1.id, t1.origterm, t1.targterm, t1.context, "\
                     " t2.name AS type, t2.shortname, t3.name AS project "\
                     " FROM term AS t1, type AS t2, project AS t3 "\
                     " WHERE (t1.projectID = :whichproj) AND "\
                     " (t1.projectID = t3.id) AND (t1.typeID = t2.id) " \
                     " ORDER BY t1.origterm ASC "\
                     + auxquery);
        sqlquery.bindValue(0, projID.toUInt());
        if (myLimit > 0) {
            sqlquery.bindValue(1, myLimit);
            sqlquery.bindValue(2, myOffset);
        }

    } else {
        sqlquery.prepare("SELECT t1.id, t1.origterm, t1.targterm, t1.context, "\
                         " t2.name AS type, t2.shortname, t3.name AS project"\
                         " FROM term AS t1, type AS t2, project AS t3 "\
                         " WHERE (t1.typeID = t2.id) AND (t1.projectID = t3.id) " \
                         " ORDER BY t1.origterm ASC "\
                         + auxquery);
        if (myLimit > 0) {
            sqlquery.bindValue(0, myLimit);
            sqlquery.bindValue(1, myOffset);
        }
    }


    sqlquery.exec();

    /*
     * Note: use 'lastQuery()' method to check SQL query.
     *  qDebug() << sqlquery.lastQuery();
     * */
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (resultSet);
    }

    while (sqlquery.next()) {

        resultSet.append(sqlquery.value(0).toString()); // Term ID
        resultSet.append(sqlquery.value(1).toString()); // Original term
        resultSet.append(sqlquery.value(2).toString()); // Target term
        resultSet.append(sqlquery.value(3).toString()); // Context
        resultSet.append(sqlquery.value(4).toString()); // type name
        resultSet.append(sqlquery.value(5).toString()); // type shortname
        resultSet.append(sqlquery.value(6).toString()); // project name
    }

    return (resultSet);
}

QStringList Sqlite::searchTerm(QString projID, QString term) {
    QSqlQuery sqlquery(dbconn);
    QStringList resultSet;

    if ( projID != "" ) {

        /*
         * Note: don't use '%' wildcard into .prepare(), it does not work. If
         *     U need a wildcard, use it into .bindValue().
         * */

        sqlquery.prepare("SELECT t1.id, t1.origterm, t1.targterm, t1.context, "\
                     " t2.name AS type, t2.shortname, t3.name AS project "\
                     " FROM term AS t1, type AS t2, project AS t3 "\
                     " WHERE (t1.projectID = :whichproj) AND (LOWER(t1.origterm) LIKE :whichterm) AND "\
                     " (t1.projectID = t3.id) AND (t1.typeID = t2.id) " \
                     " ORDER BY t1.origterm ASC ");
        sqlquery.bindValue(":whichproj", projID.toUInt());
        sqlquery.bindValue(":whichterm", "%"+term.toLower()+"%");
    } else {
        sqlquery.prepare("SELECT t1.id, t1.origterm, t1.targterm, t1.context, "\
                         " t2.name AS type, t2.shortname, t3.name AS project"\
                         " FROM term AS t1, type AS t2, project AS t3 "\
                         " WHERE (LOWER(t1.origterm) LIKE :whichterm) AND (t1.typeID = t2.id) "\
                         " AND (t1.projectID = t3.id) " \
                         " ORDER BY t1.origterm ASC ");
        sqlquery.bindValue(":whichterm", "%"+term.toLower()+"%");
    }

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (resultSet);
    }

    while (sqlquery.next()) {
        resultSet.append(sqlquery.value(0).toString()); // Term ID
        resultSet.append(sqlquery.value(1).toString()); // Original term
        resultSet.append(sqlquery.value(2).toString()); // Target term
        resultSet.append(sqlquery.value(3).toString()); // Context
        resultSet.append(sqlquery.value(4).toString()); // type name
        resultSet.append(sqlquery.value(5).toString()); // type shortname
        resultSet.append(sqlquery.value(6).toString()); // project name
    }

    return (resultSet);
}
