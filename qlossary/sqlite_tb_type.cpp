/*
 *
 * File: sqlite_tb_type.cpp
 * Description: this file contains methods to handle 'type' table
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "sqlite.h"

/*
 * Get all languages data from main glossary DB
 *
 * */
QStringList Sqlite::getAllTypes() {
    QStringList resultSet;
    QSqlQuery sqlquery(dbconn);

    qDebug() << "get TYpes!";

    sqlquery.prepare("SELECT id, name, shortname, description FROM type ORDER BY name ASC;");

    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        resultSet = QStringList() << "Error"  << "Error";
        return (resultSet);
    }

    while (sqlquery.next()) {
        resultSet.append(sqlquery.value(0).toString()); // ID
        resultSet.append(sqlquery.value(1).toString()); // name
        resultSet.append(sqlquery.value(2).toString()); // shortname
        resultSet.append(sqlquery.value(3).toString()); // description
    }

    qDebug() << "Types fetched";
    return (resultSet);
}
