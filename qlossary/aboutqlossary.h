/*
 *
 * File: aboutqlossary.h
 * Description: this file contains headers for AboutQlossary dialog.
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef ABOUTLIONOL_H
#define ABOUTLIONOL_H

#include <QDialog>
#include <QLayout>

namespace Ui {
class AboutQlossary;
}

class AboutQlossary : public QDialog
{
    Q_OBJECT

public:
    explicit AboutQlossary(QWidget *parent = 0);
    ~AboutQlossary();

private slots:
    void on_btn_close_about_dlg_clicked();

private:
    Ui::AboutQlossary *ui;
};

#endif // ABOUTLIONOL_H
