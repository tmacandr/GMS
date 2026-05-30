#include "dcw_browse_main_window.h"
#include "ui_dcw_browse_main_window.h"

dcw_browse_Main_Window::dcw_browse_Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dcw_browse_Main_Window)
{
    ui->setupUi(this);
}

dcw_browse_Main_Window::~dcw_browse_Main_Window()
{
    delete ui;
}

