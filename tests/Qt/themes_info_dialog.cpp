//-------------------------------------------------------------------------
// File : themes_info_dialog.cpp
// Date : 26-May-26 : initial definition
//
// Description:
//    Manage display of DCW "theme" data to the dialog pop-up.
//-------------------------------------------------------------------------
#include <iostream>

#include <QPlainTextEdit>

#include "themes_info_dialog.h"

Theme_Info_Dialog_Window::Theme_Info_Dialog_Window(QWidget *parent)

   : QDialog(parent),
     ui(new Ui_Theme_Info_Dialog_Window)

{
    ui->setupUi(this);
}

Theme_Info_Dialog_Window::~Theme_Info_Dialog_Window()
{
    delete ui;
}

void Theme_Info_Dialog_Window::update(const std::string theme_data)
{
    QPlainTextEdit *te = ui->theme_info_text_area;

    if (te)
    {
        std::cout << "found text-edit widget\n";
    }
}

/* EOF */

