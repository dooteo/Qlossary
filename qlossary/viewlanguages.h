/*
 *
 * File: viewlanguage.h
 * Description: this file contains headers for ViewLanguage dialog.
 * Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016
 *
 * */

#ifndef VIEWLANGUAGES_H
#define VIEWLANGUAGES_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class ViewLanguages;
}

class ViewLanguages : public QDialog {
    Q_OBJECT

public:
    explicit ViewLanguages(QWidget *parent = 0);
    ~ViewLanguages();

    void setData(QStringList langs);

private slots:
    void on_btn_close_clicked();

private:
    Ui::ViewLanguages *ui;
};

#endif // VIEWLANGUAGES_H
