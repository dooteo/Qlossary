/*
 *
 * File: sqlite.cpp
 * Description: this file contains basic methods to handle with Sqlite DB connections
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#include "sqlite.h"

/*
 * ---- Constructors ----
 */

Sqlite::Sqlite() {
}

Sqlite::~Sqlite() {
    qDebug() << "DB closed...\n";
    dbconn.close();
}

/*
 * ---- Methods ----
*/
void Sqlite::open(QString dbname, QString abspath) {

    //dbname.append(DB_EXTENSION);

    if (abspath == "") {
        // Get absolute path to create a DB in
        abspath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    }

    if (QDir(abspath).exists() ){
        qDebug() << abspath << " exists!\n";
    } else {
        QDir().mkdir(abspath);
        qDebug() << abspath << " created!\n";
    }

    dbconn = QSqlDatabase::addDatabase("QSQLITE",DB_CONNNAME);
    dbconn.setDatabaseName(abspath + QDir::separator() + dbname + DB_EXTENSION);

    if (dbconn.open()) {
        qDebug() << "DB opened...\n";
    } else {
        qDebug() << "Could not open DB\n";
    }

    dbtrash= QSqlDatabase::addDatabase("QSQLITE",DB_CONNNAME_TRASH);
    dbtrash.setDatabaseName(abspath + QDir::separator() + dbname + DB_TRASH_EXTENSION);
    if (dbtrash.open()) {
        qDebug() << "DB opened...\n";
    } else {
        qDebug() << "Could not open DB\n";
    }
    /*
     * Note: use 'connectionNames()' method to get databases' names
     *  qDebug() << dbtrash.connectionNames();
     * */




    /*
     * Note: As default, SQLite has foreign Keys disabled.
     *      That's why it must be activate each time
     *      a DB connection is opened
    */
    this->activate_foreign_keys();


    status = this->create_tables();
    // Brand new tables created...
    if (status == TB_CREATED) {
        // ...Let's Populate tables!!
        this->ins_in_language("Basque", "eu");
        this->ins_in_language("English", "en_US");
        this->ins_in_language("Spanish", "es_ES");
    }

    this->create_trash_tables();
}

void Sqlite::activate_foreign_keys() {

    QSqlQuery sqlquery("", dbconn.database(DB_CONNNAME));
    QSqlQuery sqltrash("", dbtrash.database(DB_CONNNAME_TRASH));


    // DB_ACTIVATE_FOREIGNKEYS is a #define at sqlite.h file
    sqlquery.prepare(DB_ACTIVATE_FOREIGNKEYS);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
    } else {
        qDebug() << "Glossary Foreign keys activated...";
    }


    sqltrash.prepare(DB_ACTIVATE_FOREIGNKEYS);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
    } else {
        qDebug() << "TrashDB: Foreign keys activated...";
    }

}

unsigned int Sqlite::create_tables() {

    QSqlQuery sqlquery("", dbconn.database(DB_CONNNAME));

    status = TB_CREATED; // Initialize

    QStringList tables = dbconn.tables();
     if (dbconn.tables().contains("project")) {
         qDebug() << "DB file has _project_ table.";
         return (TB_EXISTS);
     }

    // CREATE_TB_LANGUAGE is a #define at sqlite.h file
    sqlquery.prepare(CREATE_TB_LANGUAGE);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Table _language_ created!";

    sqlquery.prepare(CREATE_TB_PROJECT);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Table _project_ created!";

    // These defines are just for test multiline string defines!
    sqlquery.prepare(CREATE_TB_TYPE);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Table _type_ created!";

    sqlquery.prepare(CREATE_TB_TERM);
    if (! sqlquery.exec() ){
        qDebug() << sqlquery.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Table _term_ created!";

    // ---- ---- Populate some tables ---- ----

    /*
     * Note: this time, we're going to use execBatch() method to
     *   run multiple INSERTs at once
     * */

    sqlquery.prepare("INSERT INTO type (name, shortname, description) VALUES (?, ?, ?);");
    QVariantList names;
    names << "Noun" << "Pronoun" << "Adjective" \
         << "Verb" << "Phrasal Verb" << "Adverb" << "Verbal expression" \
         << "Article" << "Preposition" \
         << "Conjunction" << "Interjection";
    sqlquery.addBindValue(names);

    QVariantList shortnames;
    shortnames << "n" << "pron" << "adj" \
              << "v" << "v ph" << "adv" << "v expr"\
              << "prep" << "art" \
              << "conj" << "interj";
    sqlquery.addBindValue(shortnames);

    QVariantList descp;
    descp << "Refers to person, place, thing, quality, etc." \
          << "Replaces noun, for example: _He_ took the cookie and ate _it_."\
          << "Describes a noun or pronoun, for example: "
             "a _tall_ girl; an _interesting_ book."\
          << "Verb taking a direct object" \
          << "Verb with adverb(s) or preposition(s), having special meaning." \
          << "Describes a verb, adjective, adverb, or clause, for example: "
             "come _quickly_; _very_ rare; happening _now_"\
          << "Phrase with special meaning functioning as verb, for example: "
             "put their heads together; come to an end."\
          << "Relates noun or pronoun to another element of sentence, for example: "
             "a picture _of_ John; She walked _from_> my house _to_ yours."\
          << "English has two types of articles: definite (the) and indefinite (a, an.)"\
          << "Connects words, clauses, and sentences, for example: "
             "and; but; because; in order that."\
          << "Exclamation, for example: Oh no!; Wow!"; // Interjection
    sqlquery.addBindValue(descp);

    if (! sqlquery.execBatch() ){
        qDebug() << sqlquery.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Table _type_ populated!";

    return (status);

}
// ---- ---- Create tables into Trasb DB ---- ----
unsigned int Sqlite::create_trash_tables() {
    QSqlQuery sqltrash("", dbtrash.database(DB_CONNNAME_TRASH));
    QStringList tables = dbtrash.tables();

    status = TB_CREATED; // Initialize

     if (dbtrash.tables().contains("project")) {
         qDebug() << "TRASH DB file has _project_ table.\n";
         return (TB_EXISTS);
     }
    // CREATE_TB_LANGUAGE is a #define at sqlite.h file
    sqltrash.prepare(CREATE_TB_LANGUAGE);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Trash DB: Table _language_ created!";

    sqltrash.prepare(CREATE_TRASH_TB_PROJECT);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Trash DB: Table _project_ created!";

    // These defines are just for test multiline string defines!
    sqltrash.prepare(CREATE_TRASH_TB_TERM);
    if (! sqltrash.exec() ){
        qDebug() << sqltrash.lastError();
        return (TB_WITH_ERROR);
    }
    qDebug() << "Trash DB: Table _term_ created!";

    return (status);
}
