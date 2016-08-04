/*
 *
 * File: viewproject.h
 * Description: this file contains headers for ViewProject dialog.
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef VIEWPEOJECTS_H
#define VIEWPEOJECTS_H

#include <QDialog>
#include <QStringList>
#include <QListWidgetItem>
#include <QPair>

namespace Ui {
class ViewProjects;
}

class ViewProjects : public QDialog
{
    Q_OBJECT

public:
    explicit ViewProjects(QWidget *parent = 0);
    ~ViewProjects();
    void setData(QStringList projs);
    QPair<double, QString> get_project();

private slots:
    void on_btn_close_clicked();
    void on_btn_select_clicked();

    /*
    * Note: next methods are commented, this ViewProjects class
    * constructor has set Signal-Slot connections.
    *
    * void on_lst_projects_doubleClicked(const QModelIndex &index);
    * void on_lst_projects_clicked(const QModelIndex &index);
    * */
    void selected_project(QListWidgetItem*);


private:
    Ui::ViewProjects *ui;
    QVector<double> projectsID;
    QPair<double, QString> cur_project;
};

#endif // VIEWPEOJECTS_H
