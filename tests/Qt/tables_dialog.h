//-------------------------------------------------------------------------
// File : tables_dialog.h
// Date : 26-May-26 : initial definition
//
// Description:
//    Manage display of DCW "tables" of map info to the dialog pop-up.
//-------------------------------------------------------------------------
#ifndef TABLES_DIALOG_H
#define TABLES_DIALOG_H

#include <QDialog>

#include "ui_tables_dialog.h" // <-- Generated during build from ui

class Tables_Dialog_Window : public QDialog
{
    Q_OBJECT

public:
    Tables_Dialog_Window(QWidget *parent = nullptr);

    ~Tables_Dialog_Window();

    void update(const std::string table_data);

private:
    Ui_Tables_Dialog_Window *ui;   // from ui_tables_dialog.h
};
#endif // TABLES_DIALOG_H

/* EOF */

