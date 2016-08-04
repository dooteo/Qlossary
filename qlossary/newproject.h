/*
 *
 * File: newproject.h
 * Description: this file contains headers for NewProject dialog.
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <QStringList>
#include <QSize>

#include "sqlite.h"
#include "mainwindow.h"

namespace Ui {
    class NewProject;
}

class NewProject : public QDialog {
    Q_OBJECT

public:
    explicit NewProject(QWidget *parent = 0);
    ~NewProject();
    void setData(QStringList langs);
    QStringList getData();

private slots:
    void on_btn_Cancel_clicked();
    void on_btn_addNewProject_clicked();

private:
    void fill_combos();
    Ui::NewProject *ui;
    QStringList langaux;
};

#endif // NEWPROJECT_H
