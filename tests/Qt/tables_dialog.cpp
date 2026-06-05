//-------------------------------------------------------------------------
// File : tables_dialog.cpp
// Date : 26-May-26 : initial definition
//
// Description:
//    Manage display of DCW "tables" of map info to the dialog pop-up.
//-------------------------------------------------------------------------
#include <iostream>

#include <QPlainTextEdit>

#include "tables_dialog.h"

Tables_Dialog_Window::Tables_Dialog_Window(QWidget *parent)

   : QDialog(parent),
     ui(new Ui_Tables_Dialog_Window)

{
    ui->setupUi(this);
}

Tables_Dialog_Window::~Tables_Dialog_Window()
{
    delete ui;
}

void Tables_Dialog_Window::update(const std::string table_data)
{
    QPlainTextEdit *te = ui->table_info_text_area;

    QString qs = QString::fromStdString(table_data);

    te->setPlainText(qs);
}

/* EOF */

