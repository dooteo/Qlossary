/*
 *
 * File: newlanguage.h
 * Description: this file contains headers for NewLanguage dialog.
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef NEWLANGUAGE_H
#define NEWLANGUAGE_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class NewLanguage;
}

class NewLanguage : public QDialog
{
    Q_OBJECT

public:
    explicit NewLanguage(QWidget *parent = 0);
    ~NewLanguage();

    QStringList getData();

private slots:
    void on_btn_cancel_clicked();

    void on_btn_save_clicked();

private:
    Ui::NewLanguage *ui;
};

#endif // NEWLANGUAGE_H
